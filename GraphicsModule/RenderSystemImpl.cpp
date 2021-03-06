#include "pch.h"
#include "RenderSystemImpl.hpp"
#include "Texture2DBuilder.hpp"
#include "GraphicsModule.hpp"
#include "SwapChain.hpp"
#include "Mesh.hpp"
#include <mutex>
#include <d3dcompiler.h>
using namespace DirectX;
struct alignas(16) LumAvgCBuffer
{
	Size2D<u32> sourceTextureSize;
	Size2D<u32> sourceTextureRange;
	Size2D<u32> destTextureRange;
};
namespace CS
{
	struct LightInfo
	{
		int lightType;
		DirectX::XMFLOAT3 g_vLightDirection;
		DirectX::XMFLOAT4 g_vLightDiffuse;
		DirectX::XMFLOAT4 g_vLightAmbient;
	};
	struct GlobalInfo
	{
		DirectX::XMFLOAT4X4 g_mInverseViewProj;
		DirectX::XMFLOAT4	g_vCameraPosition;
		DirectX::XMUINT2	g_bufferSize;
		DirectX::XMUINT2	g_padding;
	};
}

Kumazuma::RenderSystemImpl::RenderSystemImpl(GraphicsModule* gmodule, SwapChain* swapChain)
{
	swapChain_ = swapChain;
	Size2D<u32> size{};
	ComPtr<ID3D11Device> device{};
	gmodule->GetDevice(&device);
	swapChain->GetBackbuffer()->GetSize(&size);
	Texture2D* texture{};
	texture =
		Texture2D::Builder(DXGI_FORMAT_R8G8B8A8_UNORM, size.width, size.height)
		.UnorderedResourceView()
		.Build(device.Get());
	albedoGBuffer_.reset(texture);
	
	texture =
		Texture2D::Builder(DXGI_FORMAT_R16G16B16A16_FLOAT, size.width, size.height)
		.UnorderedResourceView()
		.Build(device.Get());
	normalGBuffer_.reset(texture);

	texture =
		Texture2D::Builder(DXGI_FORMAT_R16G16B16A16_FLOAT, size.width, size.height)
		.UnorderedResourceView()
		.Build(device.Get());
	sharpnessGBuffer_.reset(texture);

	texture =
		Texture2D::Builder(DXGI_FORMAT_R32G32_FLOAT, size.width, size.height)
		.UnorderedResourceView()
		.Build(device.Get());
	depthGBuffer_.reset(texture);

	texture =
		Texture2D::Builder(DXGI_FORMAT_R16G16B16A16_FLOAT, size.width, size.height)
		.UnorderedResourceView()
		.Build(device.Get());
	lightAmbientMap_.reset(texture);

	texture =
		Texture2D::Builder(DXGI_FORMAT_R16G16B16A16_FLOAT, size.width, size.height)
		.UnorderedResourceView()
		.Build(device.Get());
	lightSpecularMap_.reset(texture);

	texture =
		Texture2D::Builder(DXGI_FORMAT_R16G16B16A16_FLOAT, size.width, size.height)
		.UnorderedResourceView()
		.Build(device.Get());
	hdrRenderResultTexture_.reset(texture);

	/*std::unique_ptr<Texture2D> luminousnessTexture;
	std::unique_ptr<Texture2D> luminousness2Texture;
	std::unique_ptr<Texture2D> cpuAccessLuminousnessTexture;*/
//광도를 구하기 위한 텍스처 세장을 만들자.
	texture =
		Texture2D::Builder(DXGI_FORMAT_R32G32B32A32_FLOAT, 729, 729)
		.UnorderedResourceView()
		.Build(device.Get());
	luminousnessTexture.reset(texture);

	texture =
		Texture2D::Builder(DXGI_FORMAT_R32G32B32A32_FLOAT, 729, 729)
		.UnorderedResourceView()
		.Build(device.Get());
	luminousness2Texture.reset(texture);
	auto cpuAccessTextureBuilder{ Texture2D::Builder(DXGI_FORMAT_R32G32B32A32_FLOAT, 1.f, 1.f) };
	cpuAccessTextureBuilder.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	cpuAccessTextureBuilder.Usage = D3D11_USAGE_STAGING;
	cpuAccessTextureBuilder.BindFlags = 0;
	texture = cpuAccessTextureBuilder.Build(device.Get());
	cpuAccessLuminousnessTexture.reset(texture);

	InitializeRenderState(device.Get());

	HRESULT hr{};
	ComPtr<ID3DBlob> bytesCode;
	ComPtr<ID3DBlob> errMsg;
	hr = D3DCompileFromFile(L"./StaticMeshVS.hlsl", nullptr, nullptr, "main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, &bytesCode, &errMsg);
	if (errMsg != nullptr)
	{
		OutputDebugStringA((char* const)errMsg->GetBufferPointer());
	}

	std::array< D3D11_INPUT_ELEMENT_DESC, 3> polygonLayout{};
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;//float 3개가 들어간다는 약속
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;
	hr = device->CreateInputLayout(
		polygonLayout.data(),
		static_cast<UINT>(polygonLayout.size()),
		bytesCode->GetBufferPointer(),
		bytesCode->GetBufferSize(),
		&staticMeshInputLayout_);

	auto worldCBufferDesc = CD3D11_BUFFER_DESC(sizeof(DirectX::XMFLOAT4X4), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	auto localCBufferDesc = CD3D11_BUFFER_DESC(sizeof(DirectX::XMFLOAT4X4) * 2, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);

	hr = device->CreateBuffer(&worldCBufferDesc, nullptr, &vsGlobalCBuffer_);
	hr = device->CreateBuffer(&localCBufferDesc, nullptr, &vsLocalCBuffer_);
	device->CreateVertexShader(bytesCode->GetBufferPointer(),
		bytesCode->GetBufferSize(), nullptr, &staticMeshVertexShader_);

	gmodule->GetComputeShader(L"directional_lighting", &directinalLightingCShader_);

	auto cslightCBufferDesc = CD3D11_BUFFER_DESC(sizeof(CS::LightInfo), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	auto csGlobalCBufferDesc = CD3D11_BUFFER_DESC(sizeof(CS::GlobalInfo), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	auto csLumCBufferDesc = CD3D11_BUFFER_DESC(sizeof(LumAvgCBuffer), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);

	hr = device->CreateBuffer(&csGlobalCBufferDesc, nullptr, &csGlobalCBuffer_);
	hr = device->CreateBuffer(&cslightCBufferDesc, nullptr, &csLightCBuffer_);
	hr = device->CreateBuffer(&csLumCBufferDesc, nullptr, &csLumCBuffer_);
	gmodule->GetComputeShader(L"combineCShader_", &combineCShader_);
	gmodule->GetComputeShader(L"toneMappingCShader_", &toneMappingCShader_);
	gmodule->GetComputeShader(L"lumAvgCShader_", &lumAvgCShader_);
	ComPtr<IDXGIDevice> dxgiDevice;
	ComPtr<IDXGISurface> dxgiSurface;
	ComPtr<ID3D11Texture2D> backbuffer;
	ComPtr<ID3D11Texture2D> texture2D;
	device->QueryInterface(__uuidof(IDXGIDevice), &dxgiDevice);
	
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory1), &d2dFactory);
	d2dFactory->CreateDevice(dxgiDevice.Get(), &d2dDevice);
	d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2dDeviceContext);
	swapChain->GetBackbuffer()->GetResource(&backbuffer);
	backbuffer->QueryInterface(__uuidof(IDXGISurface), &dxgiSurface);
	
	D2D1_BITMAP_PROPERTIES1 bitmapProperties{
		D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(swapChain->GetBackbuffer()->GetFormat(), D2D1_ALPHA_MODE_IGNORE),
			96, 96
			)
	};
	d2dDeviceContext->CreateBitmapFromDxgiSurface(dxgiSurface.Get(), bitmapProperties, &d2dBackbuffer);
	d2dDeviceContext->SetTarget(d2dBackbuffer.Get());

	bitmapProperties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_NONE,
		D2D1::PixelFormat(albedoGBuffer_->GetFormat(), D2D1_ALPHA_MODE_IGNORE));
	albedoGBuffer_->GetResource(&texture2D);
	texture2D->QueryInterface(__uuidof(IDXGISurface), &dxgiSurface);
	d2dDeviceContext->CreateBitmapFromDxgiSurface(dxgiSurface.Get(), bitmapProperties, &d2dAlphado);
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &dwFactory);

	dwFactory->CreateTextFormat(L"굴림", nullptr, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 18.f, L"ko-kr", &dwTextFormat);

	hr = d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(0.f, 0.f, 0.f), &d2dBrush);

}

