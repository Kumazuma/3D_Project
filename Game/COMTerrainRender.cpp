#include "COMTerrainRender.hpp"
#include <game/object.hpp>
#include <game/TransformComponent.hpp>
#include <GameRenderer.hpp>
Kumazuma::Client::COMTerrainRender::COMTerrainRender(
    std::shared_ptr<Graphics::GameRenderer> renderer,
    std::shared_ptr<WavefrontOBJMesh> const& mesh):
    renderer_{std::move(renderer)},
    mesh_{mesh}
{
    Bind(Game::EVT_UPDATE, &COMTerrainRender::Update);
}

auto Kumazuma::Client::COMTerrainRender::Update(Game::UpdateEvent const&) -> void
{
    renderer_->AddRender(Graphics::RenderGroupID::Deferred, std::static_pointer_cast<Graphics::RenderComponent>( this->shared_from_this()));
}

auto Kumazuma::Client::COMTerrainRender::Render(RenderModule& renderModule, Graphics::GameRenderer& renderer) -> void 
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
    mesh_->Render(&renderer, effect.Get());
    effect->EndPass();
    effect->End();
}

auto Kumazuma::Client::COMTerrainRender::Clone() const -> Game::Component* 
{
    return new COMTerrainRender{ *this };
}
