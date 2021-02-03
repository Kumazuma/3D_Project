#pragma once
#include "RenderTarget.hpp"
class DLL_CLASS DefaultRenderTarget: public IRenderTarget
{
public:
	DefaultRenderTarget(RenderModule* pRenderModule);
	auto GetSurface(IDirect3DSurface9** ppOutSurface)->HRESULT;
	auto GetWidth() const->u32;
	auto GetHeight()const->u32;
	auto GetFormat()const->D3DFORMAT;
private:
	COMPtr<IDirect3DSurface9> m_pSurface;
	D3DFORMAT m_fmt;
	u32 m_width;
	u32 m_height;
};