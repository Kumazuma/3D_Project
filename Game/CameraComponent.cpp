#include "CameraComponent.hpp"
#include <Windows.h>
#include <game/object.hpp>
#include <game/TransformComponent.hpp>
using namespace Kumazuma::Client;
using namespace Kumazuma::Game;
using namespace DirectX;
const ComponentTag< CameraComponent> CameraComponent::TAG{"CameraComponent"};
Kumazuma::Client::CameraComponent::CameraComponent():
    Component{ TAG }
{
    Bind(EVT_UPDATE, &CameraComponent::Update);
}

auto Kumazuma::Client::CameraComponent::Clone() const -> Game::Component*
{
    return nullptr;
}

auto Kumazuma::Client::CameraComponent::Update(Game::UpdateEvent const& event) -> void
{
    auto obj{ GetObj().lock() };
    auto transformComponent{ obj->GetComponent(Game::TransformComponent::TAG) };
    XMFLOAT4X4 transformMatrix{};
    transformComponent->GenerateTransformMatrixWithoutScale(&transformMatrix);
    XMMATRIX mTransform{ XMLoadFloat4x4(&transformMatrix) };
    XMVECTOR vDelta{};
    XMVECTOR vRotation{};
    if (GetAsyncKeyState('W') & 0x8000)
    {
        vDelta += mTransform.r[2];
    }
    if (GetAsyncKeyState('S') & 0x8000)
    {
        vDelta -= mTransform.r[2];
    }
    if (GetAsyncKeyState('A') & 0x8000)
    {
        vDelta -= mTransform.r[0];
    }
    if (GetAsyncKeyState('D') & 0x8000)
    {
        vDelta += mTransform.r[0];
    }
    if (GetAsyncKeyState(VK_UP) & 0x8000)
    {
        vRotation -= XMVectorSet(1.f, 0.f, 0.f, 0.f);
    }
    if (GetAsyncKeyState(VK_DOWN) & 0x8000)
    {
        vRotation += XMVectorSet(1.f, 0.f, 0.f, 0.f);
    }
    if (GetAsyncKeyState(VK_LEFT) & 0x8000)
    {
        vRotation -= XMVectorSet(0.f, 1.f, 0.f, 0.f);
    }
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
    {
        vRotation += XMVectorSet(0.f, 1.f, 0.f, 0.f);
    }
    vDelta = XMVector3Normalize(vDelta) * event.GetDelta() * 300;
    vRotation = XMVector3Normalize(vRotation) * event.GetDelta();

    XMFLOAT3 newPosition{};
    XMFLOAT3 newRotations{};
    XMStoreFloat3(&newPosition, vDelta + mTransform.r[3]);
    XMStoreFloat3(&newRotations, vRotation + XMLoadFloat3(&transformComponent->GetRotation()) );
    transformComponent->SetPosition(newPosition);
    transformComponent->SetRotation(newRotations);
}
