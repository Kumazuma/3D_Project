#pragma once
#include"Texture2DImpl.hpp"
using namespace Microsoft::WRL;
namespace Kumazuma
{
	class SwapChainTexture2D : public Texture2DImpl
	{
	public:
		SwapChainTexture2D(ID3D11Device* device, IDXGISwapChain* swapChain);
	};
}