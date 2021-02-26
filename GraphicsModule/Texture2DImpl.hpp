#pragma once
#include<unordered_map>
#include<wrl.h>
#include"Texture2D.hpp"
#include"guidhashder.hpp"

using namespace Microsoft::WRL;
namespace Kumazuma
{
	class Texture2DImpl : public Texture2D
	{
	protected:
		Texture2DImpl();
	public:
		Texture2DImpl(ID3D11Device* device, D3D11_TEXTURE2D_DESC const& desc);
	protected:
		void InitializeViews(ID3D11Device* device, D3D11_TEXTURE2D_DESC const& desc);
	public:
		HRESULT Delete() ;
		HRESULT GetResource(ID3D11Texture2D** out);
		HRESULT GetSize(Size2D<u32>* out);
		HRESULT GetView(GUID, ID3D11View** out);
		DXGI_FORMAT GetFormat() override;
		Texture2D* CreateGenerateMipmap(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	protected:
		ComPtr<ID3D11Texture2D> texture_;
		Size2D<u32> size_;
		DXGI_FORMAT format_;
		std::unordered_map<GUIDEx, ComPtr<ID3D11View> > views_;
	};
}