#include "pch.h"
#include "Line3D.h"
#include "RenderModule.h"
using namespace DirectX;
Line3D::Line3D(RenderModule* renderModule, DirectX::XMFLOAT3 const& start, DirectX::XMFLOAT3 const& end):
    m_pEntity{ new Entity{this} }
{
    m_line[0].diffuseColor = D3DCOLOR_COLORVALUE(1.f, 0.f, 0.f, 1.f);
    m_line[0].vPosition = start;

    m_line[1].diffuseColor = D3DCOLOR_COLORVALUE(1.f, 0.f, 0.f, 1.f);
    m_line[1].vPosition = end;
}

Line3D::Line3D(Line3D const& rhs):
    RenderObject{ rhs },
    m_line{ rhs.m_line },
    m_pEntity{new Entity{this}}
{
}

Line3D::Line3D(Line3D&& rhs)noexcept :
    RenderObject{std::move(rhs)},
    m_line{std::move(rhs.m_line)},
    m_pEntity{ std::move(rhs.m_pEntity) }

{
    m_pEntity->SetObject(this);
}

auto Line3D::Create(RenderModule* renderModule, DirectX::XMFLOAT3 const& start, DirectX::XMFLOAT3 const& end, Line3D** pOut) -> HRESULT
{
    try
    {
        if (pOut == nullptr)
            return E_POINTER;
        Line3D obj{ renderModule, start, end };
        *pOut = new Line3D(std::move(obj));
    }
    catch (HRESULT ex)
    {
        return ex;
    }
    return S_OK;
}

auto Line3D::GetStartPositin() const -> DirectX::XMFLOAT3
{
    return DirectX::XMFLOAT3();
}

auto Line3D::GetEndPositin() const -> DirectX::XMFLOAT3
{
    return DirectX::XMFLOAT3();
}

auto Line3D::Clone() const -> RenderObject*
{
    return new Self{ *this };
}

auto Line3D::PrepareRender(RenderModule* renderModule) -> void
{
    renderModule->AddRenderEntity(RenderModule::Kind::NAVIMASH, m_pEntity);
}

Line3DEntity::Line3DEntity(Line3D* pObj): m_pObj{pObj}
{
}

auto Line3DEntity::SetObject(Line3D* pObj) -> void
{
    m_pObj = pObj;
}

auto Line3DEntity::Render(RenderModule* renderModule) -> void
{
    COMPtr<IDirect3DDevice9> pDevice;
    renderModule->GetDevice(&pDevice);
    XMMATRIX mWorld{ XMMatrixIdentity() };
    pDevice->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&mWorld));
    pDevice->SetFVF(Line3D::FVF);
    pDevice->DrawPrimitiveUP(
        D3DPT_LINELIST,
        1,
        m_pObj->m_line.data(),
        sizeof(Line3D::Vertex)
    );
}
