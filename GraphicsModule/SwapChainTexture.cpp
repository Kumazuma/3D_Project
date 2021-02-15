#include "pch.h"
#include "SwapChainTexture.hpp"

Kumazuma::SwapChainTexture2D::SwapChainTexture2D(ID3D11Device* device, IDXGISwapChain* swapChain)
{
    HRESULT hr;
    hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &texture_);
    assert(SUCCEEDED(hr));
    if (FAILED(hr))
    {
        throw hr;
    }
    D3D11_TEXTURE2D_DESC desc;
    texture_->GetDesc(&desc);
    InitializeViews(device, desc);
    size_ = Size2D<u32>{ desc.Width, desc.Height };
}
