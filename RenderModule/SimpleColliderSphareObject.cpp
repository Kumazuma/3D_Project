#include "pch.h"
#include "RenderModule.h"
#include "SimpleColliderSphareObject.h"
#include "Renderer.h"
using namespace DirectX;
SimpleSphereObject::SimpleSphereObject(RenderModule* pRenderModule):
    m_entity{ new Entity {this} },
    m_material{},
    m_offsetAndRadius{0.f, 0.f, 0.f, 1.f},
    m_pMesh{}
{
    HRESULT hr;
    COMPtr<IDirect3DDevice9> pDevice{};
    pRenderModule->GetDevice(&pDevice);
    hr = D3DXCreateSphere(pDevice.Get(), 1.f, 12, 12, &m_pMesh, nullptr);
    if (FAILED(hr))
    {
        throw hr;
    }
    m_material.Diffuse = D3DCOLORVALUE{ 1.f, 1.f, 1.f, 1.f };
    m_material.Ambient = D3DCOLORVALUE{ 0.f, 0.f, 0.f, 1.f };
    m_material.Emissive = D3DCOLORVALUE{ 0.f, 0.f, 0.f, 1.f };
    m_material.Specular = D3DCOLORVALUE{ 0.f, 0.f, 0.f, 1.f };
    m_material.Power = 1.f;
}

SimpleSphereObject::SimpleSphereObject(SimpleSphereObject const& rhs):
    RenderObject{rhs },
    m_entity{ new Entity{this}  },
    m_material{rhs.m_material},
    m_offsetAndRadius{rhs.m_offsetAndRadius},
    m_pMesh{rhs.m_pMesh}
{

}

SimpleSphereObject::SimpleSphereObject(SimpleSphereObject&& rhs) noexcept:
    RenderObject{std::move(rhs) },
    m_material{ std::move(rhs.m_material) },
    m_offsetAndRadius{ std::move(rhs.m_offsetAndRadius) },
    m_pMesh{ std::move(rhs.m_pMesh) },
    m_entity{ new Entity{this} }
{

}

auto SimpleSphereObject::Create(RenderModule* pRenderModule, SimpleSphereObject** pOut) -> HRESULT
{
    try
    {
        if (pOut == nullptr)
            throw E_POINTER;
        SimpleSphereObject obj{ pRenderModule };
        *pOut = new SimpleSphereObject{ std::move(obj) };
    }
    catch (HRESULT ex)
    {
        return ex;
    }
    return S_OK;
}


auto SimpleSphereObject::Clone() const -> RenderObject*
{
    return new SimpleSphereObject{ *this };
}

//auto SimpleSphereObject::PrepareRender(IRenderer* pRenderer) -> void
//{
//    if (IsVisible() == false)return;
//    pRenderer->AddEntity(RenderModule::Kind::ALPHA, m_entity);
//}

auto SimpleSphereObject::Render(IRendererBase* renderer, ID3DXEffect* effect) -> void
{
    COMPtr<IDirect3DDevice9> pDevice;
    COMPtr<IDirect3DTexture9> pRedTexture;
    renderer->GetDevice(&pDevice);
    renderer->GetRenderModuleRef().GetSimpleColorTexture(DefaultColorTexture::RED, &pRedTexture);
    pDevice->SetMaterial(&m_material);
    pDevice->SetTexture(0, pRedTexture.Get());
    m_pMesh->DrawSubset(0);
}

auto SimpleSphereObject::SetRadius(f32 radius) -> void
{
    m_offsetAndRadius.w = radius;
}
auto SimpleSphereObject::SetOffset(f32 x, f32 y, f32 z) -> void
{
    m_offsetAndRadius = XMFLOAT4{x,y,z, m_offsetAndRadius.w};
}

SimpleSphareEntity::SimpleSphareEntity(SimpleSphereObject* pObj):
    m_obj{ pObj }
{

}

auto SimpleSphareEntity::Render(RenderModule* pRenderModule, IRenderer* pRenderer) -> void
{
    //auto meterial{ m_obj->m_material };
    //auto offsetAndRadius{ m_obj->m_offsetAndRadius };

    //auto* pTransfrom{ &m_obj->m_transform };
    //auto pMesh{ m_obj->m_pMesh };
    //DWORD fillMode;
    //DWORD cullMode;
    //COMPtr<IDirect3DDevice9> pDevice;
    //COMPtr<IDirect3DTexture9> pRedTexture;
    //pRenderModule->GetDevice(&pDevice);
    //pRenderModule->GetSimpleColorTexture(DefaultColorTexture::RED, &pRedTexture);
    //pDevice->GetRenderState(D3DRS_FILLMODE, &fillMode);
    //pDevice->GetRenderState(D3DRS_CULLMODE, &cullMode);
    //pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    //pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    //pDevice->SetMaterial(&meterial);
    //pDevice->SetTexture(0, pRedTexture.Get());
    //XMMATRIX mSize{ XMMatrixScaling(offsetAndRadius.w , offsetAndRadius.w, offsetAndRadius.w) };
    //XMMATRIX mOffset{ XMMatrixTranslation(offsetAndRadius.x,offsetAndRadius.y,offsetAndRadius.z) };
    //XMMATRIX mTransform{ XMLoadFloat4x4(pTransfrom) };
    //mTransform = mSize * mOffset * mTransform;
    //auto* pWorld{ reinterpret_cast<D3DMATRIX*>(&mTransform) };

    //pDevice->SetTransform(D3DTS_WORLD, pWorld);//vertex shader
    //pMesh->DrawSubset(0);
    //pDevice->SetRenderState(D3DRS_FILLMODE, fillMode);
    //pDevice->SetRenderState(D3DRS_CULLMODE, cullMode);
}
