#include "COMPlayerRender.hpp"
#include <GameRenderer.hpp>
#include <game/object.hpp>
#include <game/TransformComponent.hpp>
#include <WavefrontOBJMesh.h>
Kumazuma::Client::COMSkinnedMeshRender::COMSkinnedMeshRender(std::shared_ptr<Graphics::GameRenderer> const& renderer, std::shared_ptr<SkinnedXMeshObject> const& obj)
{
    if (dynamic_cast<WavefrontOBJMesh*>(obj.get()) != nullptr)
    {
        DebugBreak();
    }
    visible_ = true;
	skinnedMesh_ = obj;
    renderer_ = renderer;
    Bind(Game::EVT_UPDATE, &COMSkinnedMeshRender::Update);
}

auto Kumazuma::Client::COMSkinnedMeshRender::Clone() const -> Game::Component*
{
	return new COMSkinnedMeshRender{ *this };
}

auto Kumazuma::Client::COMSkinnedMeshRender::Update(Game::UpdateEvent const& evt) -> void
{
    if (visible_)
    {
        renderer_->AddRender(Graphics::RenderGroupID::Deferred, std::static_pointer_cast<RenderComponent>(shared_from_this()));
    }
}

auto Kumazuma::Client::COMSkinnedMeshRender::Render(RenderModule& renderModule, Graphics::GameRenderer& renderer) -> void 
{
    auto object{ GetObj().lock() };
    if (object == nullptr)
        return;
    auto transform{ object->GetComponent<Game::TransformComponent>() };
    f32x44 transformMatrix{};
    COMPtr<ID3DXEffect> effect;
    transform->GenerateTransformMatrix(&transformMatrix);
    renderer.GetEffect(L"DEFERRED_EFFECT", &effect);
    UINT passCount{};
    effect->Begin(&passCount, 0);
    effect->SetMatrix("g_mWorld", reinterpret_cast<D3DXMATRIX*>(&transformMatrix));
    effect->BeginPass(0);
    skinnedMesh_->Render(&renderer, effect.Get());
    effect->EndPass();
    effect->End();
}

auto Kumazuma::Client::COMSkinnedMeshRender::GetMesh()const -> std::shared_ptr<SkinnedXMeshObject>
{
    return skinnedMesh_;
}

auto Kumazuma::Client::COMSkinnedMeshRender::SetVisible(bool flag) -> void
{
    visible_ = flag;
}

auto Kumazuma::Client::COMSkinnedMeshRender::IsVisible() const -> bool
{
    return visible_;
}

auto Kumazuma::Client::COMSkinnedMeshRender::GetRenderer() const -> std::shared_ptr<Graphics::GameRenderer>
{
    return renderer_;
}
