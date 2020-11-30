#include "pch.h"
#include "SimpleColliderBoxObject.h"
#include "RenderModule.h"
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
}

auto SimpleBoxObject::GetColor() const -> XMFLOAT4 const&
{
	return m_color;
}

auto SimpleBoxObject::Clone() const -> RenderObject*
{
    return new SimpleBoxObject(*this);
}

auto SimpleBoxObject::PrepareRender(RenderModule* pRenderModule) -> void
{

}

auto SimpleBoxObject::GetWidth() const -> f32
{
	return m_width;
}

auto SimpleBoxObject::GetHeight() const -> f32
{
	return m_height;
}

auto SimpleBoxObject::GetDepth() const -> f32
{
	return m_depth;
}

auto SimpleBoxObject::SetWidth(f32 val)  -> void
{
	m_width = val;
}

auto SimpleBoxObject::SetHeight(f32 val)  -> void
{
	m_height = val;
}

auto SimpleBoxObject::SetDepth(f32 val)  -> void
{
	m_depth = val;

}

auto SimpleBoxObject::Initialize(RenderModule* pRenderModule, f32 width, f32 height, f32 depth) -> HRESULT
{
	m_height = height;
	m_width = width;
	m_depth = depth;
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
	m_width{},
	m_height{},
	m_depth{},
	m_material{},
	m_pMesh{},
	m_entity{new SimpleBoxEntity{this}}
{
}

SimpleBoxObject::SimpleBoxObject(SimpleBoxObject const& rhs):
	RenderObject{rhs},
	m_entity{ new SimpleBoxEntity {this} },
	m_width{ rhs.m_width},
	m_height{ rhs.m_height},
	m_depth{ rhs.m_depth},
	m_material{rhs.m_material},
	m_color{rhs.m_color},
	m_pMesh{rhs.m_pMesh}
{
}

SimpleBoxEntity::SimpleBoxEntity(SimpleBoxObject* pObj):
	m_obj{pObj}
{
}

auto SimpleBoxEntity::Render(RenderModule* pRenderModule) -> void
{
	auto meterial{ m_obj->m_material };
	auto width{ m_obj->m_width };
	auto height{ m_obj->m_height };
	auto depth{ m_obj->m_depth };
	auto* pTransfrom{ &m_obj->m_transform };
	auto pMesh{ m_obj->m_pMesh };
	DWORD fillMode;
	COMPtr<IDirect3DDevice9> pDevice;
	pDevice->GetRenderState(D3DRS_FILLMODE, &fillMode);
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	pRenderModule->GetDevice(&pDevice);
	pDevice->SetMaterial(&meterial);

	XMMATRIX mSize{ XMMatrixScaling(width , height, depth) };
	XMMATRIX mTransform{ XMLoadFloat4x4(pTransfrom) };
	mTransform = mSize * mTransform;
	auto* pWorld{ reinterpret_cast<D3DMATRIX*>(&mTransform) };

	pDevice->SetTransform(D3DTS_WORLD, pWorld);//vertex shader
	pMesh->DrawSubset(0);
	pDevice->SetRenderState(D3DRS_FILLMODE, fillMode);
}
