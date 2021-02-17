#include "pch.h"
#include <RenderModule/RenderModule.h>
#include <RenderModule/TextureRenderTarget.h>
#include <d3dx9.h>

#include "NativeRenderer.hpp"
#include "MaptoolRenderer.h"

#include "IRenderable.hpp"

#include <msclr\lock.h>
#include "msclr\marshal_cppstd.h"
#include <msclr\marshal.h>
#include "Camera.hpp"
using namespace msclr::interop;
using namespace System;
using namespace DirectX;
struct PPVertexFVF { XMFLOAT3 xyz; XMFLOAT2 uv; };
static constexpr DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;

#pragma unmanaged
void GenerateViewProjMatrix(DirectX::XMFLOAT4X4* view, DirectX::XMFLOAT4X4* proj, DirectX::XMFLOAT4X4* viewProj)
{
	using namespace DirectX;
	XMStoreFloat4x4(
		viewProj,
		XMLoadFloat4x4(view) * XMLoadFloat4x4(proj)
	);
}
void GenerateCameraPosition(DirectX::XMFLOAT4X4 const* viewSpace, DirectX::XMFLOAT4 * out)
{

	XMStoreFloat4(out, 
		XMVectorSetW(
			XMMatrixInverse(nullptr, XMLoadFloat4x4(viewSpace)).r[3], 1)
	);
}
void InverseMatrix(DirectX::XMFLOAT4X4 const* mat, DirectX::XMFLOAT4X4* out)
{
	XMStoreFloat4x4(
		out,
		XMMatrixInverse(
			nullptr,
			XMLoadFloat4x4(mat)
		)
	);
}
#pragma managed
std::wstring const RENDER_TARGET_ALBEDO{ L"RT_ALBEDO" };
std::wstring const RENDER_TARGET_NORMAL{ L"RT_NORMAL" };
std::wstring const RENDER_TARGET_DEPTH{ L"RT_DEPTH" };
std::wstring const RENDER_TARGET_SPECULAR{ L"RT_SPECULAR" };
std::wstring const LIGHT_SPECULAR = L"light_specular";
std::wstring const LIGHT_DIFFUSE = L"light_diffuse";
char constexpr ID_TEX_NORMAL_MAP[] = "g_normalMap";
char constexpr ID_TEX_SPECULAR_MAP[] = "g_specularMap";
char constexpr ID_CONST_INVSERSE_VIEW_PORJ_MATRIX[]{ "g_mInverseViewProj" };
char constexpr ID_TEX_DEPTH_MAP[]{ "g_depthMap" };