void Kumazuma::RenderSystemImpl::Initialize(GraphicsModule* gmodule)
{
	gmodule->LoadComputeShader(L"directional_lighting", L"./LightingCS.hlsl", "main");
	gmodule->LoadComputeShader(L"combineCShader_", L"./DeferredCombineCS.hlsl", "main");
	gmodule->LoadComputeShader(L"toneMappingCShader_", L"./HDRToneMappingCS.hlsl", "main");
	gmodule->LoadComputeShader(L"lumAvgCShader_", L"./LumAvgCS.hlsl", "main");
	gmodule->LoadPixelShader(L"deferred_gbuffer_ps", L"./StaticMeshGBufferPS.hlsl", "main");
}

void Kumazuma::RenderSystemImpl::AddMaterial(Material* material)
{
	MaterialShadingClass const shadingClass{ material->GetShadingClass() };
	LinkedList<Material*, 32>& materialList{ materials_[NumOf(shadingClass)] };
	auto it = std::find(materialList.begin(), materialList.end(), material);
	if (it != materialList.end())
	{
		return;
	}
	materialList.push_back(material);
}

void Kumazuma::RenderSystemImpl::RemoveMaterial(Material* material)
{
	MaterialShadingClass const shadingClass{ material->GetShadingClass() };
	LinkedList<Material*, 32>& materialList{ materials_[NumOf(shadingClass)] };
	auto it = std::find(materialList.begin(), materialList.end(), material);
	if (it != materialList.end())
	{
		return;
	}
	materialList.erase(it);
}

