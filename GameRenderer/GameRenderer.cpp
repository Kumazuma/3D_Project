#include "pch.h"
#include "include/GameRenderer.hpp"
#include "GameRendererImpl.hpp"
wchar_t const* const Kumazuma::Graphics::GameRenderer::RENDER_TARGET_ALBEDO{ L"RT_ALBEDO" };
wchar_t const* const Kumazuma::Graphics::GameRenderer::RENDER_TARGET_NORMAL{ L"RT_NORMAL" };
wchar_t const* const Kumazuma::Graphics::GameRenderer::RENDER_TARGET_DEPTH{ L"RT_DEPTH" };
wchar_t const* const Kumazuma::Graphics::GameRenderer::RENDER_TARGET_SPECULAR{ L"RT_SPECULAR" };
auto Kumazuma::Graphics::GameRenderer::Create(RenderModule* renderModule, GameRenderer** out) -> HRESULT
{
    try
    {
        GameRendererImpl obj{ renderModule };
        *out = new GameRendererImpl{ std::move(obj) };
        return S_OK;
    }
    catch (...)
    {
        return E_FAIL;
    }
    return E_FAIL;
}
