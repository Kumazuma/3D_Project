#include "pch.h"
#include "SwapChain.hpp"
#include "SwapChainTexture.hpp"
#include "GraphicsModuleImpl.hpp"
#include "Texture2DBuilder.hpp"
#include <wrl.h>
namespace Kumazuma
{
	using namespace Microsoft::WRL;
	class SwapChainImpl :public SwapChain
	{
		ComPtr<IDXGISwapChain> swapChain;
		std::unique_ptr<Texture2D> backbuffer;
		std::unique_ptr<Texture2D> depthBuffer;

	public:
		SwapChainImpl(HWND hWindow, GraphicsModule* gModule, Size2D<u32> const& bufferSize, DXGI_FORMAT bufferFormat, bool fullScreen)
		{
			DXGI_SWAP_CHAIN_DESC swapChainDesc{};
			swapChainDesc.BufferCount = 1;
			swapChainDesc.BufferDesc.Width = bufferSize.width;
			swapChainDesc.BufferDesc.Height = bufferSize.height;
			swapChainDesc.BufferDesc.Format = bufferFormat;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
			swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS;
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.OutputWindow = hWindow;
			swapChainDesc.Windowed = !fullScreen;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapChainDesc.Flags = 0;
			ComPtr<ID3D11Device> device;
			gModule->GetDevice(&device);
			IDXGIFactory* dxgiFactory{ static_cast<GraphicsModuleImpl*>(gModule)->GetDXGIFactory() };
			dxgiFactory->CreateSwapChain(device.Get(), &swapChainDesc, &swapChain);
			backbuffer.reset(new SwapChainTexture2D(device.Get(), swapChain.Get()));
			depthBuffer.reset(Texture2D::Create(device.Get(), Texture2D::Builder(DXGI_FORMAT_D32_FLOAT, bufferSize.width, bufferSize.height).DepthStancilView()));
		}
		virtual Texture2D* GetBackbuffer()
		{
			return backbuffer.get();
		}
		virtual Texture2D* GetDepthBuffer()
		{
			return depthBuffer.get();
		}
		virtual void Clear(ID3D11DeviceContext* deviceContext,const float(&rgba)[4], float depth)
		{
			deviceContext->ClearRenderTargetView(backbuffer->GetViewRef<ID3D11RenderTargetView>(), rgba);
			deviceContext->ClearDepthStencilView(depthBuffer->GetViewRef<ID3D11DepthStencilView>(), D3D11_CLEAR_DEPTH, depth, 0);
		}
		virtual void Present()
		{
			swapChain->Present(1, 0);
		}
	};
	SwapChain* SwapChain::Create(HWND hWindow, GraphicsModule* gModule, Size2D<u32> const& bufferSize, DXGI_FORMAT bufferFormat, bool fullScreen)
	{
		try
		{
			SwapChainImpl swapChain{ hWindow,gModule, bufferSize, bufferFormat, fullScreen };
			return new SwapChainImpl{ std::move(swapChain) };
		}
		catch (...)
		{

		}
		return nullptr;
	}

	SwapChain::~SwapChain()
	{

	}
}


