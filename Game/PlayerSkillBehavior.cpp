#include "PlayerSkillBehavior.hpp"
#include "COMPlayerRender.hpp"
#include "framework.h"
#include "COMCollider.hpp"
#include "app.h"
#include "LayerTags.hpp"
#include "COMCharacterStateInfo.hpp"
#include <game/TransformComponent.hpp>
#include <game/runtime.hpp>
#include "EventTag.hpp"
#include "COMSkills.hpp"
#include "SphareColliderAttribute.hpp"
#include "COMRenderer.hpp"
#include "app.h"
#include<sstream>
using namespace DirectX;
auto Kumazuma::Client::PlayerNormalAttackBehavior::Update(f32 timedelta) -> void
{
	auto& object{ GetObjectRef() };
	auto renderObjContainer{ object.GetComponent< COMSkinnedMeshRender>() };
	auto skillCom{ object.GetComponent<COMSkills>() };
	//auto characterContoller{ object.GetComponent<PhysicsCharacterController>() };
	auto characterMesh{ renderObjContainer->GetMesh() };
	f32 const animSeek{ characterMesh->GetCurrentSeek() };
	f32 const animLength{ characterMesh->GetCurrentAnimSetLength() };
	f32 const ratio{ animSeek / animLength };
	auto inputMgr{ InputManager::Instance() };

	if (ratio >= 1.f)
	{
		if ((GetAsyncKeyState(VK_SPACE) & 0x8000) != 0)
		{
			SetState(PlayerBehaviorID::Jump);
			return;
		}
		else
		{
			SetState(PlayerBehaviorID::Normal);
			return;
		}
	}
	characterMesh->PlayAnimation(timedelta* skillCom->GetAttackSpeed());
	auto		transformCOM{ object.GetComponent<Game::TransformComponent>() };
	auto		weapon{ object.GetChild(L"WEAPON") };
	auto		collderCOM{ object.GetComponent< COMCollider>() };
	auto		weaponColliderCom{ weapon->GetComponent<COMCollider>() };
	auto&		weaponCollider{ weaponColliderCom->GetRef(L"WEAPON_COLLIDER") };
	auto		playerTransformMatrix{ f32x44{} };
	auto		frameWorldMatrix{ f32x44{} };
	auto		frameMatrixPtr{ characterMesh->FindFrameTransfromByName(weaponCollider.GetFrameName()) };
	auto		mFrameWorldTransform{ XMMATRIX{} };
	
	transformCOM->GenerateTransform(&playerTransformMatrix);
	mFrameWorldTransform = XMLoadFloat4x4(frameMatrixPtr) * XMLoadFloat4x4(&playerTransformMatrix);
	XMStoreFloat4x4(&frameWorldMatrix, mFrameWorldTransform);
	auto const& monsterList{ App::Instance()->GetScene().GetListRef(LAYER_MONSTER) };
	for (auto& monster : monsterList)
	{
		if (auto it = preventAttack_.find(monster.get()); it != preventAttack_.end())
			continue;
		auto monsterColliderCom{ monster->GetComponent<COMCollider>() };
		auto monsterMeshCom{ monster->GetComponent<COMSkinnedMeshRender>() };
		auto monsterTransform{ monster->GetComponent<Game::TransformComponent>() };
		if (monsterColliderCom == nullptr)
			continue;
		if (monsterMeshCom == nullptr)
			continue;
		auto const& mesh{ monsterMeshCom->GetMesh() };
		auto const& colliderTable{ monsterColliderCom->GetColliderTableRef() };
		auto		monsterTransformMatrix{ f32x44{} };
		auto		mMonsterTransform{ XMMATRIX{} };
		monsterTransform->GenerateTransform(&monsterTransformMatrix);
		mMonsterTransform = XMLoadFloat4x4(&monsterTransformMatrix);
		for (auto const& item : colliderTable)
		{
			auto const& collider{ item.second };
			auto		monsterFramePtr{ mesh->FindFrameTransfromByName(collider.GetFrameName()) };
			auto		mMonsterFrame{ XMLoadFloat4x4(monsterFramePtr) };
			auto		mFrameWorldTransform{ mMonsterFrame * mMonsterTransform };
			auto		frameWorldTransformMatrix{ f32x44{} };
			XMStoreFloat4x4(&frameWorldTransformMatrix, mFrameWorldTransform);
			if (weaponCollider.CheckCollision(frameWorldMatrix, collider, frameWorldTransformMatrix))
			{
				auto runtime{ Game::Runtime::Instance() };
				SoundManager::Instance().Play(SoundID::Attack);
				Damage damage{ L"Player", L"일반 공격", DamageType::Normal, 100 };
				runtime->Broadcast<DamageEvent>(*monster, Game::COM_ANY, EVT_Damage, damage);
				preventAttack_.emplace(monster.get());
				break;
			}
		}
	}
}

