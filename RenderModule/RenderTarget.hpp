#pragma once
#include<d3d9.h>
#include"COMPtr.hpp"
#include"typedef.hpp"
#include"Common.h"
class RenderModule;
class DLL_CLASS IRenderTarget
{
public:
	IRenderTarget();
	IRenderTarget(IRenderTarget const& rhs);
	IRenderTarget(IRenderTarget&& rhs) noexcept;
	virtual ~IRenderTarget() = default;
	virtual auto GetSurface(IDirect3DSurface9** ppOutSurface)->HRESULT = 0;
	virtual auto GetWidth() const->u32 = 0;
	virtual auto GetHeight()const->u32 = 0;
	virtual auto GetFormat()const->D3DFORMAT = 0;
	auto __stdcall AddRef()->DWORD;
	auto __stdcall Release()->DWORD;
private:
	LONG cnt_;
};