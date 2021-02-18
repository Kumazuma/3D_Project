#include "pch.h"
#include "NativeRenderer.hpp"

MaptoolRenderer::NativeRenderer::NativeRenderer(RenderModule* renderModule)
{
    renderModule_ = renderModule;
}

auto MaptoolRenderer::NativeRenderer::GetRenderModuleRef() -> RenderModule&
{
    // TODO: 여기에 return 문을 삽입합니다.
    return *renderModule_;
}

auto MaptoolRenderer::NativeRenderer::GetDevice(IDirect3DDevice9** out) -> HRESULT
{
    return renderModule_->GetDevice(out);
}

auto MaptoolRenderer::NativeRenderer::SetProjMatrix(DirectX::XMFLOAT4X4 const& mat) -> void
{
    projSpaceMatrix_ = mat;
}

auto MaptoolRenderer::NativeRenderer::SetViewMatrix(DirectX::XMFLOAT4X4 const& mat) -> void
{
    viewSpaceMatrix_ = mat;
}

auto MaptoolRenderer::NativeRenderer::GetProjMatrix(DirectX::XMFLOAT4X4* pOut) -> HRESULT
{
    *pOut = projSpaceMatrix_;
    return S_OK;
}

auto MaptoolRenderer::NativeRenderer::GetViewMatrix(DirectX::XMFLOAT4X4* pOut) -> HRESULT
{
    *pOut = viewSpaceMatrix_;
    return S_OK;
}

auto MaptoolRenderer::NativeRenderer::GetFrustum(Frustum* const pOutFrustum) -> HRESULT
{
    return E_NOTIMPL;
}
