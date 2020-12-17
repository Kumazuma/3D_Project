#include "pch.h"
#include "TextureRenderTarget.h"
#include "RenderModule.h"
TextureRenderTarget::TextureRenderTarget(RenderModule* pRenderModule, u32 width, u32 height, D3DFORMAT textureFormat):
    m_width{ width },
    m_height{ height },
    m_fmt{textureFormat}
{
    HRESULT hr;
    COMPtr<IDirect3DDevice9> pDevice;
    pRenderModule->GetDevice(&pDevice);

    hr = pDevice->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, textureFormat, D3DPOOL_DEFAULT, &m_pTexture, nullptr);
    if (FAILED(hr)) throw hr;
    hr = m_pTexture->GetSurfaceLevel(0, &m_pSurface);
    if (FAILED(hr)) throw hr;

}

TextureRenderTarget::TextureRenderTarget(TextureRenderTarget&& rhs) noexcept :
    m_pSurface{std::move(rhs.m_pSurface)},
    m_pTexture{std::move(rhs.m_pTexture)},
    m_fmt{std::move(rhs.m_fmt)},
    m_width{std::move(rhs.m_width)},
    m_height{std::move(rhs.m_height)}
{

}

auto TextureRenderTarget::Create(RenderModule* pRenderModule, u32 width, u32 height, D3DFORMAT textureFormat, TextureRenderTarget** ppOut) -> HRESULT
{
    try
    {
        if (ppOut == nullptr) throw E_POINTER;

        TextureRenderTarget obj{ pRenderModule, width, height, textureFormat };
        *ppOut = new TextureRenderTarget{ std::move(obj) };
        return S_OK;
    }
    catch (HRESULT hr)
    {
        return hr;
    }
    return E_NOTIMPL;
}

auto TextureRenderTarget::GetSurface(IDirect3DSurface9** ppOutSurface) -> HRESULT
{
    if (ppOutSurface == nullptr) return E_POINTER;
    *ppOutSurface = m_pSurface.Get();
    m_pSurface->AddRef();
    return S_OK;
}

auto TextureRenderTarget::GetTexture(IDirect3DTexture9** ppOutTexture) -> HRESULT
{
    if (ppOutTexture == nullptr) return E_POINTER;
    *ppOutTexture = m_pTexture.Get();
    m_pTexture->AddRef();
    return S_OK;
}

auto TextureRenderTarget::GetWidth() const -> u32
{
    return m_width;
}

auto TextureRenderTarget::GetHeight() const -> u32
{
    return m_height;
}

auto TextureRenderTarget::GetFormat() const -> D3DFORMAT
{
    return m_fmt;
}
