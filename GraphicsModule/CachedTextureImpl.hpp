#pragma once
#include"Texture2DImpl.hpp"
namespace Kumazuma
{
	class CachedTexture2DImpl : public Texture2DImpl
	{
	public:
		CachedTexture2DImpl(ID3D11Device* device, ComPtr<ID3D11Texture2D> ptr, std::wstring const& path);
	private:
		std::wstring path;
	};
}