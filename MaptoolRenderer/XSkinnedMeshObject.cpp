#include "pch.h"
#include "COMPtr.hpp"
#include <d3dx9.h>
#include <RenderModule/SkinnedXMeshObject.h>
#include "NativeRenderer.hpp"
#include "XSkinnedMeshObject.hpp"
#include "SkinnedXMesh.hpp"
#include "MaptoolRenderer.h"
MaptoolRenderer::SkinnedXMeshObject::SkinnedXMeshObject(SkinnedXMesh^ mesh)
{
    mesh_ = safe_cast<SkinnedXMesh^>(mesh->Clone());
    static_cast<::SkinnedXMeshObject*>(mesh_->renderObject_)->SetAnimationSet(0);
    mesh_->PlayAnimation(0.1f);
    animationSet_ = 0.f;
}

auto MaptoolRenderer::SkinnedXMeshObject::PrepereRender(GraphicsDevice^ renderer) -> void
{
    renderer->AddRenderObject(RenderGroupID::Deferred, this);
}

auto MaptoolRenderer::SkinnedXMeshObject::Render(GraphicsDevice^ renderer) -> void
{
    UINT passCount{};
    COMPtr<IDirect3DDevice9> device;
    COMPtr<ID3DXEffect> effect;
    ::SkinnedXMeshObject* mesh{ static_cast<::SkinnedXMeshObject*> (mesh_->renderObject_) };
    renderer->RenderModuleHandle->GetDevice(&device);
    renderer->GetEffect("deferred", &effect);
    if (effect != nullptr)
    {
        effect->SetMatrix("g_mWorld", reinterpret_cast<D3DXMATRIX*>(transformPtr_));
        effect->Begin(&passCount, 0);
        effect->BeginPass(0);
    }
    else
    {
        device->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(transformPtr_));
    }
    mesh->Render(renderer->renderer_, effect.Get());
    if (effect != nullptr)
    {
        effect->EndPass();
        effect->End();
    }
}

auto MaptoolRenderer::SkinnedXMeshObject::SetAnimationSet(int i) -> void
{
    ::SkinnedXMeshObject* mesh{ static_cast<::SkinnedXMeshObject*> (mesh_->renderObject_) };
    mesh->SetAnimationSet(i, true);
}

auto MaptoolRenderer::SkinnedXMeshObject::PlayAnimation(float timeDelta) -> void
{
    ::SkinnedXMeshObject* mesh{ static_cast<::SkinnedXMeshObject*> (mesh_->renderObject_) };
    mesh->PlayAnimation(timeDelta);
}
auto MaptoolRenderer::SkinnedXMeshObject::AnimationSet::get()->unsigned int
{
    return animationSet_;
}
auto MaptoolRenderer::SkinnedXMeshObject::AnimationSet::set(unsigned int value)->void
{
    animationSet_ = value;
    static_cast<::SkinnedXMeshObject*>(mesh_->renderObject_)->SetAnimationSet(animationSet_);
    this->PropertyChanged(this, gcnew PropertyChangedEventArgs( "AnimationSet"));
}
auto MaptoolRenderer::SkinnedXMeshObject::AnimationCount::get()->unsigned int
{
    auto count = static_cast<::SkinnedXMeshObject*>(mesh_->renderObject_)->GetAnimationCount();
    return count;
}
auto MaptoolRenderer::SkinnedXMeshObject::FilePath::get()->String^
{
    return mesh_->FilePath;
}