MaptoolRenderer::GraphicsDevice::GraphicsDevice(Control^ control, int width, int height)
{
	this->control = gcnew WeakReference<Control^>( control);
	HWND hwnd{reinterpret_cast<HWND>( control->Handle.ToPointer()) };
	RenderModule* renderModule{};
	RenderModule::Create(hwnd, width, height,false, &renderModule);
	this->renderModulePtr_ = renderModule;
	this->renderer_ = new NativeRenderer{ renderModule };
	this->effects_ = gcnew Dictionary<String^, size_t>{};
	COMPtr<IDirect3DDevice9> device;
	renderModulePtr_->GetDevice(&device);
	renderModule->CreateRenderTarget(RENDER_TARGET_ALBEDO, width, height, D3DFMT_A8R8G8B8);
	renderModule->CreateRenderTarget(RENDER_TARGET_NORMAL, width, height, D3DFMT_A16B16G16R16F);
	renderModule->CreateRenderTarget(RENDER_TARGET_SPECULAR, width, height, D3DFMT_A16B16G16R16F);
	renderModule->CreateRenderTarget(RENDER_TARGET_DEPTH, width, height, D3DFMT_G32R32F);
	renderModule->CreateRenderTarget(LIGHT_SPECULAR, width, height, D3DFMT_A16B16G16R16F);
	renderModule->CreateRenderTarget(LIGHT_DIFFUSE, width, height, D3DFMT_A16B16G16R16F);

	IDirect3DVertexBuffer9* vertexBuffer;
	IDirect3DIndexBuffer9* indexBuffer;
	InitalizeEffect();
	device->CreateVertexBuffer(sizeof(PPVertexFVF) * 4,  D3DUSAGE_WRITEONLY, FVF, D3DPOOL_MANAGED, &vertexBuffer, nullptr);
	device->CreateIndexBuffer(sizeof(int) * 6, D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &indexBuffer, nullptr);
	vertexBuffer_ = vertexBuffer;
	indexBuffer_ = indexBuffer;
	PPVertexFVF* pVertices;
	
	vertexBuffer_->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);
	pVertices[0].uv = XMFLOAT2{ 0.f, 0.f };
	pVertices[0].xyz = XMFLOAT3{ -1.f, 1.f, 0.f };

	pVertices[1].uv = XMFLOAT2{ 1.f, 0.f };
	pVertices[1].xyz = XMFLOAT3{ 1.f, 1.f, 0.f };

	pVertices[2].uv = XMFLOAT2{ 1.f, 1.f };
	pVertices[2].xyz = XMFLOAT3{ 1.f, -1.f, 0.f };

	pVertices[3].uv = XMFLOAT2{ 0.f, 1.f };
	pVertices[3].xyz = XMFLOAT3{ -1.f, -1.f, 0.f };
	vertexBuffer_->Unlock();
	int* pIndices;
	indexBuffer_->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 3;

	pIndices[3] = 1;
	pIndices[4] = 2;
	pIndices[5] = 3;

	indexBuffer_->Unlock();

	ID3DXSprite * sprite;
	D3DXCreateSprite(device.Get(), &sprite);
	sprite_ = sprite;
}
MaptoolRenderer::GraphicsDevice::!GraphicsDevice()
{
	this->~GraphicsDevice();
}

MaptoolRenderer::GraphicsDevice::~GraphicsDevice()
{
	if (s_instance != nullptr)
	{
		msclr::lock r(GraphicsDevice::typeid);
		if (s_instance != nullptr)
		{
			s_instance = nullptr;
		}
	}
	if (renderModulePtr_ != nullptr)
	{
		delete renderModulePtr_;
		renderModulePtr_ = nullptr;
	}
	if (effects_ != nullptr)
	{
		for each (auto item in effects_)
		{
			auto effect{ (ID3DXEffect*)item.Value };
			effect->Release();
		}
		effects_ = nullptr;
	}
	if (indexBuffer_ != nullptr)
	{
		indexBuffer_->Release();
		indexBuffer_ = nullptr;
	}
	if (vertexBuffer_ != nullptr)
	{
		vertexBuffer_->Release();
		vertexBuffer_ = nullptr;
	}
	if (lightingEffect_ != nullptr)
	{
		lightingEffect_->Release();
		lightingEffect_ = nullptr;
	}
	if (combineEffect_ != nullptr)
	{
		combineEffect_->Release();
		combineEffect_ = nullptr;
	}
	if (sprite_ != nullptr)
	{
		sprite_->Release();
		sprite_ = nullptr;
	}
}

auto MaptoolRenderer::GraphicsDevice::Initalize(Control^ control, int width, int height) -> void
{
	if (s_instance == nullptr)
	{
		msclr::lock r(GraphicsDevice::typeid);
		if (s_instance == nullptr)
		{
			s_instance = gcnew GraphicsDevice(control, width, height);
		}
	}
}

