#include "pch.h"
#include "RenderSystemImpl.hpp"
#include "Texture2DBuilder.hpp"
#include "GraphicsModule.hpp"
#include <mutex>
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

void Kumazuma::RenderSystemImpl::Render()
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
	RenderDeferred();
}

void Kumazuma::RenderSystemImpl::InitializeRenderState()
{
	ComPtr<ID3D11Device> device{};
	gmodule_->GetDevice(&device);
	auto depthDesc = CD3D11_DEPTH_STENCIL_DESC(
		true,
		D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_LESS,
		false,
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
	gmodule_->GetImmediateContext(&deviceContext);
	ComPtr<ID3D11DepthStencilView> depthView{};
	ID3D11RenderTargetView* rtvs[4]{};

	gmodule_->GetDefaultDepthBuffer()->GetView<ID3D11DepthStencilView>(&depthView);
	albedoGBuffer_->GetView(&rtvs[0]);
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

	deviceContext->RSSetState(rasterizerCullBackState_.Get());
	deviceContext->RSSetViewports(1, &viewport);
	deviceContext->OMSetBlendState(blendState_.Get(), blendFactor, 0xFFFFFFFF);
	deviceContext->OMSetDepthStencilState(depthState_.Get(), 0);
	deviceContext->OMSetRenderTargets(4, rtvs, depthView.Get());
	
	LinkedList<Material*, 32>& materialList{ materials_[NumOf(MaterialShadingClass::DeferredShading)] };
	for (auto* material : materialList)
	{
		material->Render(deviceContext.Get());
	}

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
