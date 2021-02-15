#include "pch.h"
#include "Texture2DBuilder.hpp"
namespace Kumazuma
{
	Texture2D::Builder::Builder(DXGI_FORMAT format, uint32_t width, uint32_t height) :
		D3D11_TEXTURE2D_DESC{}
	{
		this->ArraySize = 1;
		this->MipLevels = 1;
		this->Usage = D3D11_USAGE_DEFAULT;
		this->BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		this->Format = format;
		this->SampleDesc.Count = 1;
		this->SampleDesc.Quality = 0;
		this->Width = width;
		this->Height = height;
	}

	auto Texture2D::Builder::GenerateMips() -> Builder&
	{
		this->MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
		this->MipLevels = -1;
		return *this;
	}

	auto Texture2D::Builder::UnorderedResourceView() -> Builder&
	{
		this->BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
		return *this;
	}

	auto Texture2D::Builder::DepthStancilView() -> Builder&
	{
		this->BindFlags = D3D11_BIND_DEPTH_STENCIL;
		this->ArraySize = 1;
		this->MipLevels = 1;
		this->Usage = D3D11_USAGE_DEFAULT;
		return *this;
	}
	auto Texture2D::Builder::Build(ID3D11Device* device) -> Texture2D*
	{
		return Texture2D::Create(device, *this);
	}
}
