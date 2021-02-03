#include "RagnarosWaitPlayerState.hpp"
#include "app.h"
#include "LayerTags.hpp"
#include <game/object.hpp>
#include<game/TransformComponent.hpp>
#include"COMRenderObjectContainer.hpp"
#include"COMPlayerRender.hpp"
using namespace DirectX;
constexpr wchar_t CHARACTER_MESH[]{ L"CHARACTER" };

auto Kumazuma::Client::RagnarosWaitPlayerState::Update(f32) -> void 
{
	auto const& r{ App::Instance()->GetScene().GetListRef(LAYER_PLAYER) };
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
	auto vCurPos{ LoadF32x3(myPosition) };
	auto vDelta{ vTargetPos - vCurPos };
	auto vDir{ XMVector3Normalize(vDelta) };
	auto distance{ StoreF32(XMVector3Length(vDelta)) };
	auto delta{ StoreF32x3(vDelta) };
	if (distance <= 50.f) //50 미만이고
	{
		auto directToTarget{ StoreF32x3(vDir) };
		auto vDirectionXZ{ XMVector3Normalize(XMVectorSetY(vDelta, 0.f)) };
		f32 angleY{ atan2f(directToTarget.x ,directToTarget.z) };
		f32 angleX{ StoreF32(XMVector3AngleBetweenNormals(vDirectionXZ, vDir)) };
		if (abs(angleX) < RAGNAROS_ANGLE_X) //거의 평지에 있을 때
		{
			SetState(RagnarosAIState::STATE_BIRTH);
		}
	}

}

auto Kumazuma::Client::RagnarosWaitPlayerState::Reset() -> void 
{
	auto renderCom{ GetObjectRef().GetComponent<COMSkinnedMeshRender>() };
	renderCom->SetVisible(false);
}
