#include "pch.h"
#include "NavMesh.h"
#include "RenderModule.h"
NavMeshRenderingObject::NavMeshRenderingObject(RenderModule* )
{

}

NavMeshRenderingObject::NavMeshRenderingObject(NavMeshRenderingObject const& rhs)
{

}

NavMeshRenderingObject::NavMeshRenderingObject(NavMeshRenderingObject&& rhs) noexcept
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

auto NavMeshRenderingObject::PushPoint(f32 x, f32 y, f32 z)
{
    
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
    auto& rVerties{ m_obj->m_vertices };
    auto& rTriangles{ m_obj->m_triangles };
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

}
