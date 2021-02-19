#include "pch.h"
#pragma unmanaged
#include <GraphicsModule.hpp>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "GraphicsModuleDX11.lib")
#pragma managed
#include "Kumazuma.Graphics.DX11.h"


namespace Kumazuma::DX11
{
	bool GraphicsDevice::Initialize(Control^ defaultControl, unsigned backbufferWidth, unsigned backbufferHeight)
	{
		GraphicsModule::Create((HWND)defaultControl->Handle.ToPointer(), { backbufferWidth, backbufferHeight }, false);

		return false;
	}
}
