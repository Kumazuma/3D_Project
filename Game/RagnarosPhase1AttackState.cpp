#include "RagnarosPhase1AttackState.hpp"
#include "app.h"
#include "LayerTags.hpp"
#include <game/object.hpp>
#include<game/TransformComponent.hpp>
#include"COMRagnarosAI.hpp"
#include"COMRenderObjectContainer.hpp"
#include"SkinnedXMeshObject.h"
#include"COMCollider.hpp"
#include"CharacterMeta.hpp"
#include<game/scene.hpp>
#include"EventTag.hpp"
#include"COMPlayerRender.hpp"
constexpr wchar_t CHARACTER_MESH[]{ L"CHARACTER" };

using namespace DirectX;
auto Kumazuma::Client::RagnarosPhase1AttackState::Update(f32 timeDelta) -> void 
{
	auto& object{ GetObjectRef() };
	auto renderComponent{ object.GetComponent< COMSkinnedMeshRender>() };
	auto weapon{ object.GetChild(L"WEAPON") };

	auto colliderContainer{ weapon->GetComponent<COMCollider>()};
	auto transform{ object.GetComponent<Game::TransformComponent>() };
	auto characterMesh{ renderComponent->GetMesh() };
	auto collider{ colliderContainer->Get(L"ARM") };
	auto framePtr{ characterMesh->FindFrameTransfromByName(collider->GetFrameName()) };
	auto const& r{ App::Instance()->GetScene().GetListRef(LAYER_PLAYER) };
	std::shared_ptr<Game::Object> player{};
	std::shared_ptr<COMCollider> playerComCollider{  };
	std::shared_ptr<Game::TransformComponent>  playerComTransform{};
	std::shared_ptr<COMSkinnedMeshRender> playerRenderCOM{};
	if (r.empty() == false)
	{
		player = *r.begin();
		playerComCollider = player->GetComponent<COMCollider>();
		playerComTransform = player->GetComponent<Game::TransformComponent>();
		playerRenderCOM = player->GetComponent<COMSkinnedMeshRender>();
	}

	characterMesh->PlayAnimation(timeDelta);
	f32 animSeek{ characterMesh->GetCurrentSeek() };
	f32 animLength{ characterMesh->GetCurrentAnimSetLength() };
	f32 const ratio{ animSeek / animLength };
	if (ratio >= 1.f)
	{
		std::optional<u64> nextState;
		if (playerComTransform != nullptr)
		{
			auto playerPosition{ playerComTransform->GetPosition() };
			auto vTargetPos{ LoadF32x3(playerPosition) };
			auto myPosition{ transform->GetPosition() };
			myPosition.y -= 9.f;
			auto vCurPos{ LoadF32x3(myPosition) };

			auto vDelta{ vTargetPos - vCurPos };
			auto vDir{ XMVector3Normalize(vDelta) };
			auto distance{ StoreF32(XMVector3Length(vDelta)) };
			auto delta{ StoreF32x3(vDelta) };
			auto directToTarget{ StoreF32x3(vDir) };
			auto vDirectionXZ{ XMVector3Normalize(XMVectorSetY(vDelta, 0.f)) };
			f32 angleY{ atan2f(directToTarget.x ,directToTarget.z) };
			f32 angleX{ StoreF32(XMVector3AngleBetweenNormals(vDirectionXZ, vDir)) };
			if (abs(angleX) < RAGNAROS_ANGLE_X) //거의 평지에 있을 때
			{
				auto rot{ transform->GetRotation() };
				rot.y = angleY;
				if (distance < 10.f)
				{
					transform->SetRotation(rot);
					nextState = RagnarosAIState::STATE_PHASE1_ATTACK;

				}
				else if (distance < 50.f) //50 미만이고
				{
					nextState = RagnarosAIState::STATE_PHASE1_CHASING;
				}
				transform->SetRotation(rot);
			}
		}
		
		if (nextState)
		{
			SetState(*nextState);
		}
		else
		{
			SetState(RagnarosAIState::STATE_PHASE1_STANCE);
		}
		return;
	}
	//
	if (framePtr == nullptr)
	{
		return;
	}
	f32x44 transformMatrix{ };
	f32x44 frameRenderMatrix{};
	transform->GenerateTransform(&transformMatrix);
	XMMATRIX mWeaponTransform{};
	XMMATRIX mCharacterTransfrom{};
	XMMATRIX mArmFrame{};
	XMMATRIX mFrameRenderTransform{};
	mArmFrame = XMLoadFloat4x4(framePtr);
	mCharacterTransfrom = XMLoadFloat4x4(&transformMatrix);
	mFrameRenderTransform = mArmFrame * mCharacterTransfrom;
	XMStoreFloat4x4(&frameRenderMatrix, mFrameRenderTransform);
	//SetARMColliderTransform(frameRenderMatrix);

	if (player == nullptr)
	{
		return;
	}
	auto& playerCollidersRef{ playerComCollider->GetColliderTableRef() };
	auto playerCharacterMesh{ playerRenderCOM->GetMesh() };
	f32x44 playerTransformMatrix{};
	XMMATRIX mPlayerWorldTransform{};
	playerComTransform->GenerateTransformMatrix(&playerTransformMatrix);
	mPlayerWorldTransform = XMLoadFloat4x4(&playerTransformMatrix);
	for (auto const& item : playerCollidersRef)
	{
		auto const& playerCollider{ item.second };
		auto playerColliderFrameName{ playerCollider.GetFrameName() };
		auto playerColliderFramePtr{ playerCharacterMesh->FindFrameTransfromByName(playerColliderFrameName) };
		auto mPlayerFrame{ XMMatrixIdentity() };
		f32x44 playerColliderTransform{};
		if (playerColliderFramePtr != nullptr)
		{
			mPlayerFrame = XMLoadFloat4x4(playerColliderFramePtr);
		}
		XMStoreFloat4x4(&playerColliderTransform, mPlayerFrame * mPlayerWorldTransform);
		if (collider->CheckCollision(frameRenderMatrix, playerCollider, playerColliderTransform))
		{
			if (auto it = objects_.find(player.get()); it == objects_.end())
			{
				objects_.emplace(player.get());
				auto runtime{ Game::Runtime::Instance() };
				Damage damage{L"Ragnaros", L"일반 공격", DamageType::Normal, 100};
				SoundManager::Instance().Play(SoundID::RagnarosAttack);

				runtime->Broadcast<DamageEvent>(*player, Game::COM_ANY, EVT_Damage, damage);
			}
			//TODO: 플레이어에게 데미지 이벤트보내기.
		}
	}
}

auto Kumazuma::Client::RagnarosPhase1AttackState::Reset() -> void 
{
	objects_.clear();
	auto& object{ GetObjectRef() };
	auto& meta{ GetComponentRef().GetCharacterMetaRef() };
	SoundManager::Instance().Play(SoundID::Ragnaros);

	if (auto index{ meta.GetAnimIndex(L"ATTACK2") }; index.has_value())
	{
		auto renderCom{ object.GetComponent<COMSkinnedMeshRender>() };
		auto mesh{ renderCom->GetMesh() };
		mesh->SetAnimationSet(*index, false);
	}
}
