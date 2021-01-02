#include "Player.h"
#include<game/TransformComponent.hpp>
#include"COMRenderObjectContainer.hpp"
#include"COMPlayerInput.hpp"
#include"ResourceManager.hpp"
#include "COMHeightmap.hpp"
auto Kumazuma::Client::SpawnPlayer(std::shared_ptr<HeightMap> heightmap) -> std::shared_ptr<Game::Object>
{
    auto resourceMgr{ ResourceManager::Instance() };
    std::shared_ptr<Game::Object> pPlayerObj{ new Game::Object{} };
    pPlayerObj->AddComponent<Game::TransformComponent>();
    pPlayerObj->AddComponent<COMRenderObjectContainer>();
    pPlayerObj->AddComponent<COMPlayerInput>();
    pPlayerObj->AddComponent<COMHeightMap>(std::move(heightmap));
    auto renderObj{ resourceMgr->GetSkinnedMesh(L"player") };
    renderObj->SetAnimationSet(0);
    pPlayerObj->GetComponent<COMRenderObjectContainer>()->Insert(L"", std::move(renderObj));
    return pPlayerObj;
}
