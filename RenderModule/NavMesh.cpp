#include "pch.h"
#include "NavMesh.h"
#include "RenderModule.h"
#include "Renderer.h"
#include <array>
#undef max
#undef min
using namespace DirectX;
//std::vector<u16> m_positionIndexing;
//std::unordered_map<DirectX::XMFLOAT3, size_t> m_table;
//std::vector<Vertex > m_vertices;
//std::vector<Triangle > m_triangles;
//std::shared_ptr<NavMeshEntity> m_entity;
NavMeshRenderingObject::NavMeshRenderingObject(RenderModule* pRenderState):
    m_entity{new Entity{this}},
    m_weldDistance{0.25f}
{

}

NavMeshRenderingObject::NavMeshRenderingObject(
    RenderModule* pRenderModule,
    DirectX::XMFLOAT4 const* vertices,
    u16 const* indices,
    u32 verticesCount,
    u32 indexCout):
    m_entity{ new Entity{this} },
    m_weldDistance{ 0.25f }
{
    u32 const triangleCount{ indexCout / 3 };
    m_vertices.reserve(verticesCount);
    m_triangles.reserve(triangleCount);
    
    for (u32 i = 0; i < triangleCount; ++i)
    {
        Triangle triangle{};
        size_t baseOffset{ static_cast<size_t>(i * 3) };
        triangle[0] = *(indices + baseOffset + 0);
        triangle[1] = *(indices + baseOffset + 1);
        triangle[2] = *(indices + baseOffset + 2);
        m_triangles.emplace_back(triangle);
    }
    auto vertexIt{ vertices };
    auto const vertexEnd{ vertices + verticesCount };
    for (; vertexIt != vertexEnd; ++vertexIt)
    {
        Vertex vertex{};
        vertex.diffuseColor = D3DCOLOR_COLORVALUE(1.0f, 0.f, 0.0f, .5f);
        vertex.vPosition = XMFLOAT3{ vertexIt->x,vertexIt->y ,vertexIt->z };
        m_vertices.emplace_back(vertex);
    }
}

NavMeshRenderingObject::NavMeshRenderingObject(NavMeshRenderingObject const& rhs):
    RenderObject{ rhs },
    m_entity{ new Entity{this} },
    m_vertices{ rhs.m_vertices },
    m_triangles{m_triangles },
    m_positionIndexing{rhs.m_positionIndexing},
    m_weldDistance{ rhs.m_weldDistance }
{

}

