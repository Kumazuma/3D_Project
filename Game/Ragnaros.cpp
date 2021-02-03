#include "Ragnaros.hpp"
#include<game/TransformComponent.hpp>
#include"COMRenderObjectContainer.hpp"
#include"COMRagnarosAI.hpp"
#include"COMCollider.hpp"
#include"ResourceManager.hpp"
#include"COMCharacterStateInfo.hpp"
#include"COMPlayerRender.hpp"
#include"COMWeaponRender.hpp"
#include"COMPickingSphere.hpp"
#include"COMColliderRender.hpp"
#include"COMRenderer.hpp"
#include"COMCharacterStateInfo.hpp"
#include"SphareColliderAttribute.hpp"
constexpr wchar_t CHARACTER_MESH[]{ L"CHARACTER" };

using namespace DirectX;
auto Kumazuma::Client::SpawnRagnaros(std::shared_ptr<Graphics::GameRenderer> const& renderer, XMFLOAT3 const& initPosition) -> std::shared_ptr<Game::Object>
{
	auto resourceMgr{ ResourceManager::Instance() };
	std::shared_ptr<Game::Object> ragnaros{ new Game::Object{} };
	ManaPoints manaPoints{ 9999 };
	HitPoints hitPoints{ 1500 };
	auto renderObj{ std::shared_ptr<SkinnedXMeshObject>{ resourceMgr->GetSkinnedMesh(L"ragnaros") }  };
	ragnaros->AddComponent<Game::TransformComponent>();
	ragnaros->AddComponent<COMSkinnedMeshRender>(renderer, renderObj);
	ragnaros->AddComponent<COMRagnarosAI>();
	ragnaros->AddComponent<COMCollider>();
	ragnaros->AddComponent<COMRenderer>(renderer);
	ragnaros->AddComponent<COMCharacterStateInfo>(hitPoints, manaPoints);
	ragnaros->AddComponent<COMPickingSphere>(f32x3{ 0.f, 700.f, 0.f }, 700.f);
	ragnaros->GetComponent<Game::TransformComponent>()->SetPosition(initPosition);
	ragnaros->GetComponent<Game::TransformComponent>()->SetScale(0.01f);
	//pPlayerObj->AddComponent<COMHeightMap>(std::move(heightmap));
	auto hammer{ std::shared_ptr<RenderObject>{resourceMgr->GetOBJMesh(L"sulfuras")} };
	hammer->SetVisible(false);
	renderObj->SetAnimationSet(0);

	auto weaponObject{ std::make_shared<Game::Object>() };
	weaponObject->AddComponent<COMWeaponRender>(renderer, L"creature_ragnaros2_ragnaros2_bone_81", hammer);
	weaponObject->AddComponent<COMCollider>();
	weaponObject->GetComponent<COMWeaponRender>()->SetVisible(false);
	weaponObject->AddComponent<Game::TransformComponent>();
	auto weaponTransformCom{ weaponObject->GetComponent<Game::TransformComponent>() };
	weaponTransformCom->SetScale(2000.f);
	weaponTransformCom->SetRotation(XM_PI, 0.f, XM_PI * 0.5f);
	weaponTransformCom->SetPosition(290.f, -200.f, 1000.f);
	/*auto mScale{ XMMatrixScaling(2000.f, 2000.f, 2000.f) };
	auto mRotation{ XMMatrixRotationRollPitchYaw(XM_PI,0.f, XM_PI * 0.5f) };
	auto mOffset{ XMMatrixTranslation(290.f, -200.f, 1000.f) };*/
	ragnaros->AddChild(L"WEAPON",weaponObject);
	auto dummyColliderRenderObj{ std::make_shared<Game::Object>() };
	dummyColliderRenderObj->AddComponent<COMColliderRender>(renderer, renderObj, ragnaros);
	ragnaros->AddChild(L"DUMMY_COLLIDER_RENDER", dummyColliderRenderObj);
	auto comCollider{ ragnaros->GetComponent<COMCollider>() };
	Collider collider;
	collider.SetType(ColliderType::SPHERE);
	collider.SetFrameName(L"creature_ragnaros2_ragnaros2_bone_49");
	auto& sphereAttr{ static_cast<SphereColliderAttribute&>(collider.GetAttributeRef()) };
	sphereAttr.SetRadius(1900.f);
	comCollider->Add(L"BODY", collider);
	//for (auto const& collider : )
	//{

	//}
	return ragnaros;
}
