#pragma once
#include "GraphicsModule.hpp"
#include "RenderSystem.hpp"
#include <unordered_map>
#include <string>
#include <wrl.h>
#include "SwapChainTexture.hpp"
#include <memory>
#include "TextureManager.hpp"
using namespace Microsoft::WRL;
namespace Kumazuma
{
	class TextureManager;
	class GraphicsModuleImpl : public GraphicsModule
	{
	public:
		GraphicsModuleImpl(HWND hWindow, Size2D<u32> const& bufferSize, bool fullScreen);
		GraphicsModuleImpl(GraphicsModuleImpl&& rhs) noexcept;
		HRESULT GetDevice(ID3D11Device** out) ;
		HRESULT GetImmediateContext(ID3D11DeviceContext** out);
		HRESULT GetSwapChain(IDXGISwapChain** out) ;
		HRESULT CreateCBuffer(wchar_t const* name, size_t bufferSize);
		HRESULT GetCBuffer(wchar_t const* name, ID3D11Buffer** out);
		HRESULT LoadPixelShader(wchar_t const* id, wchar_t const* path, char const* entry);
		HRESULT LoadPixelShaderFromBytes(wchar_t const* id, u8 const* ptr, u32 len);
		HRESULT GetPixelShader(wchar_t const* id, ID3D11PixelShader** out) ;
		TextureManager& GetTextureManager();
		RenderSystem& GetRenderSystem() override;
		Texture2D* GetSwapChainTexture();
		Texture2D* GetDefaultDepthBuffer();
	private:
		ComPtr<ID3D11Device>		device_;
		ComPtr<ID3D11DeviceContext> deviceContext_;
		ComPtr<IDXGISwapChain>		swapChain_;
		std::unordered_map<std::wstring, ComPtr<ID3D11PixelShader> >	pixelShaders_;

		std::unique_ptr<TextureManager>							textureManager_;
		std::unique_ptr<RenderSystem>							renderSystem_;
		std::unique_ptr<Kumazuma::Texture2D>					swapChainTexture_;
		std::unique_ptr<Kumazuma::Texture2D>					defaultDepthBuffer_;
		std::unordered_map<std::wstring, ComPtr<ID3D11Buffer> > cbuffers_;
	};
}