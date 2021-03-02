#include "pch.h"
#include "CachedTextureImpl.hpp"

Kumazuma::CachedTexture2DImpl::CachedTexture2DImpl(ID3D11Device* device, ComPtr<ID3D11Texture2D> ptr, std::wstring const& path):
    path{path}
{
	texture_ = std::move(ptr);
    D3D11_TEXTURE2D_DESC desc;
    texture_->GetDesc(&desc);
    InitializeViews(device, desc);
    size_ = Size2D<u32>{ desc.Width, desc.Height };
}
