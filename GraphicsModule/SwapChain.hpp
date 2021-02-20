#pragma once
#include "common.hpp"
#include "typedef.hpp"
#include <dxgiformat.h>
#include <d3d11.h>
namespace Kumazuma
{
	class GraphicsModule;
	class Texture2D;
	class DLL_CLASS SwapChain
	{
	public:
		static SwapChain* Create(HWND hWindow, GraphicsModule* gModule, Size2D<u32> const& bufferSize, DXGI_FORMAT bufferFormat, bool fullScreen);
		virtual ~SwapChain();
		virtual Texture2D* GetBackbuffer() = 0;
		virtual Texture2D* GetDepthBuffer() = 0;
		virtual void Clear(ID3D11DeviceContext* deviceContext, const float(&rgba)[4], float depth) = 0;
		virtual void Present() = 0;
	};
}