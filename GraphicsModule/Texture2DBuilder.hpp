#pragma once
#include"Texture2D.hpp"
namespace Kumazuma
{
	class DLL_CLASS Texture2D::Builder :public D3D11_TEXTURE2D_DESC
	{
	public:
		Builder(DXGI_FORMAT format, uint32_t width, uint32_t height);
		auto GenerateMips()->Builder&;
		auto UnorderedResourceView()->Builder&;
		auto DepthStancilView()->Builder&;
		auto Build(ID3D11Device* device)->Texture2D*;
	};
}