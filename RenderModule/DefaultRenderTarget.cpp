#include "pch.h"
#include "DefaultRenderTarget.hpp"
#include "RenderModule.h"
DefaultRenderTarget::DefaultRenderTarget(RenderModule* pRenderModule)
{
    COMPtr<IDirect3DDevice9> pDevice;
    pRenderModule->GetDevice(&pDevice);
    pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &this->m_pSurface);
    D3DSURFACE_DESC surfaceDesc{};
    m_pSurface->GetDesc(&surfaceDesc);
    m_fmt = surfaceDesc.Format;
    m_width = surfaceDesc.Width;
    m_height = surfaceDesc.Height;

}

auto DefaultRenderTarget::GetSurface(IDirect3DSurface9** ppOutSurface) -> HRESULT
{
    if (ppOutSurface == nullptr)
    {
        return E_POINTER;
    }
    return E_NOTIMPL;
}

auto DefaultRenderTarget::GetWidth() const -> u32
{
    return m_width;
}

auto DefaultRenderTarget::GetHeight() const -> u32
{
    return m_height;
}

auto DefaultRenderTarget::GetFormat() const -> D3DFORMAT
{
    return m_fmt;
}
