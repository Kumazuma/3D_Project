#include "Player.h"
#include <game/TransformComponent.hpp>
#include "COMRenderObjectContainer.hpp"
#include "COMPlayerInput.hpp"
#include "ResourceManager.hpp"
#include "COMHeightmap.hpp"
#include "PhysicsManager.hpp"
using namespace DirectX;
auto Kumazuma::Client::SpawnPlayer(XMFLOAT3 const& initPosition) -> std::shared_ptr<Game::Object>
{
    auto physicsManager{ PhysicsManager::Instance() };
    auto resourceMgr{ ResourceManager::Instance() };
    auto physicsComponent = physicsManager->CreateCharacterController(0.4f, 4.0f, initPosition, XMFLOAT3{ 0.f, -2.4f, 0.f });
    if (physicsComponent == std::nullopt)
    {
        return nullptr;
    }
    std::shared_ptr<Game::Object> pPlayerObj{ new Game::Object{} };
    pPlayerObj->AddComponent<Game::TransformComponent>();
    pPlayerObj->AddComponent<COMRenderObjectContainer>();
    pPlayerObj->AddComponent<COMPlayerInput>();
    pPlayerObj->AddComponent<PhysicsCharacterController>(std::move(*physicsComponent));
    //pPlayerObj->AddComponent<COMHeightMap>(std::move(heightmap));
    auto renderObj{ resourceMgr->GetSkinnedMesh(L"player") };
    renderObj->SetAnimationSet(0);
    pPlayerObj->GetComponent<COMRenderObjectContainer>()->Insert(L"CHARACTER", std::move(renderObj));
    return pPlayerObj;
}
