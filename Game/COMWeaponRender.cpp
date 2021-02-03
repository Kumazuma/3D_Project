#include "COMWeaponRender.hpp"
#include <game/object.hpp>
#include <game/TransformComponent.hpp>
#include <GameRenderer.hpp>
#include "COMPlayerRender.hpp"
using namespace DirectX;
Kumazuma::Client::COMWeaponRender::COMWeaponRender(std::shared_ptr<Graphics::GameRenderer> const& renderer, std::wstring const& frameName, std::shared_ptr<RenderObject> const& mesh)
{
    Bind(Game::EVT_UPDATE, &COMWeaponRender::Update);
    frameName_ = frameName;
    renderer_ = renderer;
    mesh_ = mesh;
    visible_ = true;
}

auto Kumazuma::Client::COMWeaponRender::Update(Game::UpdateEvent const& evt) -> void
{
    if (visible_)
    {
        renderer_->AddRender(
            Graphics::RenderGroupID::Deferred,
            std::static_pointer_cast<RenderComponent>(shared_from_this())
        );
    }
    
}

auto Kumazuma::Client::COMWeaponRender::Render(RenderModule& renderModule, Graphics::GameRenderer& renderer) -> void
{
    auto object{ GetObj().lock() };
    if (object == nullptr)
        return;
    auto parentObject{ object->GetParent() };
    auto parentTransformCom{ parentObject->GetComponent<Game::TransformComponent>() };
    auto parentRenderCom{ std::static_pointer_cast<COMSkinnedMeshRender const>(parentObject->GetComponent<Graphics::RenderComponent>()) };
    auto framePtr{ parentRenderCom->GetMesh()->FindFrameTransfromByName(frameName_) };
    auto transform{ object->GetComponent<Game::TransformComponent>() };
    f32x44 parentTranformMatrix{};
    f32x44 transformMatrix{};
    transform->GenerateTransformMatrix(&transformMatrix);
    parentTransformCom->GenerateTransformMatrix(&parentTranformMatrix);
    
    auto mParentTransform{ DirectX::XMLoadFloat4x4(&parentTranformMatrix) };
    auto mFrameMatrix{ DirectX::XMLoadFloat4x4(framePtr) };
    auto mRenderTransform{ XMLoadFloat4x4(&transformMatrix) * mFrameMatrix * mParentTransform };
    XMStoreFloat4x4(&transformMatrix, mRenderTransform);
    COMPtr<ID3DXEffect> effect;
    renderer.GetEffect(L"DEFERRED_EFFECT", &effect);
    UINT passCount{};
    effect->Begin(&passCount, 0);
    effect->SetMatrix("g_mWorld", reinterpret_cast<D3DXMATRIX*>(&transformMatrix));
    effect->BeginPass(0);
    mesh_->Render(&renderer, effect.Get());
    effect->EndPass();
    effect->End();

}

auto Kumazuma::Client::COMWeaponRender::Clone() const -> Game::Component*
{
    return nullptr;
}

auto Kumazuma::Client::COMWeaponRender::SetVisible(bool flag) -> void
{
    visible_ = flag;
}

auto Kumazuma::Client::COMWeaponRender::IsVisible() const -> bool
{
    return visible_;
}
