#include "pch.h"
#include "RenderSystemImpl.hpp"
#include "Texture2DBuilder.hpp"
#include "GraphicsModule.hpp"
#include "Mesh.hpp"
#include <mutex>
#include <d3dcompiler.h>
using namespace DirectX;
Kumazuma::RenderSystemImpl::RenderSystemImpl(GraphicsModule* gmodule)
{
	gmodule_ = gmodule;
	Size2D<u32> size{};
	ComPtr<ID3D11Device> device{};
	gmodule->GetDevice(&device);
	gmodule->GetSwapChainTexture()->GetSize(&size);
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

	InitializeRenderState();

	HRESULT hr{};
	hr = gmodule_->LoadPixelShader(L"deferred_gbuffer_ps", L"./StaticMeshGBufferPS.hlsl", "main");
	ComPtr<ID3DBlob> bytesCode;
	ComPtr<ID3DBlob> errMsg;
	hr = D3DCompileFromFile(L"./StaticMeshVS.hlsl", nullptr, nullptr, "main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &bytesCode, &errMsg);
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

void Kumazuma::RenderSystemImpl::Render(DirectX::XMFLOAT4X4 const* view, DirectX::XMFLOAT4X4 const* proj)
{
	std::lock_guard<GraphicsModule> guard{ *gmodule_ };
	ComPtr<ID3D11RenderTargetView> swapChainRtv{};
	ComPtr<ID3D11DeviceContext> deviceContext{};
	ComPtr<ID3D11DepthStencilView> depthView{};
	gmodule_->GetSwapChainTexture()->GetView<ID3D11RenderTargetView>(&swapChainRtv);
	gmodule_->GetDefaultDepthBuffer()->GetView<ID3D11DepthStencilView>(&depthView);
	gmodule_->GetImmediateContext(&deviceContext);
	deviceContext->ClearRenderTargetView(swapChainRtv.Get(), std::array<f32, 4>{ }.data());
	deviceContext->ClearDepthStencilView(depthView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	using namespace DirectX;
	DirectX::XMFLOAT4X4 viewProj{};
	DirectX::XMStoreFloat4x4(&viewProj, XMLoadFloat4x4(view) * XMLoadFloat4x4(proj));
	DirectX::XMFLOAT4X4* mapped;
	D3D11_MAPPED_SUBRESOURCE mappedResource{};
	deviceContext->Map(vsGlobalCBuffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	mapped = reinterpret_cast<DirectX::XMFLOAT4X4*>(mappedResource.pData);
	XMStoreFloat4x4(mapped, XMMatrixTranspose(XMLoadFloat4x4(&viewProj)));

	deviceContext->Unmap(vsGlobalCBuffer_.Get(), 0);

	RenderDeferred();
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
	gmodule_ = module;
}

void Kumazuma::RenderSystemImpl::InitializeRenderState()
{
	ComPtr<ID3D11Device> device{};
	gmodule_->GetDevice(&device);
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

}

void Kumazuma::RenderSystemImpl::RenderShadowMap()
{

}

void Kumazuma::RenderSystemImpl::RenderDeferred()
{
	ComPtr<ID3D11DeviceContext> deviceContext{};
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deferredDeviceContext;
	gmodule_->GetImmediateContext(&deviceContext);
	gmodule_->GetDevice(&device);
	device->CreateDeferredContext(0, &deferredDeviceContext);

	ComPtr<ID3D11DepthStencilView> depthView{};
	ID3D11RenderTargetView* rtvs[4]{};

	gmodule_->GetDefaultDepthBuffer()->GetView<ID3D11DepthStencilView>(&depthView);
	//albedoGBuffer_->GetView(&rtvs[0]);
	gmodule_->GetSwapChainTexture()->GetView(&rtvs[0]);
	normalGBuffer_->GetView(&rtvs[1]);
	sharpnessGBuffer_->GetView(&rtvs[2]);
	depthGBuffer_->GetView(&rtvs[3]);
	f32 blendFactor[4]{ 0.f, 0.f, 0.f, 0.f };

	Size2D<u32> size{};
	D3D11_VIEWPORT viewport{};
	gmodule_->GetSwapChainTexture()->GetSize(&size);
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
		material->Render(deferredDeviceContext.Get());
	}
	ComPtr<ID3D11CommandList> commandList;
	deferredDeviceContext->FinishCommandList(false, &commandList);
	deviceContext->ExecuteCommandList(commandList.Get(), true);
	for (auto* rtv : rtvs)
	{
		rtv->Release();
	}
}

void Kumazuma::RenderSystemImpl::RenderForward()
{

}

void Kumazuma::RenderSystemImpl::RenderUI()
{

}
