#include "Player.h"
#include <game/TransformComponent.hpp>
#include "COMRenderObjectContainer.hpp"
#include "COMPlayerInput.hpp"
#include "ResourceManager.hpp"
#include "COMHeightmap.hpp"
#include "PhysicsManager.hpp"
#include"COMMoveController.hpp"
using namespace DirectX;
auto Kumazuma::Client::SpawnPlayer(XMFLOAT3 const& initPosition) -> std::shared_ptr<Game::Object>
{
    auto physicsManager{ PhysicsManager::Instance() };
    auto resourceMgr{ ResourceManager::Instance() };
    auto physicsComponent = physicsManager->CreatePhysicsComponent(L"PLAYER_CAPSULE", 100.f, initPosition, XMFLOAT3{ 0.f, 0.0f, 0.f });
    if (physicsComponent == std::nullopt)
    {
        return nullptr;
    }
    std::shared_ptr<Game::Object> pPlayerObj{ new Game::Object{} };
    pPlayerObj->AddComponent<Game::TransformComponent>();
    pPlayerObj->AddComponent<COMRenderObjectContainer>();
    pPlayerObj->AddComponent<COMPlayerInput>();
    pPlayerObj->AddComponent<PhysicsComponent>(std::move(*physicsComponent));
    pPlayerObj->AddComponent<COMMoveController>(1.f);
    //pPlayerObj->AddComponent<COMHeightMap>(std::move(heightmap));
    auto renderObj{ resourceMgr->GetSkinnedMesh(L"player") };
    renderObj->SetAnimationSet(0);
    pPlayerObj->GetComponent<COMRenderObjectContainer>()->Insert(L"", std::move(renderObj));
    return pPlayerObj;
}
