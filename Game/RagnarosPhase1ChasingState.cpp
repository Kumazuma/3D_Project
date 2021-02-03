#include "RagnarosPhase1ChasingState.hpp"
#include "app.h"
#include "COMRenderObjectContainer.hpp"
#include <game/object.hpp>
#include "SkinnedXMeshObject.h"
#include "ResourceManager.hpp"
#include "CharacterMeta.hpp"
#include <game/TransformComponent.hpp>
#include "COMRenderObjectContainer.hpp"
#include "LayerTags.hpp"
#include <sstream>
#include "COMCollider.hpp"
#include "Collder.hpp"
#include "BoxColliderAttribute.hpp"
#include "SimpleColliderBoxObject.h"
#include "COMRagnarosAI.hpp"
#include "COMPlayerRender.hpp"
#include "PhysicsCharacterController.hpp"
constexpr wchar_t CHARACTER_MESH[]{ L"CHARACTER" };
using namespace DirectX;

auto Kumazuma::Client::RagnarosPhase1ChasingState::Update(f32 timeDelta) -> void
{
	auto const& r{ App::Instance()->GetScene().GetListRef(LAYER_PLAYER) };
	auto transform{ GetObjectRef().GetComponent<Game::TransformComponent>() };
	
	auto renderCom{ GetObjectRef().GetComponent<COMSkinnedMeshRender>() };
	auto characterMesh{ renderCom->GetMesh() };

	if (characterMesh)
	{
		f32x44 transformMatrix{};
		transform->GenerateTransformMatrix(&transformMatrix);
		//characterMesh->SetTransform(transformMatrix);
		//TODO
		characterMesh->PlayAnimation(timeDelta);
	}
	
	
	auto player{ *r.begin() };

	if (player == nullptr)
	{
		return;
	}

	auto playerTransfromComponent{ player->GetComponent<Game::TransformComponent>() };
	auto myTransfromComponent{ GetObjectRef().GetComponent<Game::TransformComponent>() };
	auto playerPosition{ playerTransfromComponent->GetPosition() };
	auto vTargetPos{ LoadF32x3(playerPosition) };
	auto myPosition{ myTransfromComponent->GetPosition() };
	myPosition.y -= 9.f;

	auto const distance{ CalculateDistanceToTarget(myPosition, playerPosition) };
	f32x2 angle{};
	CalculateXYAngleToTarget(myPosition, playerPosition, &angle);

	if (abs(angle.x) < RAGNAROS_ANGLE_X) //거의 평지에 있을 때
	{
		auto rot{ transform->GetRotation() };
		rot.y = angle.y;
		if (distance < 10.f)
		{
			transform->SetRotation(rot);
			SetState(RagnarosAIState::STATE_PHASE1_ATTACK);
		}
		else if (distance < 50.f) //50 미만이고
		{
			transform->SetRotation(rot);
			auto mRot{ XMMatrixRotationRollPitchYawFromVector(LoadF32x3(rot)) };
			auto vForward{ mRot.r[2] };
			auto characterContoller{ GetObjectRef().GetComponent<PhysicsCharacterController>() };
			characterContoller->Move(RAGNAROS_WALK_SPEED * vForward);
			//std::stringstream ss;
			//ss << "angle: " << XMConvertToDegrees(angleX) << " dx: " << delta.x << " dy: " << delta.y << " dz: " << delta.z << '\n';
			//OutputDebugStringA(ss.str().c_str());
		}
		else
		{
			SetState(RagnarosAIState::STATE_PHASE1_STANCE);
		}
	}
	else
	{
		SetState(RagnarosAIState::STATE_PHASE1_STANCE);
	}

	
}

auto Kumazuma::Client::RagnarosPhase1ChasingState::Reset() -> void 
{
	auto& meta{ GetComponentRef().GetCharacterMetaRef() };
	auto renderModule{ App::Instance()->GetRenderModule() };
	auto renderCom{ GetObjectRef().GetComponent<COMSkinnedMeshRender>() };
	auto characterMesh{ renderCom->GetMesh() };
	auto animIndex{ meta.GetAnimIndex(L"WALK") };
	if (animIndex.has_value())
	{
		characterMesh->SetAnimationSet(static_cast<u32>(*animIndex), true);
	}
	auto weapon{ GetObjectRef().GetChild(L"WEAPON") };
	
	auto comCollider{ weapon->GetComponent<COMCollider>() };
	comCollider->Remove(L"ARM");
	Collider collider;
	collider.SetFrameName(L"creature_ragnaros2_ragnaros2_bone_81");
	collider.SetType(ColliderType::BOX);
	auto& boxColliderAttr{ static_cast<BoxColliderAttribute&>(collider.GetAttributeRef()) };
	boxColliderAttr.SetDepth(2000.f);
	boxColliderAttr.SetHeight(750.f);
	boxColliderAttr.SetWidth(750.f);

	collider.SetOffset(f32x3{ 500.f, -100.f, 1500.f});
	comCollider->Add(L"ARM", collider);
}
