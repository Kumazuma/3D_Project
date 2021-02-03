#include "RagnarosBirthState.hpp"
#include "COMRenderObjectContainer.hpp"
#include <game/object.hpp>
#include "SkinnedXMeshObject.h"
#include "ResourceManager.hpp"
#include "CharacterMeta.hpp"
#include <game/TransformComponent.hpp>
#include "COMRenderObjectContainer.hpp"
#include "PhysicsManager.hpp"
#include "COMPlayerRender.hpp"
#include "COMWeaponRender.hpp"
constexpr wchar_t CHARACTER_MESH[]{ L"CHARACTER" };
using namespace DirectX;
auto Kumazuma::Client::RagnarosBirthState::Update(f32 timeDelta) -> void
{
	auto renderCom{ GetObjectRef().GetComponent<COMSkinnedMeshRender>() };
	auto characterMesh{ renderCom->GetMesh() };

	if (characterMesh)
	{
		auto const seek{ characterMesh->GetCurrentSeek() };
		auto const length{ characterMesh->GetCurrentAnimSetLength() };
		f32x44 transformMatrix{};
		auto transform = GetObjectRef().GetComponent<Game::TransformComponent>();

		characterMesh->PlayAnimation(timeDelta);
		if (seek >= length - 0.05f)
		{
			auto position{ transform->GetPosition() };
			position.y -= 7.5f;

			auto physicsManager{ PhysicsManager::Instance() };
			auto physicsComponent = physicsManager->CreateCharacterController(6.0f, 8.0f, position, f32x3{ 0.f, 0.0f, 0.f });
			auto weapon{ GetObjectRef().GetChild(L"WEAPON") };
			auto weaponRenderCom{ weapon->GetComponent<COMWeaponRender>() };
			weaponRenderCom->SetVisible(true);

			GetObjectRef().AddComponent<PhysicsCharacterController>(std::move(*physicsComponent));
			SetState(RagnarosAIState::STATE_PHASE1_CHASING);
		}
	}
}

auto Kumazuma::Client::RagnarosBirthState::Reset() -> void
{
	auto resourceManager{ ResourceManager::Instance() };
	auto meta{ resourceManager->GetCharacterMeta(L"ragnaros") };
	auto renderCom{ GetObjectRef().GetComponent<COMSkinnedMeshRender>() };
	auto characterMesh{ renderCom->GetMesh() };
	renderCom->SetVisible(true);
	auto animIndex{ meta->GetAnimIndex(L"BIRTH") };
	auto transform{ GetObjectRef().GetComponent<Game::TransformComponent>() };
	auto position{ transform->GetPosition() };
	position.y += 7.5f;
	transform->SetPosition(position);
	if (animIndex.has_value())
	{
		characterMesh->SetAnimationSet(static_cast<u32>( *animIndex), false);
	}
}
