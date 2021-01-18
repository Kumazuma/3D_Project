#include "pch.h"
#include "SimpleColliderBoxObject.h"
#include "RenderModule.h"
#include "Renderer.h"
#include <DirectXMath.h>
using namespace DirectX;
auto SimpleBoxObject::Create(RenderModule* pRenderModule, f32 width, f32 height, f32 depth, SimpleBoxObject** pOut) -> HRESULT
{
    HRESULT hr{};
    if (pOut == nullptr)
    {
        return E_POINTER;
    }
    SimpleBoxObject* obj{ new SimpleBoxObject{} };
    hr = obj->Initialize(pRenderModule, width, height, depth);
	if(FAILED(hr))
	{
		delete obj;
		obj = nullptr;
	}
	*pOut = obj;
    return hr;
}

auto SimpleBoxObject::SetColor(float r, float g, float b, float a) -> void
{
	m_color = XMFLOAT4{ r,g,b,a };
	m_material.Diffuse = D3DCOLORVALUE{ r,g,b,a };
	m_material.Ambient = D3DCOLORVALUE{ r,g,b,a };
	m_material.Specular = D3DCOLORVALUE{ r,g,b,a };
}

auto SimpleBoxObject::GetColor() const -> XMFLOAT4 const&
{
	return m_color;
}

auto SimpleBoxObject::Clone() const -> RenderObject*
{
    return new SimpleBoxObject(*this);
}

auto SimpleBoxObject::PrepareRender(IRenderer* pRenderer) -> void
{
	if (IsVisible() == false)return;
	pRenderer->AddEntity(RenderModule::Kind::ALPHA, m_entity);
}

auto SimpleBoxObject::GetWidth() const -> f32
{
	return m_size.x;
}

auto SimpleBoxObject::GetHeight() const -> f32
{
	return m_size.y;
}

auto SimpleBoxObject::GetDepth() const -> f32
{
	return m_size.z;
}

auto SimpleBoxObject::SetSize(f32 width, f32 height, f32 depth) -> void
{
	m_size = XMFLOAT3A{ width, height, depth };
}

auto SimpleBoxObject::SetOffset(f32 x, f32 y, f32 z) -> void
{
	m_offset = XMFLOAT3A{ x,y,z };
}

auto SimpleBoxObject::Initialize(RenderModule* pRenderModule, f32 width, f32 height, f32 depth) -> HRESULT
{
	m_size = XMFLOAT3A{ width, height, depth };
	m_offset = XMFLOAT3A{ 0,0,0 };

	HRESULT hr{ E_FAIL };

	COMPtr<IDirect3DDevice9> pDevice;
	pRenderModule->GetDevice(&pDevice);
	hr = D3DXCreateBox(pDevice.Get(), 1, 1, 1, &m_pMesh, nullptr);
	m_material.Diffuse = D3DCOLORVALUE{ 1.f, 1.f, 1.f, 1.f };
	m_material.Ambient = D3DCOLORVALUE{ 0.f, 0.f, 0.f, 1.f };
	m_material.Emissive = D3DCOLORVALUE{ 0.f, 0.f, 0.f, 1.f };
	m_material.Specular = D3DCOLORVALUE{ 0.f, 0.f, 0.f, 1.f };
	m_material.Power = 1.f;
	m_color = { 1.f,1.f,1.f,1.f };
	return S_OK;
}

SimpleBoxObject::SimpleBoxObject():
	RenderObject{},
	m_size{},
	m_offset{},
	m_material{},
	m_pMesh{},
	m_entity{new SimpleBoxEntity{this}}
{
}

SimpleBoxObject::SimpleBoxObject(SimpleBoxObject const& rhs):
	RenderObject{rhs},
	m_entity{ new SimpleBoxEntity {this} },
	m_size{ rhs.m_size },
	m_offset{ rhs.m_offset },
	m_material{rhs.m_material},
	m_color{rhs.m_color},
	m_pMesh{rhs.m_pMesh}
{
}

SimpleBoxEntity::SimpleBoxEntity(SimpleBoxObject* pObj):
	m_obj{pObj}
{
}

auto SimpleBoxEntity::Render(RenderModule* pRenderModule, IRenderer* pRenderer) -> void 
{
	auto meterial{ m_obj->m_material };
	auto size{ m_obj->m_size };
	auto offset{ m_obj->m_offset };

	auto* pTransfrom{ &m_obj->m_transform };
	auto pMesh{ m_obj->m_pMesh };
	DWORD fillMode;
	DWORD cullMode;
	COMPtr<IDirect3DDevice9> pDevice;
	COMPtr<IDirect3DTexture9> pRedTexture;
	pRenderModule->GetDevice(&pDevice);
	pRenderModule->GetSimpleColorTexture(DefaultColorTexture::RED, &pRedTexture);
	pDevice->GetRenderState(D3DRS_FILLMODE, &fillMode);
	pDevice->GetRenderState(D3DRS_CULLMODE, &cullMode);
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	COMPtr<IDirect3DTexture9> pTexture;
	//pDevice->CreateTexture(2048, 2048, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A32B32G32R32F, D3DPOOL_MANAGED, pTexture,);
	pDevice->SetMaterial(&meterial);
	pDevice->SetTexture(0, pRedTexture.Get());
	XMMATRIX mSize{ XMMatrixScaling(size.x , size.y, size.z) };
	XMMATRIX mOffset{ XMMatrixTranslation(offset.x,offset.y,offset.z) };
	XMMATRIX mTransform{ XMLoadFloat4x4(pTransfrom) };
	mTransform = mSize * mOffset * mTransform;
	auto* pWorld{ reinterpret_cast<D3DMATRIX*>(&mTransform) };

	pDevice->SetTransform(D3DTS_WORLD, pWorld);//vertex shader
	pMesh->DrawSubset(0);
	pDevice->SetRenderState(D3DRS_FILLMODE, fillMode);
	pDevice->SetRenderState(D3DRS_CULLMODE, cullMode);
}
