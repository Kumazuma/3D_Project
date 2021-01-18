#include "RagnarosPhase1Stance.hpp"
#include "COMRagnarosAI.hpp"
#include "app.h"
#include "COMRenderObjectContainer.hpp"
#include "CharacterMeta.hpp"
#include <SkinnedXMeshObject.h>
#include "LayerTags.hpp"
#include "COMCollider.hpp"
#include <game/TransformComponent.hpp>
using namespace DirectX;
constexpr wchar_t CHARACTER_MESH[]{ L"CHARACTER" };

auto Kumazuma::Client::RagnarosPhase1StanceState::Update(f32 timeDelta) -> void 
{
	auto& object{ GetObjectRef() };
	auto container{ object.GetComponent<COMRenderObjectContainer>() };
	auto colliderContainer{ object.GetComponent<COMCollider>() };


	auto characterMesh{ std::static_pointer_cast<SkinnedXMeshObject>(container->Get(CHARACTER_MESH)) };
	characterMesh->PlayAnimation(timeDelta);
	auto armMesh{ container->Get(L"ARM") };
	auto collider{ colliderContainer->Get(L"ARM") };
	auto framePtr{ characterMesh->FindFrameTransfromByName(collider->GetFrameName()) };
	auto transform{ GetObjectRef().GetComponent<Game::TransformComponent>() };

	if (framePtr != nullptr)
	{
		f32x44 transformMatrix{ };
		f32x44 frameRenderMatrix{};
		transform->GenerateTransformMatrix(&transformMatrix);
		XMMATRIX mWeaponTransform{};
		XMMATRIX mCharacterTransfrom{};
		XMMATRIX mArmFrame{};
		XMMATRIX mFrameRenderTransform{};
		mArmFrame = XMLoadFloat4x4(framePtr);
		mCharacterTransfrom = XMLoadFloat4x4(&transformMatrix);
		mFrameRenderTransform = mArmFrame * mCharacterTransfrom;
		XMStoreFloat4x4(&transformMatrix, mFrameRenderTransform);
		SetARMColliderTransform(transformMatrix);
		auto mScale{ XMMatrixScaling(2000.f, 2000.f, 2000.f) };
		auto mRotation{ XMMatrixRotationRollPitchYaw(XM_PI,0.f, XM_PI * 0.5f) };
		auto mOffset{ XMMatrixTranslation(290.f, -200.f, 1000.f) };
		auto renderTransform{ StoreF32x44(mScale * mRotation * mOffset * mFrameRenderTransform) };
		armMesh->SetTransform(renderTransform);
	}

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
	auto container{ GetObjectRef().GetComponent<COMRenderObjectContainer>() };
	auto characterMesh{ std::static_pointer_cast<SkinnedXMeshObject>(container->Get(CHARACTER_MESH)) };
	auto animIndex{ meta.GetAnimIndex(L"STANCE") };
	if (animIndex.has_value())
	{
		characterMesh->SetAnimationSet(static_cast<u32>(*animIndex), true);
	}
}
