#include "COMRagnarosAI.hpp"
#include <DirectXMath.h>
#include <game/runtime.hpp>
#include <game/object.hpp>
#include <game/TransformComponent.hpp>
#include "COMRenderObjectContainer.hpp"
#include "COMRenderer.hpp"
#include <SkinnedXMeshObject.h>
#include "RagnarosAIState.hpp"
#include "RagnarosBirthState.hpp"
#include "RagnarosWaitPlayerState.hpp"
#include "RagnarosPhase1ChasingState.hpp"
#include "RagnarosPhase1AttackState.hpp"
#include "RagnarosPhase1Stance.hpp"
#include "PhysicsCharacterController.hpp"
#include "ResourceManager.hpp"
#include "COMPlayerRender.hpp"
#include "EventTag.hpp"
#include "COMCharacterStateInfo.hpp"
#include "COMRagnarosDead.hpp"
#include "LayerTags.hpp"
#include "CharacterMeta.hpp"
#include "app.h"
constexpr wchar_t CHARACTER_MESH[]{ L"CHARACTER" };
namespace Kumazuma::Client
{
	using namespace DirectX;
	IMPLEMENT_COMPONENT_CLASS(Kumazuma::Client::COMRagnarosAI)

	COMRagnarosAI::COMRagnarosAI():
		Component(TAG),
		currentStateID_{},
		currentState_{},
		nextStateID_{},
		nextState_{}
	{
		Bind(Game::EVT_UPDATE, &COMRagnarosAI::Update);
		Bind(EVT_Damage, &COMRagnarosAI::OnDamaged);
		auto resourceManager{ ResourceManager::Instance() };
		auto meta{ resourceManager->GetCharacterMeta(L"ragnaros") };
		characterMeta_ = meta;
		aiStates_.emplace(RagnarosAIState::STATE_WAIT_PLAYER, new RagnarosWaitPlayerState{});
		aiStates_.emplace(RagnarosAIState::STATE_BIRTH, new RagnarosBirthState{});
		aiStates_.emplace(RagnarosAIState::STATE_PHASE1_CHASING, new RagnarosPhase1ChasingState{});
		aiStates_.emplace(RagnarosAIState::STATE_PHASE1_ATTACK, new RagnarosPhase1AttackState{});
		aiStates_.emplace(RagnarosAIState::STATE_PHASE1_STANCE, new RagnarosPhase1StanceState{});

		SetState(RagnarosAIState::STATE_WAIT_PLAYER);
	}

	auto COMRagnarosAI::Clone() const -> Game::Component*
	{
		return nullptr;
	}

	auto COMRagnarosAI::Update(Game::UpdateEvent const& event) -> void
	{
		if (auto obj{ GetObj().lock() }; obj != nullptr)
		{
			if (this->currentState_ != nullptr)
			{
				this->currentState_->Update(this, obj.get(), event.GetDelta());
			}
			if (this->nextState_ != nullptr)
			{
				this->currentStateID_ = std::move(this->nextStateID_);
				this->currentState_ = this->nextState_;
				this->nextState_ = nullptr;
				if (this->currentState_ != nullptr)
				{
					this->currentState_->Reset(this, obj.get());
				}
			}
			auto characterContoller{ obj->GetComponent<PhysicsCharacterController>() };
			auto transform = obj->GetComponent<Game::TransformComponent>();
			if (characterContoller != nullptr)
			{
				transform->SetPosition(characterContoller->GetPosition());
			}
		}
	}
	auto COMRagnarosAI::SetState(u64 state) -> void
	{
		auto const& aiStates{ this->aiStates_ };
		if (auto const it = aiStates.find(state); it != aiStates.end())
		{
			nextStateID_ = state;
			nextState_ = it->second.get();
		}
	}
	auto COMRagnarosAI::GetState() const -> u64
	{
		return nextStateID_;
	}
	auto COMRagnarosAI::GetCharacterMetaRef() const -> CharacterMeta const&
	{
		return *characterMeta_;
	}
	auto COMRagnarosAI::OnDamaged(DamageEvent const& evt) -> void
	{
		auto const& damage{ evt.GetDamage() };
		if (currentStateID_ == RagnarosAIState::STATE_WAIT_PLAYER ||
			currentStateID_ == RagnarosAIState::STATE_BIRTH
			)
		{
			return;
		}

		if (auto object = GetObj().lock(); object != nullptr)
		{
			auto const& damage{ evt.GetDamage() };
			auto state{ object->GetComponent<COMCharacterStateInfo>() };
			if (state == nullptr) return;
			auto& hitPoints{ state->GetHitPoints() };
			hitPoints.Damage(damage.amount);
			auto renderCom{ object->GetComponent< COMSkinnedMeshRender >() };
			auto transformCom{ object->GetComponent<Game::TransformComponent>() };
			if (hitPoints.GetCurrent() <= 0)
			{
				SoundManager::Instance().Play(SoundID::RagnarosDeath);
				auto& scene{ App::Instance()->GetScene() };
				auto deadObject{ std::make_shared<Game::Object>() };
				auto mesh{std::shared_ptr<SkinnedXMeshObject>(  static_cast<SkinnedXMeshObject*>(renderCom->GetMesh()->Clone()) )};
				auto rendererCom{ object->GetComponent<COMRenderer>() };
				deadObject->AddComponent<COMSkinnedMeshRender>(renderCom->GetRenderer(), mesh);
				deadObject->AddComponent<Game::TransformComponent>();
				deadObject->AddComponent<COMRagnarosDeadController>();
				deadObject->AddComponent<COMRenderer>(rendererCom->GetRenderer());
				auto deadObjTransform{ deadObject->GetComponent<Game::TransformComponent>() };
				deadObjTransform->SetPosition(transformCom->GetPosition());
				deadObjTransform->SetRotation(transformCom->GetRotation());
				deadObjTransform->SetScale(transformCom->GetScale());
				mesh->SetAnimationSet(*characterMeta_->GetAnimIndex(L"DEATH"),false);

				scene.RemoveObject(LAYER_MONSTER, *object);
				scene.AddObject(LAYER_ETC, deadObject);
			}
		}
	}
}