void Kumazuma::RenderSystemImpl::Render(GraphicsModule* gmodule, DirectX::XMFLOAT4X4 const* view, DirectX::XMFLOAT4X4 const* proj)
{
	
	std::lock_guard<GraphicsModule> guard{ *gmodule };
	ComPtr<ID3D11DeviceContext> deviceContext;
	ComPtr<ID3D11Device> device;
	Size2D<u32> bufferSize{};
	gmodule->GetDevice(&device);
	gmodule->GetImmediateContext(&deviceContext);

	swapChain_->Clear(deviceContext.Get(), { 0.f, 0.f, 1.f, 1.f }, 1.f);
	swapChain_->GetBackbuffer()->GetSize(&bufferSize);
	viewSpaceMatrix_ = *view;
	projSpaceMatrix_ = *proj;
	XMMATRIX mView{ XMLoadFloat4x4(view) };
	XMMATRIX mViewProjection{ mView * XMLoadFloat4x4(proj) };
	XMStoreFloat4x4(&viewProjMatrix_, mViewProjection);
	XMMATRIX mInverseViewProj{ XMMatrixInverse(nullptr, mViewProjection) };
	XMVECTOR vCameraPosition = XMMatrixInverse(nullptr, mView).r[3];

	XMFLOAT4X4* mapped;
	D3D11_MAPPED_SUBRESOURCE mappedResource{};
	deviceContext->Map(vsGlobalCBuffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	mapped = reinterpret_cast<DirectX::XMFLOAT4X4*>(mappedResource.pData);
	XMStoreFloat4x4(mapped, XMMatrixTranspose(XMLoadFloat4x4(&viewProjMatrix_)));
	deviceContext->Unmap(vsGlobalCBuffer_.Get(), 0);

	deviceContext->Map(csGlobalCBuffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	CS::GlobalInfo* csGlobalCBuffer = reinterpret_cast<CS::GlobalInfo*>(mappedResource.pData);
	csGlobalCBuffer->g_bufferSize = XMUINT2{ bufferSize.width, bufferSize.height };
	XMStoreFloat4x4(&csGlobalCBuffer->g_mInverseViewProj, mInverseViewProj);
	XMStoreFloat4(&csGlobalCBuffer->g_vCameraPosition, vCameraPosition);
	deviceContext->Unmap(csGlobalCBuffer_.Get(), 0);

	RenderDeferred(device.Get(), deviceContext.Get());
	RenderForward(device.Get(), deviceContext.Get());
	ToneMapping(device.Get(), deviceContext.Get());
	deviceContext->ClearState();
	d2dDeviceContext->BeginDraw();
	
	//d2dDeviceContext->DrawBitmap(d2dAlphado.Get(), D2D1::RectF(0.f, 0.f, 200.f, 200.f));
	std::wstring helloWorld{ L"Hello, World" };
	
	d2dDeviceContext->DrawTextW(helloWorld.c_str(), helloWorld.size(), dwTextFormat.Get(), D2D1::RectF(0.f, 0.f, 200.f, 200.f), d2dBrush.Get());
	d2dDeviceContext->EndDraw();
}

void Kumazuma::RenderSystemImpl::SettupVertexShader(MeshType type, ID3D11DeviceContext* context, DirectX::XMFLOAT4X4* worldSpace)
{
	DirectX::XMFLOAT4X4* mapped;
	D3D11_MAPPED_SUBRESOURCE mappedResource{};
	ID3D11Buffer* vsGlobalCBuffer{ vsGlobalCBuffer_.Get() };
	ID3D11Buffer* vsLocalCBuffer{ vsLocalCBuffer_.Get() };
	DirectX::XMMATRIX mWorld{ XMLoadFloat4x4(worldSpace) };
	DirectX::XMMATRIX mNormalWorld{ mWorld };
	mNormalWorld.r[3] = { 0.f, 0.f, 0.f, 1.f };
	mNormalWorld = XMMatrixInverse(nullptr, mNormalWorld);

	switch (type)
	{
	case MeshType::Static:
		context->IASetInputLayout(staticMeshInputLayout_.Get());
		context->VSSetShader(staticMeshVertexShader_.Get(), nullptr, 0);
		context->Map(vsLocalCBuffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		mapped = reinterpret_cast<DirectX::XMFLOAT4X4*>(mappedResource.pData);
		XMStoreFloat4x4(mapped, XMMatrixTranspose(mWorld));
		XMStoreFloat4x4(mapped + 1, mNormalWorld);
		context->Unmap(vsLocalCBuffer_.Get(), 0);
		context->VSSetConstantBuffers(0, 1, &vsGlobalCBuffer);
		context->VSSetConstantBuffers(1, 1, &vsLocalCBuffer);
		break;
	}
}

void Kumazuma::RenderSystemImpl::SetGraphicsModule(GraphicsModule* module)
{
}

void Kumazuma::RenderSystemImpl::InitializeRenderState(ID3D11Device* device)
{
	auto depthDesc = CD3D11_DEPTH_STENCIL_DESC(
		true,
		D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_LESS,
		true,
		0xFF,
		0xFF,
		D3D11_STENCIL_OP_KEEP,
		D3D11_STENCIL_OP_INCR,
		D3D11_STENCIL_OP_KEEP,
		D3D11_COMPARISON_ALWAYS,
		D3D11_STENCIL_OP_KEEP,
		D3D11_STENCIL_OP_DECR,
		D3D11_STENCIL_OP_KEEP,
		D3D11_COMPARISON_ALWAYS
		);
	device->CreateDepthStencilState(&depthDesc, &depthState_);

	D3D11_BLEND_DESC blendStateDesc{};
	blendStateDesc.RenderTarget[0].BlendEnable = false;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	device->CreateBlendState(&blendStateDesc, &blendState_);

	D3D11_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 0.f;

	device->CreateRasterizerState(&rasterizerDesc, &rasterizerCullBackState_);

	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	device->CreateRasterizerState(&rasterizerDesc, &rasterizerCullNoneState_);

	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDesc, &lumSamplerState_);
}

void Kumazuma::RenderSystemImpl::RenderShadowMap(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{

}

void Kumazuma::RenderSystemImpl::RenderDeferred(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	f32 rgba[4]{};
	ComPtr<ID3D11DeviceContext> deferredDeviceContext;
	deviceContext->ClearRenderTargetView(lightAmbientMap_->GetViewRef<ID3D11RenderTargetView>(), rgba);
	deviceContext->ClearRenderTargetView(lightSpecularMap_->GetViewRef<ID3D11RenderTargetView>(), rgba);
	deviceContext->ClearRenderTargetView(albedoGBuffer_->GetViewRef<ID3D11RenderTargetView>(), rgba);
	deviceContext->ClearRenderTargetView(normalGBuffer_->GetViewRef<ID3D11RenderTargetView>(), rgba);
	deviceContext->ClearRenderTargetView(sharpnessGBuffer_->GetViewRef<ID3D11RenderTargetView>(), rgba);
	deviceContext->ClearRenderTargetView(depthGBuffer_->GetViewRef<ID3D11RenderTargetView>(), rgba);

	 

	device->CreateDeferredContext(0, &deferredDeviceContext);

	ComPtr<ID3D11DepthStencilView> depthView{};
	ID3D11RenderTargetView* rtvs[4]{};

	swapChain_->GetDepthBuffer()->GetView<ID3D11DepthStencilView>(&depthView);
	albedoGBuffer_->GetView(&rtvs[0]);
	//gmodule_->GetSwapChainTexture()->GetView(&rtvs[0]);
	normalGBuffer_->GetView(&rtvs[1]);
	sharpnessGBuffer_->GetView(&rtvs[2]);
	depthGBuffer_->GetView(&rtvs[3]);
	f32 blendFactor[4]{ 0.f, 0.f, 0.f, 0.f };

	Size2D<u32> size{};
	D3D11_VIEWPORT viewport{};
	swapChain_->GetBackbuffer()->GetSize(&size);
	viewport.Width = static_cast<f32>(size.width);
	viewport.Height = static_cast<f32>(size.height);
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	deferredDeviceContext->RSSetState(rasterizerCullBackState_.Get());
	deferredDeviceContext->RSSetViewports(1, &viewport);
	deferredDeviceContext->OMSetBlendState(blendState_.Get(), blendFactor, 0xFFFFFFFF);
	deferredDeviceContext->OMSetDepthStencilState(depthState_.Get(), 1);
	deferredDeviceContext->OMSetRenderTargets(4, rtvs, depthView.Get());
	
	LinkedList<Material*, 32>& materialList{ materials_[NumOf(MaterialShadingClass::DeferredShading)] };
	for (auto* material : materialList)
	{
		material->Render(this, deferredDeviceContext.Get());
	}
	ComPtr<ID3D11CommandList> commandList;
	deferredDeviceContext->FinishCommandList(false, &commandList);
	deviceContext->ExecuteCommandList(commandList.Get(), true);
	for (auto* rtv : rtvs)
	{
		rtv->Release();
	}
	DeferredLighting(device, deviceContext);
	DeferredCombine(device, deviceContext);
}

void Kumazuma::RenderSystemImpl::DeferredLighting(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	deviceContext->ClearState();
	ComPtr<ID3D11DeviceContext> deferredDeviceContext;
	Size2D<u32> size{};
	swapChain_->GetBackbuffer()->GetSize(&size);

	XMFLOAT3 lightDirection{ 1.f, -1.f, 1.f };
	XMStoreFloat3(&lightDirection, XMVector3Normalize(XMLoadFloat3(&lightDirection)));
	D3D11_MAPPED_SUBRESOURCE mappedResource{};
	deviceContext->Map(csLightCBuffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	CS::LightInfo* csLightCBuffer = reinterpret_cast<CS::LightInfo*>(mappedResource.pData);
	csLightCBuffer->g_vLightAmbient = XMFLOAT4{ 0.1f, 0.1f, 0.1f,0.1f};
	csLightCBuffer->g_vLightDiffuse = XMFLOAT4{ 10.f, 10.f, 10.f,1.f };
	csLightCBuffer->g_vLightDirection = lightDirection;
	csLightCBuffer->lightType = 0;
	deviceContext->Unmap(csLightCBuffer_.Get(), 0);

	ID3D11Buffer* csCBuffer0{ csLightCBuffer_.Get() };
	ID3D11Buffer* csCBuffer1{ csGlobalCBuffer_.Get() };

	ID3D11UnorderedAccessView* normalMapUAV;
	ID3D11UnorderedAccessView* depthMapUAV;
	ID3D11UnorderedAccessView* sharpnessMapUAV;
	ID3D11UnorderedAccessView* lightAmbientMapUAV;
	ID3D11UnorderedAccessView* lightSpecularMapUAV;

	this->normalGBuffer_->GetView(&normalMapUAV);
	this->depthGBuffer_->GetView(&depthMapUAV);
	this->sharpnessGBuffer_->GetView(&sharpnessMapUAV);
	this->lightAmbientMap_->GetView(&lightAmbientMapUAV);
	this->lightSpecularMap_->GetView(&lightSpecularMapUAV);

	deviceContext->CSSetConstantBuffers(0, 1, &csCBuffer0);
	deviceContext->CSSetConstantBuffers(1, 1, &csCBuffer1);

	deviceContext->CSSetUnorderedAccessViews(0, 1, &normalMapUAV, nullptr);
	deviceContext->CSSetUnorderedAccessViews(1, 1, &depthMapUAV, nullptr);
	deviceContext->CSSetUnorderedAccessViews(2, 1, &sharpnessMapUAV, nullptr);
	deviceContext->CSSetUnorderedAccessViews(3, 1, &lightAmbientMapUAV, nullptr);
	deviceContext->CSSetUnorderedAccessViews(4, 1, &lightSpecularMapUAV, nullptr);

	deviceContext->CSSetShader(directinalLightingCShader_.Get(), nullptr, 0);
	int x = size.width / 16 + static_cast<UINT>(size.width % 16 != 0);
	int y = size.height / 16 + static_cast<UINT>(size.height % 16 != 0);
	deviceContext->Dispatch(x, y, 1);

	normalMapUAV->Release();
	depthMapUAV->Release();
	sharpnessMapUAV->Release();
	lightAmbientMapUAV->Release();
	lightSpecularMapUAV->Release();
}

void Kumazuma::RenderSystemImpl::DeferredCombine(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	ID3D11UnorderedAccessView* albedoMapUAV;
	ID3D11UnorderedAccessView* lightAmbientMapUAV;
	ID3D11UnorderedAccessView* lightSpecularMapUAV;
	ID3D11UnorderedAccessView* hdrRenderResultUAV;
	Size2D<u32> size{};

	swapChain_->GetBackbuffer()->GetSize(&size);
	hdrRenderResultTexture_->GetView(&hdrRenderResultUAV);
	this->albedoGBuffer_->GetView(&albedoMapUAV);
	this->lightAmbientMap_->GetView(&lightAmbientMapUAV);
	this->lightSpecularMap_->GetView(&lightSpecularMapUAV);

	deviceContext->ClearState();
	deviceContext->CSSetShader(combineCShader_.Get(), nullptr, 0);
	deviceContext->CSSetUnorderedAccessViews(0, 1, &albedoMapUAV,nullptr );
	deviceContext->CSSetUnorderedAccessViews(1, 1, &lightAmbientMapUAV,nullptr );
	deviceContext->CSSetUnorderedAccessViews(2, 1, &lightSpecularMapUAV,nullptr );
	deviceContext->CSSetUnorderedAccessViews(3, 1, &hdrRenderResultUAV, nullptr);
	deviceContext->Dispatch(size.width, size.height, 1);


	hdrRenderResultUAV->Release();
	albedoMapUAV->Release();
	lightAmbientMapUAV->Release();
	lightSpecularMapUAV->Release();
}

void Kumazuma::RenderSystemImpl::ToneMapping(ID3D11Device* device, ID3D11DeviceContext* context)
{
	HRESULT hr{};
	ID3D11UnorderedAccessView* hdrRenderResultUAV;
	ID3D11UnorderedAccessView* swapChainUAV;
	Size2D<u32> size{};
	D3D11_MAPPED_SUBRESOURCE mappedResource{};
	Size2D<u32> destTextureSize{ 729 / 3, 729 / 3 };
	//
	ComPtr<ID3D11Texture2D> hdrRenderTexture2D;
	ComPtr<ID3D11Texture2D> sourceTexture2D;
	ComPtr<ID3D11Texture2D> destTexture2D;
	CS::GlobalInfo* csGlobalCBuffer{};
	LumAvgCBuffer* csLumAvgCBuffer{};
	swapChain_->GetBackbuffer()->GetSize(&size);
	hdrRenderResultTexture_->GetResource(&hdrRenderTexture2D);
	luminousnessTexture->GetResource(&sourceTexture2D);
	luminousness2Texture->GetResource(&destTexture2D);
	//우선 HDR렌더타겟의 내용을 원본으로 리사이징한다.
	context->Map(csLumCBuffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	csLumAvgCBuffer = reinterpret_cast<LumAvgCBuffer*>(mappedResource.pData);
	csLumAvgCBuffer->sourceTextureRange = size;
	csLumAvgCBuffer->sourceTextureSize = size;
	csLumAvgCBuffer->destTextureRange = { 729 , 729 };
	context->Unmap(csLumCBuffer_.Get(), 0);
	auto sampler = lumSamplerState_.Get();
	auto srv = hdrRenderResultTexture_->GetViewRef<ID3D11ShaderResourceView>();
	auto uav = luminousnessTexture->GetViewRef<ID3D11UnorderedAccessView>();
	context->ClearState();
	context->CSSetShader(lumAvgCShader_.Get(), nullptr, 0);
	context->CSSetUnorderedAccessViews(0, 1, &uav, nullptr);
	context->CSSetShaderResources(0, 1, &srv);
	context->CSSetConstantBuffers(0, 1, csLumCBuffer_.GetAddressOf());
	context->CSSetSamplers(0, 1, &sampler);
	context->Dispatch(729, 729, 1);

	Texture2D* sourceTexture = luminousnessTexture.get();
	Texture2D* destTexture = luminousness2Texture.get();
	while (
		destTextureSize.height >= 1 ||
		destTextureSize.width >= 1)
	{
		context->Map(csLumCBuffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		csLumAvgCBuffer = reinterpret_cast<LumAvgCBuffer*>(mappedResource.pData);
		csLumAvgCBuffer->sourceTextureRange = { destTextureSize.width * 3, destTextureSize.height * 3 };
		csLumAvgCBuffer->sourceTextureSize = { 729 , 729 };
		csLumAvgCBuffer->destTextureRange = destTextureSize;
		context->Unmap(csLumCBuffer_.Get(), 0);
		auto srv = sourceTexture->GetViewRef<ID3D11ShaderResourceView>();
		auto uav = destTexture->GetViewRef<ID3D11UnorderedAccessView>();
		//셰이더를 지정하고
		//
		auto sampler = lumSamplerState_.Get();
		context->ClearState();
		context->CSSetShader(lumAvgCShader_.Get(), nullptr, 0);
		context->CSSetUnorderedAccessViews(0, 1, &uav, nullptr);
		context->CSSetShaderResources(0, 1, &srv);
		context->CSSetConstantBuffers(0, 1, csLumCBuffer_.GetAddressOf());
		context->CSSetSamplers(0, 1, &sampler);
		
		//실행시킨 후에
		context->Dispatch(destTextureSize.width, destTextureSize.height, 1);
		//포인터만 바꿔 근원 텍스처와 목적지 텍스처의 역할을 바꾼다.
		std::swap(sourceTexture, destTexture);
		
		destTextureSize = { destTextureSize.width / 3, destTextureSize.height / 3 };
	}
	ComPtr<ID3D11Texture2D> cpuReadableTexture2D{};
	ComPtr<ID3D11Texture2D> lumTexture2D{};
	sourceTexture->GetResource(&lumTexture2D);
	this->cpuAccessLuminousnessTexture->GetResource(&cpuReadableTexture2D);
	D3D11_BOX box{};
	box.left = 0;
	box.right = 1;
	box.top = 0;
	box.bottom = 1;
	box.front = 0;
	box.back = 1;
	XMFLOAT4 YCbCr{};
	context->CopySubresourceRegion(cpuReadableTexture2D.Get(), 0, 0, 0, 0, lumTexture2D.Get(), 0, &box);
	context->Map(cpuReadableTexture2D.Get(), 0, D3D11_MAP_READ, 0, &mappedResource);
	YCbCr = *reinterpret_cast<XMFLOAT4*>(mappedResource.pData);
	context->Unmap(cpuReadableTexture2D.Get(), 0);

	context->Map(csGlobalCBuffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	csGlobalCBuffer = reinterpret_cast<CS::GlobalInfo*>(mappedResource.pData);
	csGlobalCBuffer->g_bufferSize = XMUINT2{ size.width, size.height };
	csGlobalCBuffer->g_mInverseViewProj(0, 0) = 1.0f;
	context->Unmap(csGlobalCBuffer_.Get(), 0);


	hdrRenderResultTexture_->GetView(&hdrRenderResultUAV);
	swapChain_->GetBackbuffer()->GetView(&swapChainUAV);
	context->ClearState();
	context->CSSetShader(toneMappingCShader_.Get(), nullptr, 0);
	context->CSSetConstantBuffers(0, 1, csGlobalCBuffer_.GetAddressOf());
	context->CSSetUnorderedAccessViews(0, 1, &hdrRenderResultUAV, nullptr);
	context->CSSetUnorderedAccessViews(1, 1, &swapChainUAV, nullptr);
	int x = size.width / 16 + static_cast<UINT>(size.width % 16 != 0);
	int y = size.height / 16 + static_cast<UINT>(size.height % 16 != 0);
	context->Dispatch(x, y, 1);
	hdrRenderResultUAV->Release();
	swapChainUAV->Release();
}

void Kumazuma::RenderSystemImpl::RenderForward(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{

}

void Kumazuma::RenderSystemImpl::RenderUI(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{

}
