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
#include "COMPlayerRender.hpp"
#include "COMWeaponRender.hpp"
#include "COMColliderRender.hpp"
#include "COMPlayerSkill.hpp"
#include "COMSkills.hpp"
#include "COMRenderer.hpp"
#include "app.h"
using namespace DirectX;
auto Kumazuma::Client::SpawnPlayer(std::shared_ptr<Graphics::GameRenderer> const& renderer, XMFLOAT3 const& initPosition) -> std::shared_ptr<Game::Object>
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
    HitPoints hitPoints{ 5000 };
    std::shared_ptr<Game::Object> pPlayerObj{ new Game::Object{} };
    pPlayerObj->AddComponent<Game::TransformComponent>();
    pPlayerObj->AddComponent<COMPlayerInput>();
    pPlayerObj->AddComponent<COMRenderer>(renderer);
    pPlayerObj->AddComponent<COMCharacterStateInfo>(hitPoints, manaPoints);
    pPlayerObj->AddComponent<PhysicsCharacterController>(std::move(*physicsComponent));
    pPlayerObj->AddComponent<COMSkills>();
    //pPlayerObj->AddComponent<COMPlayerSkills>();
    pPlayerObj->AddComponent<COMCollider>();
    auto comCollider{ pPlayerObj->GetComponent<COMCollider>() };
    
    auto const& colliders{ meta->GetCollidersRef() };
    i32 const colliderCount{ static_cast<i32>( colliders .size())};
    i32 i{};
    for (auto const& collider : colliders)
    {
        comCollider->Add(std::to_wstring(i), collider);
        ++i;
    }
    auto renderModule{ App::Instance()->GetRenderModule() };

    //pPlayerObj->AddComponent<COMHeightMap>(std::move(heightmap));
    
    auto characterMesh{ std::shared_ptr<SkinnedXMeshObject>{ resourceMgr->GetSkinnedMesh(L"player")} };
    characterMesh->SetAnimationSet(0);
    pPlayerObj->AddComponent<COMSkinnedMeshRender>(renderer, characterMesh);

    auto weaponObject{ std::make_shared<Game::Object>() };
    //auto mWeaponTransfrom{ XMMatrixScaling(50.f, 50.f, 50.f) * XMMatrixRotationRollPitchYaw(XM_PI, 0.f, XM_PI * 0.5f) * mWeaponSlotTransform * mTransform };
    auto colliderRenderDummy{ std::make_shared<Game::Object>() };
    colliderRenderDummy->AddComponent<COMColliderRender>(renderer, characterMesh, pPlayerObj);
    auto weaponMesh{std::shared_ptr<RenderObject>(resourceMgr->GetOBJMesh(L"sword2") )};
    weaponObject->AddComponent<Game::TransformComponent>();
    weaponObject->AddComponent<COMWeaponRender>(renderer, L"character_human_male_humanmale_bone_73", weaponMesh);
    weaponObject->AddComponent<COMCollider>();
    //renderer, L"character_human_male_humanmale_bone_73"
    //container->Insert(L"WEAPON", std::move(weaponMesh));
    auto weaponTransform{ weaponObject->GetComponent<Game::TransformComponent>() };
    weaponTransform->SetScale(50.f);
    weaponTransform->SetRotation({ XM_PI, 0.f, XM_PI * 0.5f });
    pPlayerObj->AddChild(L"WEAPON",weaponObject);
    pPlayerObj->AddChild(L"COLLIDER_RENDER_DUMMY", colliderRenderDummy);
    Collider weaponCollider;
    weaponCollider.SetType(ColliderType::BOX);
    auto& boxColliderAttr{ static_cast<BoxColliderAttribute&>(weaponCollider.GetAttributeRef()) };
    boxColliderAttr.SetDepth(1.6f * 50.f);
    boxColliderAttr.SetHeight(20.f);
    boxColliderAttr.SetWidth(20.f);
    weaponCollider.SetFrameName(L"character_human_male_humanmale_bone_73");
    weaponCollider.SetOffset({ 0.f, 0.f, 1.6f * 25.f });
    auto weaponColliderCom{ weaponObject->GetComponent<COMCollider>() };
    weaponColliderCom->Add(L"WEAPON_COLLIDER", weaponCollider);
    return pPlayerObj;
}
