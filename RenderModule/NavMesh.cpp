#include "pch.h"
#include "NavMesh.h"
#include "RenderModule.h"
#include <array>
using namespace DirectX;
//std::vector<u16> m_positionIndexing;
//std::unordered_map<DirectX::XMFLOAT3, size_t> m_table;
//std::vector<Vertex > m_vertices;
//std::vector<Triangle > m_triangles;
//std::shared_ptr<NavMeshEntity> m_entity;
NavMeshRenderingObject::NavMeshRenderingObject(RenderModule* pRenderState):
    m_entity{new Entity{this}}
{

}

NavMeshRenderingObject::NavMeshRenderingObject(NavMeshRenderingObject const& rhs):
    RenderObject{ rhs },
    m_entity{ new Entity{this} },
    m_table{ rhs.m_table },
    m_vertices{ rhs.m_vertices },
    m_triangles{m_triangles },
    m_positionIndexing{rhs.m_positionIndexing}
{

}

NavMeshRenderingObject::NavMeshRenderingObject(NavMeshRenderingObject&& rhs) noexcept:
    RenderObject{ std::move(rhs) },
    m_entity{ new Entity{this} },
    m_table{ std::move(rhs.m_table) },
    m_vertices{std::move(rhs.m_vertices)},
    m_triangles{ std::move(rhs.m_triangles) },
    m_positionIndexing{ std::move( rhs.m_positionIndexing )}
{

}

auto NavMeshRenderingObject::Create(RenderModule* pRenderModule, NavMeshRenderingObject** pOut) -> HRESULT
{
    try
    {
        if (pOut == nullptr)
        {
            return E_POINTER;
        }
        NavMeshRenderingObject navMeshObj{ pRenderModule };
        *pOut = new NavMeshRenderingObject{ std::move(navMeshObj) };
    }
    catch (HRESULT ex)
    {
        return ex;
    }
    return S_OK;
}

auto NavMeshRenderingObject::Clone() const -> RenderObject*
{
    return new NavMeshRenderingObject{ *this };
}

auto NavMeshRenderingObject::PushPoint(DirectX::XMFLOAT3 const& pt, DirectX::XMFLOAT3 const& cameraAt, DirectX::XMFLOAT3 const& rotation)
{
    Vertex newVertex{};
    newVertex.vPosition = pt;
    newVertex.diffuseColor = D3DCOLOR_COLORVALUE(.5f, 1.f, .5f, .5f);
    auto it{ m_table.find(newVertex.vPosition) };
    if (it == m_table.end())
    {
        it = m_table.emplace(newVertex.vPosition, m_vertices.size()).first;
        m_vertices.emplace_back(newVertex);
    }
    m_positionIndexing.emplace_back(static_cast<u16>(it->second) );
    //버텍스가 3개면, 이게 폴리곤으로 집어 넣자.
    if (m_positionIndexing.size() == 3)
    {
        //하지만, 우선 해당 삼각형이 시계방향으로 들어왔는지 체크하고 아니면 돌리자
        //뷰행렬을 구축해준다.
        XMMATRIX cameraTransform{ XMMatrixRotationRollPitchYaw(rotation.x,rotation.y,rotation.z) };
        cameraTransform.r[3] = XMVectorSetW(XMLoadFloat3(&cameraAt), 1.f);

        XMMATRIX viewMatrix{ XMMatrixTranspose( XMMatrixRotationRollPitchYaw(rotation.x,rotation.y,rotation.z) )};
        viewMatrix.r[3] = XMVectorSetW(XMLoadFloat3(&cameraAt) * -1.f, 1.f);
        std::array<XMVECTOR, 3> vTriangle{};
        auto triangleIt = vTriangle.begin();
        for (auto index : m_positionIndexing)
        {
            *triangleIt = XMLoadFloat3(&m_vertices[static_cast<size_t>(index)].vPosition);
        }
        for (auto vertex : vTriangle)
        {
            vertex = XMVector3TransformCoord(vertex, viewMatrix);
        }
        XMVECTOR vNormal{ -XMVector3Cross(vTriangle[1] - vTriangle[0], vTriangle[2] - vTriangle[0]) };
        float dotRes{};
        XMStoreFloat(&dotRes, XMVector3Dot(cameraTransform.r[2], vNormal));
        if(dotRes < 0)//0보다 작으면 둔각으로서 정점찍은 순서가 틀렸다.
        {
            std::swap(m_positionIndexing[1], m_positionIndexing[2]);//그러니까 1번이랑 2번의 순서를 바꿔주자.
        }
        Triangle newTriangle{};
        newTriangle[0] = m_positionIndexing[0];
        newTriangle[1] = m_positionIndexing[1];
        newTriangle[2] = m_positionIndexing[2];
        this->m_triangles.emplace_back(newTriangle);
        m_positionIndexing.clear();
    }
}
auto NavMeshRenderingObject::PrepareRender(RenderModule* pRenderModule) -> void
{
    pRenderModule->AddRenderEntity(RenderModule::Kind::NAVIMASH, m_entity);
}

NavMeshEntity::NavMeshEntity(NavMeshRenderingObject* pObj):
    m_obj{pObj}
{

}

auto NavMeshEntity::Render(RenderModule* pRenderModule) -> void 
{
    COMPtr<IDirect3DDevice9> pDevice{};
    XMMATRIX mWorld{ XMMatrixIdentity() };
    pDevice->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&mWorld));
    auto& rVerties{ m_obj->m_vertices };
    auto& rTriangles{ m_obj->m_triangles };
    auto& rCompositingPointIndex{ m_obj->m_positionIndexing };
    pRenderModule->GetDevice(&pDevice);
    pDevice->SetFVF(NavMeshRenderingObject::FVF);
    pDevice->DrawIndexedPrimitiveUP(
        D3DPT_TRIANGLELIST,
        0,
        rVerties.size(),
        rTriangles.size(),
        rTriangles.data(),
        NavMeshRenderingObject::INDEX_FMT,
        rVerties.data(),
        sizeof(NavMeshRenderingObject::Vertex)
    );
    pDevice->SetRenderState(D3DRS_POINTSIZE, 2);
    pDevice->DrawIndexedPrimitiveUP(
        D3DPT_POINTLIST,
        0,
        rVerties.size(),
        rCompositingPointIndex.size(),
        rCompositingPointIndex.data(),
        NavMeshRenderingObject::INDEX_FMT,
        rVerties.data(),
        sizeof(NavMeshRenderingObject::Vertex)
    );
}