auto MaptoolRenderer::GraphicsDevice::Instance::get() -> GraphicsDevice^
{
	return s_instance;
}
auto MaptoolRenderer::GraphicsDevice::RenderModuleHandle::get() -> RenderModule*
{
	return renderModulePtr_;
}
auto MaptoolRenderer::GraphicsDevice::Render(Control^ control, IEnumerable<IRenderable^>^ renderables, Camera^ camera) -> void
{
	DirectX::XMFLOAT4X4 viewSpaceMatrix{};
	DirectX::XMFLOAT4X4 projSpaceMatrix{};
	DirectX::XMFLOAT4X4 viewProjMatrix{};
	COMPtr<IDirect3DDevice9> device;
	renderModulePtr_->GetDevice(&device);
	HRESULT hr = device->TestCooperativeLevel();
	if (hr == D3DERR_DEVICENOTRESET)
	{
		device->SetRenderTarget(0, nullptr);
		device->SetRenderTarget(1, nullptr);
		device->SetRenderTarget(2, nullptr);
		device->SetRenderTarget(3, nullptr);
		if (effects_ != nullptr)
		{
			for each (auto item in effects_)
			{
				auto effect{ (ID3DXEffect*)item.Value };
				effect->OnLostDevice();
			}
		}
		sprite_->OnLostDevice();
		lightingEffect_->OnLostDevice();
		combineEffect_->OnLostDevice();
		//ClearEffect();
		Control^ defaultRenderWindow;
		this->control->TryGetTarget(defaultRenderWindow);
		if (renderModulePtr_->Reset())
		{
			sprite_->OnResetDevice();
			combineEffect_->OnResetDevice();
			lightingEffect_->OnResetDevice();
			if (effects_ != nullptr)
			{
				for each (auto item in effects_)
				{
					auto effect{ (ID3DXEffect*)item.Value };
					effect->OnResetDevice();
				}
			}
		}
		else
		{
			return;
		}
	}
	camera->GenerateProjMatrix(control->Width, control->Height, &projSpaceMatrix);
	camera->GenerateMatrix(&viewSpaceMatrix);
	GenerateViewProjMatrix(&viewSpaceMatrix, &projSpaceMatrix, &viewProjMatrix);
	device->SetTransform(D3DTS_PROJECTION, reinterpret_cast<D3DMATRIX*>(&projSpaceMatrix));
	device->SetTransform(D3DTS_VIEW, reinterpret_cast<D3DMATRIX*>(&viewSpaceMatrix));
	device->SetRenderState(D3DRS_LIGHTING, FALSE);

	hr = device->BeginScene();
	
	
	device->Clear(0, nullptr, D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, D3DCOLOR_COLORVALUE(0.f, 0.f, 1.f, 1.f), 1.f, 0);
	for each (auto item in effects_)
	{
		ID3DXEffect* effect{ reinterpret_cast<ID3DXEffect*>(item.Value) };
		effect->SetMatrix("g_mView", reinterpret_cast<D3DXMATRIX*>(&viewSpaceMatrix));
		effect->SetMatrix("g_mProj", reinterpret_cast<D3DXMATRIX*>(&projSpaceMatrix));
		effect->SetMatrix("g_mViewProj", reinterpret_cast<D3DXMATRIX*>(&viewProjMatrix));
	}
	for each (IRenderable ^ renderable in renderables)
	{
		renderable->PrepereRender(this);
	}
	RenderDeferred(&viewSpaceMatrix, &projSpaceMatrix, &viewProjMatrix);
	RenderWireframe(&viewSpaceMatrix, &projSpaceMatrix, &viewProjMatrix);
	device->SetRenderState(D3DRS_LIGHTING, TRUE);
	DrawDebug();
	device->EndScene();

	device->Present(nullptr, nullptr, (HWND)control->Handle.ToPointer(), nullptr);
	this->renderGroups_.Clear();
}

auto MaptoolRenderer::GraphicsDevice::AddRenderObject(RenderGroupID group, IRenderEntity^ entity) -> void
{
	if (!this->renderGroups_.ContainsKey(group))
	{
		this->renderGroups_.Add(group, gcnew List<IRenderEntity^>{});
	}
	IList<IRenderEntity^>^ list{ this->renderGroups_[group] };
	list->Add(entity);
}

