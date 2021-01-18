#include "Player.h"
#include <game/TransformComponent.hpp>
#include "COMRenderObjectContainer.hpp"
#include "COMPlayerInput.hpp"
#include "ResourceManager.hpp"
#include "COMHeightmap.hpp"
#include "PhysicsManager.hpp"
#include "COMCollider.hpp"
#include "CharacterMeta.hpp"
#include "SimpleColliderBoxObject.h"
#include "SimpleColliderSphareObject.h"
#include "BoxColliderAttribute.hpp"
#include "SphareColliderAttribute.hpp"
#include "COMCharacterStateInfo.hpp"
#include "app.h"
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
    auto meta{ resourceMgr->GetCharacterMeta(L"player") };
    ManaPoints manaPoints{ 9999 };
    HitPoints hitPoints{ 9999 };
    std::shared_ptr<Game::Object> pPlayerObj{ new Game::Object{} };
    pPlayerObj->AddComponent<Game::TransformComponent>();
    pPlayerObj->AddComponent<COMRenderObjectContainer>();
    pPlayerObj->AddComponent<COMPlayerInput>();
    pPlayerObj->AddComponent<COMCharacterStateInfo>(hitPoints, manaPoints);
    pPlayerObj->AddComponent<PhysicsCharacterController>(std::move(*physicsComponent));
    pPlayerObj->AddComponent<COMCollider>();
    auto comCollider{ pPlayerObj->GetComponent<COMCollider>() };
    auto const& colliders{ meta->GetCollidersRef() };
    i32 const colliderCount{ static_cast<i32>( colliders .size())};
    auto container{ pPlayerObj->GetComponent<COMRenderObjectContainer>() };
    auto renderModule{ App::Instance()->GetRenderModule() };
    for (i32 i = 0; i < colliderCount; ++i)
    {
        auto id{ L"COOLIDER" + std::to_wstring(i + 1) };
        auto const& collider{ colliders[i] };
        auto const& offset{ collider.GetOffset() };
        comCollider->Add(id, collider);

        if (collider.GetType() == ColliderType::BOX)
        {
            auto const& boxAttr{ static_cast<BoxColliderAttribute const&>(collider.GetAttributeRef()) };
            SimpleBoxObject* box{};
            SimpleBoxObject::Create(renderModule.get(), boxAttr.GetWidth(), boxAttr.GetHeight(), boxAttr.GetDepth(), &box);
            box->SetOffset(offset.x, offset.y, offset.z);
            container->Insert(id, std::shared_ptr<RenderObject>{box});
        }
        else if(collider.GetType() == ColliderType::SPHERE)
        {
            auto const& sphereAttr{ static_cast<SphereColliderAttribute const&>(collider.GetAttributeRef()) };
            SimpleSphareObject* sphere{};
            SimpleSphareObject::Create(renderModule.get(),  &sphere);
            sphere->SetRadius(sphereAttr.GetRadius());
            sphere->SetOffset(offset.x, offset.y, offset.z);
            container->Insert(id, std::shared_ptr<RenderObject>{sphere});
        }
    }
    //pPlayerObj->AddComponent<COMHeightMap>(std::move(heightmap));
    
    auto characterMesh{ resourceMgr->GetSkinnedMesh(L"player") };
    characterMesh->SetAnimationSet(0);
    container->Insert(L"CHARACTER", std::move(characterMesh));
    
    auto weaponMesh{ resourceMgr->GetOBJMesh(L"sword2") };
    container->Insert(L"WEAPON", std::move(weaponMesh));

    return pPlayerObj;
}
