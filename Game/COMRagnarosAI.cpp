#include "COMRagnarosAI.hpp"
#include <DirectXMath.h>
#include <game/runtime.hpp>
#include <game/object.hpp>
#include <game/TransformComponent.hpp>
#include "COMRenderObjectContainer.hpp"
#include <SkinnedXMeshObject.h>
#include "RagnarosAIState.hpp"
#include "RagnarosBirthState.hpp"
#include "RagnarosWaitPlayerState.hpp"
#include "RagnarosPhase1ChasingState.hpp"
#include "RagnarosPhase1AttackState.hpp"
#include "RagnarosPhase1Stance.hpp"
#include "PhysicsCharacterController.hpp"
#include "ResourceManager.hpp"
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
			auto renderObjContainer{ obj->GetComponent<COMRenderObjectContainer>() };
			auto renderObj{ renderObjContainer->Get(CHARACTER_MESH) };
			if (!renderObj)return;

			auto characterContoller{ obj->GetComponent<PhysicsCharacterController>() };
			auto transform = obj->GetComponent<Game::TransformComponent>();
			if (characterContoller != nullptr)
			{
				transform->SetPosition(characterContoller->GetPosition());
				XMFLOAT4X4 transformMatrix;
				transform->GenerateTransformMatrix(&transformMatrix);
				renderObj->SetTransform(transformMatrix);
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
}