NavMeshRenderingObject::NavMeshRenderingObject(NavMeshRenderingObject&& rhs) noexcept:
    RenderObject{ std::move(rhs) },
    m_entity{ new Entity{this} },
    m_vertices{std::move(rhs.m_vertices)},
    m_triangles{ std::move(rhs.m_triangles) },
    m_positionIndexing{ std::move( rhs.m_positionIndexing )},
    m_weldDistance{rhs.m_weldDistance}
{
    rhs.m_weldDistance = 0.25f;
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

auto NavMeshRenderingObject::Load(RenderModule* pRenderModule, DirectX::XMFLOAT4 const* vertices, u16 const* indices, u32 verticesCount, u32 indexCout, NavMeshRenderingObject** pOut) -> HRESULT
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

auto NavMeshRenderingObject::PushPoint(DirectX::XMFLOAT3 const& pt, DirectX::XMFLOAT3 const& cameraAt, DirectX::XMFLOAT3 const& rotation)->void
{
    XMVECTOR vCurrentPt{ XMLoadFloat3(&pt) };
    size_t vertexIndex{ std::numeric_limits<size_t>::max() };
    f32 vertexDistance{ std::numeric_limits<float>::max() };
    for(size_t i = 0; i < m_vertices.size();++i)
    {
        auto& vertex{ m_vertices[i] };
        XMVECTOR vVertexPos{ XMLoadFloat3(&vertex.vPosition) };
        f32 length{};
        XMStoreFloat(&length, XMVector3Length(vCurrentPt - vVertexPos));
        if (length <= vertexDistance)
        {
            vertexIndex = i;
            vertexDistance = length;
        }
    }
    if (vertexDistance > m_weldDistance)
    {
        vertexIndex = std::numeric_limits<size_t>::max();
    }
    if (vertexIndex == std::numeric_limits<size_t>::max())
    {
        Vertex newVertex{};
        newVertex.vPosition = pt;
        newVertex.diffuseColor = D3DCOLOR_COLORVALUE(1.0f, 0.f, 0.0f, .5f);
        vertexIndex = m_vertices.size();
        m_vertices.emplace_back(newVertex);
    }
    for (auto it : m_positionIndexing)
    {
        if (it == static_cast<u16>(vertexIndex))
        {
            return;
        }
    }
    m_positionIndexing.emplace_back(static_cast<u16>(vertexIndex) );
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
            ++triangleIt;
        }
        for (auto& vertex : vTriangle)
        {
            vertex = XMVector3TransformCoord(vertex, viewMatrix);
        }
        XMVECTOR vNormal{ -XMVector3Cross(vTriangle[1] - vTriangle[0], vTriangle[2] - vTriangle[0]) };
        f32 dotRes{};
        XMStoreFloat(&dotRes, XMVector3Dot(XMVectorSet(0.f, 0.f, 1.f, 0.f), vNormal));
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
auto NavMeshRenderingObject::GetVertexCount() const -> u32
{
    return static_cast<u32>(m_vertices.size());
}
auto NavMeshRenderingObject::GetVertex(u32 index) const -> DirectX::XMFLOAT3 const&
{
    return m_vertices[index].vPosition;
}
auto NavMeshRenderingObject::SetVertex(u32 index, DirectX::XMFLOAT3 const& position) -> void
{
    m_vertices[index].vPosition = position;
}
auto NavMeshRenderingObject::RemoveLastTriangle() -> void
{
    m_triangles.pop_back();
}
auto NavMeshRenderingObject::SetWeldDistance(f32 value) -> void
{
    m_weldDistance = value;
}
auto NavMeshRenderingObject::GetWeldDistance() const -> f32
{
    return m_weldDistance;
}
auto NavMeshRenderingObject::PrepareRender(IRenderer* pRenderer) -> void
{
    pRenderer->AddEntity(RenderModule::Kind::NAVIMASH, m_entity);
}

auto NavMeshRenderingObject::GetIndexCount() const -> u32
{
    return static_cast<u32>(m_triangles.size() * 3);
}

auto NavMeshRenderingObject::GetVertices(DirectX::XMFLOAT4* const pBuffer, u32 bufferSize) -> HRESULT
{
    auto it = pBuffer;
    auto const end = reinterpret_cast<XMFLOAT4*>((reinterpret_cast<u8*>(pBuffer) + bufferSize));
    for (auto& vertex : m_vertices)
    {
        *it = XMFLOAT4{ vertex.vPosition.x, vertex.vPosition.y ,vertex.vPosition.z ,1.f };
        ++it;
    }
    return S_OK;
}

auto NavMeshRenderingObject::GetIndices(u16* const pBuffer, u32 bufferSize) -> HRESULT
{
    auto it = pBuffer;
    auto const end = reinterpret_cast<u16*>((reinterpret_cast<u8*>(pBuffer) + bufferSize));
    for (auto& triangle : m_triangles)
    {
        for (int i = 0; i < 3; ++i)
        {
            *it = triangle[i];
            ++it;
        }
    }
    return S_OK;
}


NavMeshEntity::NavMeshEntity(NavMeshRenderingObject* pObj):
    m_obj{pObj}
{

}
DWORD FtoDW(float f) { return *((DWORD*)&f); }
auto NavMeshEntity::Render(RenderModule* pRenderModule, IRenderer* pRenderer) -> void
{
    COMPtr<IDirect3DDevice9> pDevice{};
    XMMATRIX mWorld{ XMMatrixIdentity() };
    pRenderModule->GetDevice(&pDevice);
    pDevice->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&mWorld));
    auto& rVerties{ m_obj->m_vertices };
    auto& rTriangles{ m_obj->m_triangles };
    auto& rCompositingPointIndex{ m_obj->m_positionIndexing };
    pDevice->SetFVF(NavMeshRenderingObject::FVF);
    HRESULT hr{};
    pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
    if (rTriangles.size() != 0)
    {
        pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
        hr = pDevice->DrawIndexedPrimitiveUP(
            D3DPT_TRIANGLELIST,
            0,
            rVerties.size(),
            rTriangles.size(),
            rTriangles.data(),
            NavMeshRenderingObject::INDEX_FMT,
            rVerties.data(),
            sizeof(NavMeshRenderingObject::Vertex)
        );
        

        assert(SUCCEEDED(hr));
    }
    pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    pDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDW(0.1f));
    pDevice->SetRenderState(D3DRS_POINTSIZE_MAX, FtoDW(30.0f));
    pDevice->SetRenderState(D3DRS_POINTSCALE_A, FtoDW(0.0f));
    pDevice->SetRenderState(D3DRS_POINTSCALE_B, FtoDW(0.0f));
    pDevice->SetRenderState(D3DRS_POINTSCALE_C, FtoDW(1.0f));
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
    pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

}
