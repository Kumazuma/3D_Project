#include "COMFinale.hpp"
#include <GameRenderer.hpp>
#include <SimplePlane.hpp>
using namespace DirectX;
IMPLEMENT_COMPONENT_CLASS(Kumazuma::Client::COMFinale)
Kumazuma::Client::COMFinale::COMFinale(std::shared_ptr<Graphics::GameRenderer> const& renderer):
    Component(TAG)
{
    COMPtr<IDirect3DDevice9> device;
    renderer->GetDevice(&device);

}

auto Kumazuma::Client::COMFinale::Update(Game::UpdateEvent const& evt) -> void
{

}

auto Kumazuma::Client::COMFinale::Clone() const -> Game::Component*
{
    return nullptr;
}

Kumazuma::Client::COMFinaleRender::COMFinaleRender(std::shared_ptr<Graphics::GameRenderer> const& renderer)
{
    renderer_ = renderer;
    Bind(Game::EVT_UPDATE, &COMFinaleRender::Update);
    COMPtr<IDirect3DDevice9> device;
    renderer->GetDevice(&device);
    simplePlane_ = Graphics::SimplePlane::Create(device.Get());
    timeAccum_ = 0.f;
}

auto Kumazuma::Client::COMFinaleRender::Update(Game::UpdateEvent const& evt) -> void
{
    renderer_->AddRender(Graphics::RenderGroupID::PostCombine,std::static_pointer_cast<RenderComponent>( shared_from_this()));
    
    if (timeAccum_ < 5.f)
    {
        timeAccum_ += evt.GetDelta();
    }
}

auto Kumazuma::Client::COMFinaleRender::Render(RenderModule& renderModule, Graphics::GameRenderer& renderer) -> void
{
    f32 color = timeAccum_ / 5.f;
    XMVECTOR vColor{ XMVectorSet(0,0,0,color) };
    COMPtr<IDirect3DDevice9> device;
    COMPtr<ID3DXEffect> effect;
    XMMATRIX mScale{ XMMatrixScaling(2.f, 2.f, 1.f) };

    renderer.GetDevice(&device);
    renderer.GetEffect(L"EFFECT1", &effect);
    UINT passCount{};
    effect->SetVector("g_effect2Color", reinterpret_cast<D3DXVECTOR4*>(&vColor));
    effect->SetMatrix("g_mWorld", reinterpret_cast<D3DXMATRIX*>(&mScale));
    effect->Begin(&passCount, 0);
    effect->BeginPass(1);
    simplePlane_->Render(device.Get());
    effect->EndPass();
    effect->End();
}

auto Kumazuma::Client::COMFinaleRender::Clone() const -> Game::Component*
{
    return nullptr;
}
