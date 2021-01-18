#include "RagnarosBirthState.hpp"
#include "COMRenderObjectContainer.hpp"
#include <game/object.hpp>
#include "SkinnedXMeshObject.h"
#include "ResourceManager.hpp"
#include "CharacterMeta.hpp"
#include <game/TransformComponent.hpp>
#include "COMRenderObjectContainer.hpp"
#include "PhysicsManager.hpp"
constexpr wchar_t CHARACTER_MESH[]{ L"CHARACTER" };
using namespace DirectX;
auto Kumazuma::Client::RagnarosBirthState::Update(f32 timeDelta) -> void
{
	auto container{ GetObjectRef().GetComponent<COMRenderObjectContainer>() };
	auto characterMesh{ std::static_pointer_cast<SkinnedXMeshObject>(container->Get(CHARACTER_MESH)) };

	if (characterMesh)
	{
		auto const seek{ characterMesh->GetCurrentSeek() };
		auto const length{ characterMesh->GetCurrentAnimSetLength() };
		f32x44 transformMatrix{};
		auto transform = GetObjectRef().GetComponent<Game::TransformComponent>();
		transform->GenerateTransformMatrix(&transformMatrix);
		auto mTransform{ LoadF32x44(transformMatrix) * XMMatrixTranslation(0.f, 10.f, 0.f) };
		transformMatrix = StoreF32x44(mTransform);
		characterMesh->SetTransform(transformMatrix);
		characterMesh->PlayAnimation(timeDelta);
		if (seek >= length - 0.05f)
		{
			auto position{ transform->GetPosition() };
			auto physicsManager{ PhysicsManager::Instance() };
			auto physicsComponent = physicsManager->CreateCharacterController(6.0f, 8.0f, position, f32x3{ 0.f, 0.0f, 0.f });
			auto armMesh{ container->Get(L"ARM") };
			armMesh->SetVisible(true);
			GetObjectRef().AddComponent<PhysicsCharacterController>(std::move(*physicsComponent));
			SetState(RagnarosAIState::STATE_PHASE1_CHASING);
		}
	}
}

auto Kumazuma::Client::RagnarosBirthState::Reset() -> void
{
	auto resourceManager{ ResourceManager::Instance() };
	auto meta{ resourceManager->GetCharacterMeta(L"ragnaros") };
	auto container{ GetObjectRef().GetComponent<COMRenderObjectContainer>() };
	auto characterMesh{ std::static_pointer_cast<SkinnedXMeshObject>(container->Get(CHARACTER_MESH)) };
	characterMesh->SetVisible(true);
	auto animIndex{ meta->GetAnimIndex(L"BIRTH") };
	if (animIndex.has_value())
	{
		characterMesh->SetAnimationSet(static_cast<u32>( *animIndex), false);
	}
}
