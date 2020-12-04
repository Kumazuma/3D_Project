#include "pch.h"
#include "MapToolRender.h"
#include <RenderModule/RenderModule.h>
#include "ControlSwapChain.h"
#include<d3d9.h>
MapToolRender::SwapChain::SwapChain(GraphicsDevice^ device, System::Windows::Forms::Control^ control, unsigned int width, unsigned int height, PixelFormat pixelFormat, bool fullscreen)
{
    IDirect3DDevice9* pDevice;
	IDirect3DSwapChain9* pSwapChain{};
    device->Handle->GetDevice(&pDevice);
    D3DPRESENT_PARAMETERS d3dpp{};
    d3dpp.BackBufferWidth = width;
    d3dpp.BackBufferHeight = height;
    d3dpp.Windowed = !fullscreen;
    d3dpp.EnableAutoDepthStencil = true;
    d3dpp.hDeviceWindow = (HWND)control->Handle.ToPointer();
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	pDevice->CreateAdditionalSwapChain(&d3dpp, &pSwapChain);
	if (pSwapChain == nullptr)
	{
		gcnew System::Exception("Can not create swapchain");
	}
    pDevice->Release();
	m_pSwapChain = pSwapChain;
}

MapToolRender::SwapChain::!SwapChain()
{
	if (m_pSwapChain != nullptr)
	{
		m_pSwapChain->Release();
		m_pSwapChain = nullptr;
	}
}
