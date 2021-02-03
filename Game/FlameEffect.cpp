#include "pch.h"
#include "FlameEffect.hpp"
#include "FlameEffectShare.hpp"
#include <GameRenderer.hpp>
#include <TextureRenderTarget.h>
#include <SpinLock.hpp>
#include <mutex>
static SpinLock g_locker;
using namespace DirectX;
_BEGIN_NS(Kumazuma::Client)
std::weak_ptr<FlameEffectShareState> FlameEffect::s_shareState_;
FlameEffect::FlameEffect(RenderModule* renderModule)
{
    //< 1단계. 시드 설정
    std::random_device rn;
    //< 2단계. 분포 설정 ( 정수 )
    rand_ = std::mt19937_64{ rn() };
    range_ = std::uniform_int_distribution<int>{ 0, 16 };
    seek_ = 0.f;
    length_ = 1.f;
    scale_ = 1.f;
    center_ = { 0.f, 0.f, 0.f };
    shareState_ = s_shareState_.lock();
    if (shareState_ == nullptr)
    {
        std::lock_guard<decltype(g_locker)> guard{ g_locker };
        shareState_ = s_shareState_.lock();
        if (shareState_ == nullptr)
        {
            shareState_ = std::make_shared<FlameEffectShareState>(renderModule);
            s_shareState_ = shareState_;
        }
    }
    COMPtr<IDirect3DDevice9> device;
    renderModule->GetDevice(&device);
    simplePlane_ = Graphics::SimplePlane::Create(device.Get());
}

auto FlameEffect::Render(RenderModule& renderModule, Graphics::GameRenderer& renderer) -> void
{
    
    COMPtr<IDirect3DDevice9>    device;
    COMPtr<ID3DXEffect>         effect;
    UINT        passCount{};
    u32 const   texture1Index{ static_cast<u32>((16.f * seek_ ) / length_) };
    u32         row{ texture1Index / 4 };
    u32         cols{ texture1Index % 4 };
    XMMATRIX    mParentTransform{  XMMatrixTranslation(center_.x,center_.y, center_.z) };
    /*float4x4 g_mUVTransform1;
    float4x4 g_mUVTransform2;*/
    XMMATRIX    mUVTransform1{ XMMatrixScaling(0.25f, 0.25f, 1) * XMMatrixTranslation(row * 0.25f, cols * 0.25f, 0) };
    XMMATRIX    mUVTransform2{ XMMatrixIdentity() };
    f32x44      viewSpaceMatrix{};
    XMMATRIX    mCameraTransform{};
    XMMATRIX    mScaleTransform{ XMMatrixScaling(scale_, scale_, scale_) };
    XMMATRIX    mRotation{ XMMatrixRotationY(XMConvertToRadians(180.f / 8.f)) };

    COMPtr<IRenderTarget> renderTarget;
    COMPtr<IDirect3DTexture9> depthMapTex;
    renderModule.GetRenderTarget(Graphics::GameRenderer::RENDER_TARGET_DEPTH, &renderTarget);
    static_cast<TextureRenderTarget*>(renderTarget.Get())->GetTexture(&depthMapTex);
    renderer.GetDevice(&device);
    renderer.GetEffect(L"EFFECT1", &effect);
    renderer.GetViewMatrix(&viewSpaceMatrix);
    mCameraTransform = XMMatrixInverse(nullptr, XMLoadFloat4x4(&viewSpaceMatrix));
    
    mCameraTransform.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);
    mParentTransform = mScaleTransform* mCameraTransform * mParentTransform;
    effect->SetMatrix("g_mWorld", reinterpret_cast<D3DXMATRIX const*>(&mParentTransform));
    effect->SetMatrix("g_mUVTransform1", reinterpret_cast<D3DXMATRIX const*>(&mUVTransform1));
    effect->SetMatrix("g_mUVTransform2", reinterpret_cast<D3DXMATRIX const*>(&mUVTransform2));
    effect->SetTexture("g_depthMap", depthMapTex.Get());
    effect->Begin(&passCount, 0);
    effect->BeginPass(0);
    shareState_->Render(device.Get(), effect.Get());
    simplePlane_->Render(device.Get());
    effect->EndPass();
    effect->End();
}

auto FlameEffect::GetCenter(DirectX::XMFLOAT3* out) const -> void
{
    if (out == nullptr)
        return;
    *out = center_;
}

auto FlameEffect::SetSize(f32 scale) -> void
{
    scale_ = scale;
}

auto FlameEffect::PlayEffect(f32 timeDelta) -> void
{
    SetSeek(seek_ + timeDelta);
}

auto FlameEffect::SetPlayLength(f32 length) -> void
{
    f32 ratio{ seek_ / length_ };
    seek_ = ratio * length;
    length_ = length;
}

auto FlameEffect::SetCenter(f32x3 const& pos) -> void
{
    center_ = pos;
}
auto FlameEffect::SetSeek(f32 seek) -> void
{
    seek_ = seek;
    if (seek_ >= length_)
    {
        seek_ = seek_ - length_;
    }
}
auto FlameEffect::Reset() -> void
{
    seek_ = 0.f;
}
auto FlameEffect::Clone() const -> FlameEffect*
{
    return new FlameEffect{ *this };
}
_END_NS