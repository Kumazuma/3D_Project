#include<d3d9.h>
#include"COMPtr.hpp"
#include"typedef.hpp"
class RenderModule;
class TextureRenderTarget
{
public:
	TextureRenderTarget(RenderModule* pRenderModule, u32 width, u32 height, D3DFORMAT textureFormat);
	TextureRenderTarget(TextureRenderTarget&& rhs) noexcept;
public:
	static auto Create(RenderModule* pRenderModule, u32 width, u32 height, D3DFORMAT textureFormat, TextureRenderTarget** ppOut)->HRESULT;
public:
	auto GetSurface(IDirect3DSurface9** ppOutSurface)->HRESULT;
	auto GetTexture(IDirect3DTexture9** ppOutTexture)->HRESULT;
	auto GetWidth() const->u32;
	auto GetHeight()const->u32;
	auto GetFormat()const->D3DFORMAT;
private:
	COMPtr<IDirect3DTexture9> m_pTexture;
	COMPtr<IDirect3DSurface9> m_pSurface;
	D3DFORMAT m_fmt;
	u32 m_width;
	u32 m_height;
};