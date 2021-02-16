#pragma once
#include<d3d11.h>
#include"common.hpp"
#include"typedef.hpp"
namespace Kumazuma
{
	class DLL_CLASS Texture2D
	{
	public:
		virtual ~Texture2D() {};
		class Builder;
		static Texture2D* Create(ID3D11Device* device, D3D11_TEXTURE2D_DESC const& desc);
		virtual HRESULT GetResource(ID3D11Texture2D** out) = 0;
		virtual HRESULT GetSize(Size2D<u32>* out) = 0;
		virtual HRESULT GetView(GUID, ID3D11View** out) = 0;
		
		virtual Texture2D* CreateGenerateMipmap(ID3D11Device* device, ID3D11DeviceContext* deviceContext) = 0;
		template<typename T>
		HRESULT GetView(T** out);
		template<typename T>
		T* GetViewRef();
	};
	template<typename T>
	inline HRESULT Texture2D::GetView(T** out) 
	{
		return GetView(__uuidof(T), reinterpret_cast<ID3D11View**>(out));
	}
	template<typename T>
	inline T* Texture2D::GetViewRef()
	{
		T* ptr{};
		GetView(__uuidof(T), reinterpret_cast<ID3D11View**>(&ptr));
		ptr->Release();
		return ptr;
	}
}