auto MaptoolRenderer::GraphicsDevice::GetEffect(String^ key, ID3DXEffect** out) -> void
{
	size_t value{};
	if (effects_->TryGetValue(key, value))
	{
		auto ptr{ (ID3DXEffect*)value };
		*out = ptr;
		ptr->AddRef();
	}
}
auto MaptoolRenderer::GraphicsDevice::InitalizeEffect()->void
{
	COMPtr<IDirect3DDevice9> device;
	COMPtr<ID3DXBuffer> msgBuffer;
	ID3DXEffect* effect;
	renderModulePtr_->GetDevice(&device);
	Resources::ResourceManager^ rm =
		gcnew Resources::ResourceManager(L"MaptoolRenderer.shader", this->GetType()->Assembly);
	array<char>^ arr = nullptr;
	arr = safe_cast<array<char>^> (rm->GetObject("deferred"));
	pin_ptr<char> pin = nullptr;
	pin = &arr[0];
	D3DXCreateEffect(device.Get(), pin, static_cast<UINT>(arr->Length), nullptr, nullptr, D3DXSHADER_OPTIMIZATION_LEVEL3, nullptr, &effect, &msgBuffer);
	effects_->Add("deferred", (size_t)effect);
	arr = safe_cast<array<char>^> (rm->GetObject("combine"));
	pin = &arr[0];
	D3DXCreateEffect(device.Get(), pin, static_cast<UINT>(arr->Length), nullptr, nullptr, D3DXSHADER_OPTIMIZATION_LEVEL3, nullptr, &effect, &msgBuffer);
	combineEffect_ = effect;

	arr = safe_cast<array<char>^> (rm->GetObject("deferred_lighting"));
	pin = &arr[0];
	D3DXCreateEffect(device.Get(), pin, static_cast<UINT>(arr->Length), nullptr, nullptr, D3DXSHADER_OPTIMIZATION_LEVEL3, nullptr, &effect, &msgBuffer);
	lightingEffect_ = effect;
	if (msgBuffer != nullptr)
	{
		char const* errMsg{ reinterpret_cast<char const*>(msgBuffer->GetBufferPointer()) };
		OutputDebugStringA(errMsg);
	}
}
auto MaptoolRenderer::GraphicsDevice::ClearEffect() -> void
{
	for each (auto item in effects_)
	{
		auto effect{ (ID3DXEffect*)item.Value };
		effect->Release();
	}
	effects_->Clear();
}
auto MaptoolRenderer::GraphicsDevice::ClearRenderTarget() -> void
{
	COMPtr<IDirect3DDevice9> pDevice{};

	COMPtr<IRenderTarget> albedo;
	COMPtr<IRenderTarget> normal;
	COMPtr<IRenderTarget> specular;
	COMPtr<IRenderTarget> depth;
	COMPtr<IRenderTarget> lightSpecular;
	COMPtr<IRenderTarget> lightDiffuse;

	COMPtr<IDirect3DSurface9> albedoSurface;
	COMPtr<IDirect3DSurface9> normalSurface;
	COMPtr<IDirect3DSurface9> specularSurface;
	COMPtr<IDirect3DSurface9> depthSurface;
	COMPtr<IDirect3DSurface9> lightSpecularSurface;
	COMPtr<IDirect3DSurface9> lightDiffuseSurface;
	renderModulePtr_->GetDevice(&pDevice);

	renderModulePtr_->GetRenderTarget(RENDER_TARGET_ALBEDO, &albedo);
	renderModulePtr_->GetRenderTarget(RENDER_TARGET_NORMAL, &normal);
	renderModulePtr_->GetRenderTarget(RENDER_TARGET_SPECULAR, &specular);
	renderModulePtr_->GetRenderTarget(RENDER_TARGET_DEPTH, &depth);
	renderModulePtr_->GetRenderTarget(LIGHT_SPECULAR, &lightSpecular);
	renderModulePtr_->GetRenderTarget(LIGHT_DIFFUSE, &lightDiffuse);

	albedo->GetSurface(&albedoSurface);
	normal->GetSurface(&normalSurface);
	specular->GetSurface(&specularSurface);
	depth->GetSurface(&depthSurface);

	pDevice->SetRenderTarget(0, albedoSurface.Get());
	pDevice->SetRenderTarget(1, normalSurface.Get());
	pDevice->SetRenderTarget(2, specularSurface.Get());
	pDevice->SetRenderTarget(3, depthSurface.Get());
	pDevice->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_COLORVALUE(0.f, 0.f, 0.f, 0.f), 1.f, 0);
}
auto MaptoolRenderer::GraphicsDevice::RenderDeferred(DirectX::XMFLOAT4X4 const* viewSpacePtr, DirectX::XMFLOAT4X4 const* projSpacePtr, DirectX::XMFLOAT4X4 const* viewProj) -> void
{
	ClearRenderTarget();
	IList<IRenderEntity^>^ list;
	D3DXVECTOR4 defaultSpecular{ 0.0f,0.0f,0.0f,1.f };
	for each (auto item in effects_)
	{
		ID3DXEffect* effect{ reinterpret_cast<ID3DXEffect*>(item.Value) };
		effect->SetVector("g_vSpecular", &defaultSpecular);
	}
	if (renderGroups_.TryGetValue(RenderGroupID::Deferred, list))
	{
		for each (auto entity in list)
		{
			entity->Render(this);
			for each (auto item in effects_)
			{
				ID3DXEffect* effect{ reinterpret_cast<ID3DXEffect*>(item.Value) };
				effect->SetVector("g_vSpecular", &defaultSpecular);
			}
		}
	}
	DeferredLighting(viewSpacePtr, projSpacePtr, viewProj);
	DeferredCombine();
}
auto MaptoolRenderer::GraphicsDevice::DeferredLighting(DirectX::XMFLOAT4X4 const* viewSpacePtr, DirectX::XMFLOAT4X4 const* projSpacePtr, DirectX::XMFLOAT4X4 const* viewProj)-> void
{
	HRESULT hr{};

	COMPtr<IDirect3DDevice9> pDevice;
	COMPtr<IDirect3DSurface9> lightDiffuseTarget;
	COMPtr<IDirect3DSurface9> lightSpecularTarget;
	COMPtr<IDirect3DTexture9> depthMapTexture{};
	COMPtr<IDirect3DTexture9> normalMapTexture{};
	COMPtr<IDirect3DTexture9> specularMapTexture{};

	COMPtr<IRenderTarget> renderTarget;

	renderModulePtr_->GetRenderTarget(LIGHT_DIFFUSE, &renderTarget);
	renderTarget->GetSurface(&lightDiffuseTarget);

	renderModulePtr_->GetRenderTarget(LIGHT_SPECULAR, &renderTarget);
	renderTarget->GetSurface(&lightSpecularTarget);

	renderModulePtr_->GetRenderTarget(RENDER_TARGET_NORMAL, &renderTarget);
	static_cast<TextureRenderTarget*>(renderTarget.Get())->GetTexture(&normalMapTexture);

	renderModulePtr_->GetRenderTarget(RENDER_TARGET_SPECULAR, &renderTarget);
	static_cast<TextureRenderTarget*>(renderTarget.Get())->GetTexture(&specularMapTexture);

	renderModulePtr_->GetRenderTarget(RENDER_TARGET_DEPTH, &renderTarget);
	static_cast<TextureRenderTarget*>(renderTarget.Get())->GetTexture(&depthMapTexture);

	renderModulePtr_->GetDevice(&pDevice);

	XMFLOAT4 cameraPosition{  };
	GenerateCameraPosition(viewSpacePtr, &cameraPosition);
	pDevice->SetRenderTarget(0, lightDiffuseTarget.Get());
	pDevice->SetRenderTarget(1, lightSpecularTarget.Get());
	pDevice->SetRenderTarget(2, nullptr);
	pDevice->SetRenderTarget(3, nullptr);
	pDevice->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_COLORVALUE(0.f, 0.f, 0.f, 0.f), 1.f, 0);
	pDevice->SetFVF(FVF);
	pDevice->SetIndices(indexBuffer_);
	pDevice->SetStreamSource(0, vertexBuffer_, 0, sizeof(PPVertexFVF));

	D3DXMATRIX InverseViewProMatrix{};
	InverseMatrix(
		viewProj,
		reinterpret_cast<XMFLOAT4X4*>(&InverseViewProMatrix)
	);
	lightingEffect_->SetMatrix(ID_CONST_INVSERSE_VIEW_PORJ_MATRIX, &InverseViewProMatrix);
	lightingEffect_->SetTexture(ID_TEX_NORMAL_MAP, normalMapTexture.Get());
	lightingEffect_->SetTexture(ID_TEX_SPECULAR_MAP, specularMapTexture.Get());
	lightingEffect_->SetTexture(ID_TEX_DEPTH_MAP, depthMapTexture.Get());
	lightingEffect_->SetVector("g_vCameraPosition", reinterpret_cast<D3DXVECTOR4 const*>(&cameraPosition));
	UINT passCount{};
	lightingEffect_->Begin(&passCount, 0);

	D3DLIGHT9 light{};
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Direction = D3DVECTOR{ 0.f, 0.0f,1.f };
	light.Ambient = D3DCOLORVALUE{ 0.2f, 0.2f, 0.2f, 1.0f};
	light.Diffuse = D3DCOLORVALUE{ 0.7f, 0.7f, 0.7f, 1.0f };
	D3DXVec3Normalize(
		reinterpret_cast<D3DXVECTOR3*>(&light.Direction),
		reinterpret_cast<D3DXVECTOR3*>(&light.Direction));
	//for (auto& light : lights_)
	//{
	int passNum{ -1 };
	switch (light.Type)
	{
	case D3DLIGHT_DIRECTIONAL:
		passNum = 0;
		{
			D3DXVECTOR4 vLightDir{ light.Direction.x,light.Direction.y ,light.Direction.z , 1.f };
			lightingEffect_->SetVector("g_vLightDirectionAndPower", reinterpret_cast<D3DXVECTOR4 const*>(&vLightDir));
			lightingEffect_->SetVector("g_vLightAmbient", reinterpret_cast<D3DXVECTOR4 const*>(&light.Ambient));
			lightingEffect_->SetVector("g_vLightDiffuse", reinterpret_cast<D3DXVECTOR4 const*>(&light.Diffuse));
		}
		break;
	}
	lightingEffect_->CommitChanges();
	lightingEffect_->BeginPass(passNum);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
	lightingEffect_->EndPass();
	//}
	lightingEffect_->End();
	//lights_.clear();
}
auto MaptoolRenderer::GraphicsDevice::DeferredCombine() -> void
{
	COMPtr<IDirect3DSwapChain9> pSwapChain{};
	COMPtr<IDirect3DSurface9> pBackbuffer{};
	COMPtr<IDirect3DDevice9> pDevice;
	COMPtr<IDirect3DTexture9> pLightSpecularMapTexture;
	COMPtr<IDirect3DTexture9> pLightDiffuseMapTexture;
	COMPtr<IDirect3DTexture9> pAlbedoMapTexture;
	COMPtr<IDirect3DTexture9> pDepthMapTexture;
	COMPtr<IRenderTarget> textureRenderTarget;
	renderModulePtr_->GetDefaultSwapChain(&pSwapChain);
	renderModulePtr_->GetDevice(&pDevice);
	pSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackbuffer);

	renderModulePtr_->GetRenderTarget(RENDER_TARGET_ALBEDO, &textureRenderTarget);
	static_cast<TextureRenderTarget*>(textureRenderTarget.Get())->GetTexture(&pAlbedoMapTexture);

	renderModulePtr_->GetRenderTarget(LIGHT_SPECULAR, &textureRenderTarget);
	static_cast<TextureRenderTarget*>(textureRenderTarget.Get())->GetTexture(&pLightSpecularMapTexture);

	renderModulePtr_->GetRenderTarget(LIGHT_DIFFUSE, &textureRenderTarget);
	static_cast<TextureRenderTarget*>(textureRenderTarget.Get())->GetTexture(&pLightDiffuseMapTexture);

	renderModulePtr_->GetRenderTarget(RENDER_TARGET_DEPTH, &textureRenderTarget);
	static_cast<TextureRenderTarget*>(textureRenderTarget.Get())->GetTexture(&pDepthMapTexture);

	pDevice->SetRenderTarget(0, pBackbuffer.Get());
	pDevice->SetRenderTarget(1, nullptr);
	pDevice->SetRenderTarget(2, nullptr);
	pDevice->SetRenderTarget(3, nullptr);
	UINT passCount{};

	pDevice->SetFVF(FVF);
	pDevice->SetIndices(indexBuffer_);
	pDevice->SetStreamSource(0, vertexBuffer_, 0, sizeof(PPVertexFVF));

	combineEffect_->Begin(&passCount, 0);
	combineEffect_->SetTexture("g_shadeMap", pLightDiffuseMapTexture.Get());
	combineEffect_->SetTexture("g_specularMap", pLightSpecularMapTexture.Get());
	combineEffect_->SetTexture("g_albedoMap", pAlbedoMapTexture.Get());
	combineEffect_->SetTexture("g_depthMap", pDepthMapTexture.Get());
	combineEffect_->BeginPass(0);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
	combineEffect_->EndPass();
	combineEffect_->End();
}
auto MaptoolRenderer::GraphicsDevice::RenderWireframe(DirectX::XMFLOAT4X4 const* viewSpacePtr, DirectX::XMFLOAT4X4 const* projSpacePtr, DirectX::XMFLOAT4X4 const* viewProj) -> void
{
	IList<IRenderEntity^>^ list;
	D3DXVECTOR4 defaultSpecular{ 0.0f,0.0f,0.0f,1.f };
	for each (auto item in effects_)
	{
		ID3DXEffect* effect{ reinterpret_cast<ID3DXEffect*>(item.Value) };
		effect->SetVector("g_vSpecular", &defaultSpecular);
	}
	if (renderGroups_.TryGetValue(RenderGroupID::Wireframe, list))
	{
		for each (auto entity in list)
		{
			entity->Render(this);
			for each (auto item in effects_)
			{
				ID3DXEffect* effect{ reinterpret_cast<ID3DXEffect*>(item.Value) };
				effect->SetVector("g_vSpecular", &defaultSpecular);
			}
		}
	}
}
auto MaptoolRenderer::GraphicsDevice::DrawDebug() -> void
{
	D3DXMATRIX tmpMat;
	D3DXMATRIX tmpMat2;
	D3DXMatrixScaling(&tmpMat, .2f, .2f, .2f);
	D3DXMatrixTranslation(&tmpMat2, renderModulePtr_->GetWidth() * 0.2f, 0, 0);
	COMPtr<IRenderTarget> normalRenderTarget;
	COMPtr<IDirect3DTexture9> normalMapTexture;
	renderModulePtr_->GetRenderTarget(LIGHT_SPECULAR, &normalRenderTarget);
	static_cast<TextureRenderTarget*>(normalRenderTarget.Get())->GetTexture(&normalMapTexture);
	sprite_->Begin(D3DXSPRITE_ALPHABLEND);
	sprite_->SetTransform(&tmpMat);
	sprite_->Draw(normalMapTexture.Get(), nullptr, nullptr, nullptr, D3DCOLOR_COLORVALUE(1.f, 1.f, 1.f, 1.f));
	sprite_->End();
}
#pragma unmanaged
auto MaptoolRenderer::GenerateTransform(
	DirectX::XMFLOAT3* position,
	DirectX::XMFLOAT3* rotation,
	float scale,
	DirectX::XMFLOAT4X4* out)->void
{
	using namespace DirectX;
	auto vPosition{ DirectX::XMLoadFloat3(position) };
	auto vRotation{ DirectX::XMLoadFloat3(rotation) };
	vRotation *= (XM_2PI / 360.f);
	XMStoreFloat4x4(
		out,
		XMMatrixScaling(scale, scale, scale) * 
		XMMatrixRotationRollPitchYawFromVector(vRotation)*
		XMMatrixTranslationFromVector(vPosition)
	);
}
#pragma managed