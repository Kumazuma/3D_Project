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

auto SimpleBoxObject::Render(RenderModule* pRenderModule) -> void
{
	DWORD fillMode;
    COMPtr<IDirect3DDevice9> pDevice;
	pDevice->GetRenderState(D3DRS_FILLMODE, &fillMode);
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    pRenderModule->GetDevice(&pDevice);
	pDevice->SetMaterial(&m_material);
	
	XMMATRIX mSize{ XMMatrixScaling(m_width , m_height, m_depth ) };
	XMMATRIX mTransform{ XMLoadFloat4x4(&m_transform) };
	mTransform = mSize * mTransform;
	auto* pWorld{ reinterpret_cast<D3DMATRIX*>(&mTransform) };
	
	pDevice->SetTransform(D3DTS_WORLD, pWorld);//vertex shader
	m_pMesh->DrawSubset(0);
	pDevice->SetRenderState(D3DRS_FILLMODE, fillMode);
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