auto Kumazuma::Client::PlayerNormalAttackBehavior::Reset(f32 timedelta) -> void
{
	i32 const i{ 3 + rand() % 3 };
	std::wstringstream ss;
	ss << L"ATTACK_" << i;
	auto& obj{ GetObjectRef() };
	auto renderCom{ obj.GetComponent<COMSkinnedMeshRender>() };
	auto mesh{ renderCom->GetMesh() };
	auto& component{ GetComponentRef() };
	auto meta{ component.GetCharacterMeta() };
	mesh->SetAnimationSet(*meta->GetAnimIndex(ss.str()),false);
	preventAttack_.clear();
}

auto Kumazuma::Client::PlayerSkill1Behavior::Update(f32 timedelta) -> void
{
	auto& object{ GetObjectRef() };
	auto renderObjContainer{ object.GetComponent< COMSkinnedMeshRender>() };
	//auto characterContoller{ object.GetComponent<PhysicsCharacterController>() };
	auto characterMesh{ renderObjContainer->GetMesh() };
	f32 const animSeek{ characterMesh->GetCurrentSeek() };
	f32 const animLength{ characterMesh->GetCurrentAnimSetLength() };
	f32 const ratio{ animSeek / animLength };
	auto inputMgr{ InputManager::Instance() };

	if (ratio >= 1.f)
	{
		 //object.GetComponent<COMCharacterStateInfo>()
		auto skillCom{ object.GetComponent<COMSkills>() };
		skillCom->OnSkillBuff(BuffEvent{ EVT_Buff, L"IncreaseAttackSpeed" });

		if ((GetAsyncKeyState(VK_SPACE) & 0x8000) != 0)
		{
			SetState(PlayerBehaviorID::Jump);
			return;
		}
		else
		{
			SetState(PlayerBehaviorID::Normal);
			return;
		}
	}
	characterMesh->PlayAnimation(timedelta);
}

auto Kumazuma::Client::PlayerSkill1Behavior::Reset(f32 timedelta) -> void
{
	auto& obj{ GetObjectRef() };
	auto renderCom{ obj.GetComponent<COMSkinnedMeshRender>() };
	auto mesh{ renderCom->GetMesh() };
	auto& component{ GetComponentRef() };
	auto meta{ component.GetCharacterMeta() };
	mesh->SetAnimationSet(*meta->GetAnimIndex(L"ROAR"), false);
	auto skillCom{ obj.GetComponent<COMSkills>() };
	skillCom->OnSkillBuff(BuffEvent{ EVT_Buff, L"IncreaseAttackSpeed" });
	SoundManager::Instance().Play(SoundID::Roar);
}

Kumazuma::Client::PlayerSkill2Behavior::PlayerSkill2Behavior()
{
	fireCollider_.SetType(ColliderType::SPHERE);
	static_cast<SphereColliderAttribute&>(fireCollider_.GetAttributeRef()).SetRadius(80.f);
	effect_.reset(new FlameEffect(App::Instance()->GetRenderModule().get()));
	effect_->SetPlayLength(2.0f);
	effect_->SetSize(6.f);
}

