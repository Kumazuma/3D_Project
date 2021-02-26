#pragma once
#include "GraphicsModule.hpp"
#include "RenderSystem.hpp"
#include <unordered_map>
#include <string>
#include <wrl.h>
#include <memory>
#include "SwapChainTexture.hpp"
#include "TextureManager.hpp"
using namespace Microsoft::WRL;
namespace Kumazuma
{
	class TextureManager;
	class GraphicsModuleImpl : public GraphicsModule
	{
	public:
		GraphicsModuleImpl();
		GraphicsModuleImpl(GraphicsModuleImpl&& rhs) noexcept;
		HRESULT GetDevice(ID3D11Device** out) ;
		HRESULT GetImmediateContext(ID3D11DeviceContext** out);
		HRESULT CreateCBuffer(wchar_t const* name, size_t bufferSize);
		HRESULT GetCBuffer(wchar_t const* name, ID3D11Buffer** out);
		HRESULT LoadPixelShader(wchar_t const* id, wchar_t const* path, char const* entry);
		HRESULT LoadPixelShaderFromBytes(wchar_t const* id, u8 const* ptr, u32 len);
		HRESULT GetPixelShader(wchar_t const* id, ID3D11PixelShader** out) ;
		HRESULT LoadComputeShader(wchar_t const* id, wchar_t const* path, char const* entry);
		HRESULT LoadComputeShaderFromBytes(wchar_t const* id, u8 const* ptr, u32 len);
		HRESULT GetComputeShader(wchar_t const* id, ID3D11ComputeShader** out);
		HRESULT GetVertexShaderAndInputLayout(MeshType meshType, ID3D11VertexShader** outVS, ID3D11InputLayout** outInputLayout);

		TextureManager& GetTextureManager();
		IDXGIFactory* GetDXGIFactory();
	private:
		ComPtr<ID3D11Device>		device_;
		ComPtr<ID3D11DeviceContext> deviceContext_;
		ComPtr<IDXGIFactory>		dxgiFactory_;
		std::unordered_map < GUIDEx, std::unordered_map<std::wstring, ComPtr<ID3D11DeviceChild> > >shaders_;
		std::unique_ptr<TextureManager>							textureManager_;
		std::unordered_map<std::wstring, ComPtr<ID3D11Buffer> > cbuffers_;
	};
}