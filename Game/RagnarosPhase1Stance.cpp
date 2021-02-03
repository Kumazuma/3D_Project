#include "RagnarosPhase1Stance.hpp"
#include "COMRagnarosAI.hpp"
#include "app.h"
#include "COMRenderObjectContainer.hpp"
#include "CharacterMeta.hpp"
#include <SkinnedXMeshObject.h>
#include "LayerTags.hpp"
#include "COMCollider.hpp"
#include <game/TransformComponent.hpp>
#include "COMPlayerRender.hpp"
using namespace DirectX;
constexpr wchar_t CHARACTER_MESH[]{ L"CHARACTER" };

auto Kumazuma::Client::RagnarosPhase1StanceState::Update(f32 timeDelta) -> void 
{
	auto& object{ GetObjectRef() };
	auto renderCOM{ object.GetComponent<COMSkinnedMeshRender>() };
	auto colliderContainer{ object.GetComponent<COMCollider>() };
	auto weapon{ object.GetChild(L"WEAPON") };

	auto characterMesh{ renderCOM->GetMesh() };
	characterMesh->PlayAnimation(timeDelta);
	auto transform{ GetObjectRef().GetComponent<Game::TransformComponent>() };

	auto const& r{ App::Instance()->GetScene().GetListRef(LAYER_PLAYER) };
	auto player{ *r.begin() };
	if (player == nullptr)
	{
		return;
	}
	auto playerTransfromComponent{ player->GetComponent<Game::TransformComponent>() };
	auto playerPosition{ playerTransfromComponent->GetPosition() };
	auto myPosition{ transform->GetPosition() };
	myPosition.y -= 9.f;
	f32x2 angle{};
	CalculateXYAngleToTarget(myPosition, playerPosition, &angle);
	if (abs(angle.x) < RAGNAROS_ANGLE_X) //거의 평지에 있을 때
	{
		auto rot{ transform->GetRotation() };
		rot.y = angle.y;
		auto const distance{ CalculateDistanceToTarget(myPosition, playerPosition) };
		if (distance < 10.f)
		{
			SetState(RagnarosAIState::STATE_PHASE1_ATTACK);
		}
		else if (distance < 50.f) //50 미만이고
		{
			SetState(RagnarosAIState::STATE_PHASE1_CHASING);
		}
	}
}

auto Kumazuma::Client::RagnarosPhase1StanceState::Reset() -> void 
{
	auto& meta{ GetComponentRef().GetCharacterMetaRef() };
	auto renderModule{ App::Instance()->GetRenderModule() };
	auto renderComponent{ GetObjectRef().GetComponent<COMSkinnedMeshRender>() };
	auto characterMesh{ renderComponent->GetMesh() };
	auto animIndex{ meta.GetAnimIndex(L"STANCE") };
	if (animIndex.has_value())
	{
		characterMesh->SetAnimationSet(static_cast<u32>(*animIndex), true);
	}
}