auto Kumazuma::Client::PlayerSkill2Behavior::Update(f32 timedelta) -> void
{
	auto& object{ GetObjectRef() };
	auto renderObjContainer{ object.GetComponent< COMSkinnedMeshRender>() };
	auto skillCom{ object.GetComponent<COMSkills>() };
	auto transformCOM{ object.GetComponent<Game::TransformComponent>() };
	auto playerTransformMatrix{ f32x44{} };
	auto mPlayerTransform{ XMMATRIX{} };
	transformCOM->GenerateTransform(&playerTransformMatrix);
	mPlayerTransform = XMLoadFloat4x4(&playerTransformMatrix);
	//auto characterContoller{ object.GetComponent<PhysicsCharacterController>() };
	auto characterMesh{ renderObjContainer->GetMesh() };
	f32 const animSeek{ characterMesh->GetCurrentSeek() };
	f32 const animLength{ characterMesh->GetCurrentAnimSetLength() };
	f32 const ratio{ animSeek / animLength };
	auto inputMgr{ InputManager::Instance() };
	auto rendererCom{ object.GetComponent<COMRenderer>() };
	auto renderer{ rendererCom->GetRenderer() };

	for (auto& effect : effects_)
	{
		renderer->AddEffect(effect);
		effect->PlayEffect(timedelta);
	}
	if (ratio >= 1.f)
	{
		if ((GetAsyncKeyState(VK_SPACE) & 0x8000) != 0)
		{
			SetState(PlayerBehaviorID::Jump);
			return;
		}
		else
		{
			SetState(PlayerBehaviorID::Normal);
			return;
		}
	}
	characterMesh->PlayAnimation(timedelta);


	auto const& monsterList{ App::Instance()->GetScene().GetListRef(LAYER_MONSTER) };
	for (auto& monster : monsterList)
	{
		if (auto it = preventAttack_.find(monster.get()); it != preventAttack_.end())
			continue;
		auto monsterColliderCom{ monster->GetComponent<COMCollider>() };
		auto monsterMeshCom{ monster->GetComponent<COMSkinnedMeshRender>() };
		auto monsterTransform{ monster->GetComponent<Game::TransformComponent>() };
		if (monsterColliderCom == nullptr)
			continue;
		if (monsterMeshCom == nullptr)
			continue;
		auto const& mesh{ monsterMeshCom->GetMesh() };
		auto const& colliderTable{ monsterColliderCom->GetColliderTableRef() };
		auto		monsterTransformMatrix{ f32x44{} };
		auto		mMonsterTransform{ XMMATRIX{} };
		monsterTransform->GenerateTransform(&monsterTransformMatrix);
		mMonsterTransform = XMLoadFloat4x4(&monsterTransformMatrix);
		for (auto const& item : colliderTable)
		{
			auto const& collider{ item.second };
			auto		monsterFramePtr{ mesh->FindFrameTransfromByName(collider.GetFrameName()) };
			auto		mMonsterFrame{ XMLoadFloat4x4(monsterFramePtr) };
			auto		mFrameWorldTransform{ mMonsterFrame * mMonsterTransform };
			auto		frameWorldTransformMatrix{ f32x44{} };
			XMStoreFloat4x4(&frameWorldTransformMatrix, mFrameWorldTransform);
			if (fireCollider_.CheckCollision(playerTransformMatrix, collider, frameWorldTransformMatrix))
			{
				auto runtime{ Game::Runtime::Instance() };
				Damage damage{ L"Player", L"파이어 브레스", DamageType::Fire, 200 };
				runtime->Broadcast<DamageEvent>(*monster, Game::COM_ANY, EVT_Damage, damage);
				preventAttack_.emplace(monster.get());
				break;
			}
		}
	}
}

auto Kumazuma::Client::PlayerSkill2Behavior::Reset(f32 timedelta) -> void
{
	auto& obj{ GetObjectRef() };
	auto renderCom{ obj.GetComponent<COMSkinnedMeshRender>() };
	auto mesh{ renderCom->GetMesh() };
	auto& component{ GetComponentRef() };
	auto meta{ component.GetCharacterMeta() };
	mesh->SetAnimationSet(*meta->GetAnimIndex(L"BREATHFIRE"), false);
	preventAttack_.clear();
	effects_.clear();
	accumTime = 0.f;
	auto& object{ GetObjectRef() };
	auto transformCOM{ object.GetComponent<Game::TransformComponent>() };
	auto		playerTransformMatrix{ f32x44{} };
	auto mPlayerTransform{ XMMATRIX{} };
	transformCOM->GenerateTransform(&playerTransformMatrix);
	mPlayerTransform = XMLoadFloat4x4(&playerTransformMatrix);
	for (i32 i = 0; i < 128; ++i)
	{
		auto newEffectPosition{ transformCOM->GetPosition() };
		auto vQuanterion{ XMQuaternionRotationAxis(mPlayerTransform.r[1], XM_2PI * static_cast<f32>(rand()) / RAND_MAX) };
		auto mRot{ XMMatrixRotationQuaternion(vQuanterion) };
		f32 length{ 10.f * static_cast<f32>(rand()) / RAND_MAX };
		auto vEffectPosition{ XMLoadFloat3(&newEffectPosition) + XMVector3TransformCoord(XMVectorSet(0.f, 0.f, length, 0.f), mRot) };
		auto newEffect{ effect_->Clone() };
		XMStoreFloat3(&newEffectPosition, vEffectPosition);
		newEffect->SetCenter(newEffectPosition);
		newEffect->SetSeek(static_cast<f32>(rand()) / RAND_MAX);
		effects_.emplace_back(newEffect);
	}
	
}
