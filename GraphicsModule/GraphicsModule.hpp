#pragma once
#include"common.hpp"
#include"typedef.hpp"
#include<d3d11.h>
#include<dxgi.h>
namespace Kumazuma
{
	class Texture2D;
	class TextureManager;
	class DLL_CLASS GraphicsModule
	{
	public:
		virtual ~GraphicsModule() {};
		static GraphicsModule* Create(HWND hWindow, Size2D<u32> const& bufferSize, bool fullScreen);
		virtual HRESULT CreateCBuffer(wchar_t const* name, size_t bufferSize) = 0;
		virtual HRESULT GetDevice(ID3D11Device** out) = 0;
		virtual HRESULT GetImmediateContext(ID3D11DeviceContext** out) = 0;
		virtual HRESULT GetSwapChain(IDXGISwapChain** out) = 0;
		virtual HRESULT GetCBuffer(wchar_t const* name, ID3D11Buffer** out) = 0;
		virtual HRESULT LoadPixelShader(wchar_t const* path, ID3D11PixelShader** out)		=0;
		virtual HRESULT LoadVertexShader(wchar_t const* path, ID3D11VertexShader** out)		=0;
		virtual HRESULT LoadComputeShader(wchar_t const* path, ID3D11ComputeShader** out)	=0;
		virtual Texture2D* GetDefaultDepthBuffer() = 0;
		virtual Texture2D* GetSwapChainTexture() = 0;
		virtual TextureManager& GetTextureManager() = 0;
		void LockDevice();
		void UnlockDevice();
		void lock();
		void unlock();
	};
}