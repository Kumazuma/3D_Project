#include "PlayerJumpBehavior.hpp"
#include "PhysicsCharacterController.hpp"
#include "COMRenderObjectContainer.hpp"
#include "framework.h"
#include <game/TransformComponent.hpp>
#include "app.h"
constexpr wchar_t CHARACTER_MESH[]{ L"CHARACTER" };
using namespace DirectX;
_BEGIN_NS(Kumazuma::Client)
auto PlayerJumpBehavior::Update(f32 timedelta) -> void 
{
	auto& object{ GetObjectRef() };
	auto& component{ GetComponentRef() };
	auto meta{ component.GetCharacterMeta() };
	auto renderObjContainer{ object.GetComponent< COMRenderObjectContainer>() };
	auto characterContoller{ object.GetComponent<PhysicsCharacterController>() };
	auto characterMesh{ std::static_pointer_cast<SkinnedXMeshObject>(renderObjContainer->Get(CHARACTER_MESH)) };
	auto transformComponent{ object.GetComponent<Game::TransformComponent>() };
	auto inputMgr{ InputManager::Instance() };

	f32x44 transformMatrix{};
	characterMesh->PlayAnimation(timedelta);

	if (!characterContoller->IsJumping() && jumpAnimID_ != JumpAnimID::END)
	{
		jumpAnimID_ = JumpAnimID::END;
		characterMesh->SetAnimationSet(*meta->GetAnimIndex(L"JUMP_END"), false);
	}
	else
	{
		f32 const seek{ characterMesh->GetCurrentSeek() };
		f32 const length{ characterMesh->GetCurrentAnimSetLength() };
		f32 const ratio{ seek / length };
		if (ratio >= 1.f)
		{
			switch (jumpAnimID_)
			{
			case JumpAnimID::START:
				jumpAnimID_ = JumpAnimID::FLYING;
				characterMesh->SetAnimationSet(*meta->GetAnimIndex(L"JUMP"));
				break;
			case JumpAnimID::FLYING:
				//jumpAnimID_ = JumpAnimID::END;
				//characterMesh->SetAnimationSet(*meta->GetAnimIndex(L"JUMP_END"), false);
				break;
			case JumpAnimID::END:
				SetState(PlayerBehaviorID::Normal);
				return;
			}
		}
	}						 
	transformComponent->GenerateTransformMatrixWithoutScale(&transformMatrix);
	XMVECTOR vDelta{};
	XMMATRIX mTransform{ XMLoadFloat4x4(&transformMatrix) };
	XMVECTOR vForward{ mTransform.r[2] };
	XMVECTOR vRight{ mTransform.r[0] };
	XMVECTOR vPosition{ mTransform.r[3] };
	auto rotation{ transformComponent->GetRotation() };

	if (inputMgr->IsPressing(PLAYER_INPUT::MOVE_FORWARD))
	{
		vDelta += vForward;
	}
	if (inputMgr->IsPressing(PLAYER_INPUT::MOVE_BACKWARD))
	{
		vDelta -= vForward;
	}
	if (inputMgr->IsPressing(PLAYER_INPUT::MOUSE_RBUTTON))
	{
		auto& window{ App::Instance()->GetWindow() };

		auto rc{ window.GetRect() };
		POINT cursorPos{ inputMgr->GetMousePos() };
		ScreenToClient(window.GetHandle(), &cursorPos);
		f32x2 const xy{
			static_cast<f32>(cursorPos.x),
			static_cast<f32>(cursorPos.y)
		};
		f32x2 const size{
			static_cast<f32>(rc.right - rc.left),
			static_cast<f32>(rc.bottom - rc.top)
		};
		XMVECTOR vCurosrPos{ XMLoadFloat2(&xy) };
		XMVECTOR vScreenSize{ XMLoadFloat2(&size) };
		XMVECTOR vAxis{ XMVectorSet(1.f, 1.f, 0.f, 0.f) };
		//0 ~ 2.f
		vCurosrPos = vAxis * (vCurosrPos * 2.f) / vScreenSize;
		vCurosrPos -= vAxis;
		XMVECTOR vPrevCursor{ 0,0,0,0 };
		XMVECTOR vCursorDelta{ (vCurosrPos - vPrevCursor) * (XM_2PI / 4.f) };
		f32 length{};
		XMStoreFloat(&length, XMVector2Length(vCursorDelta));
		if (length > 0.f)
		{
			XMVECTOR vRot{ XMLoadFloat3(&transformComponent->GetRotation()) };
			vCursorDelta = XMVECTOR{ 0, vCursorDelta.m128_f32[0], 0.f, 0.f };
			vRot = XMVectorAddAngles(vRot, vCursorDelta);
			rotation = StoreF32x3(vRot);
			transformComponent->SetRotation(rotation);
			transformComponent->GenerateTransformMatrixWithoutScale(&transformMatrix);
			mTransform = XMLoadFloat4x4(&transformMatrix);
		}
		if (inputMgr->IsPressing(PLAYER_INPUT::MOVE_LEFT))
		{
			vDelta -= vRight;
		}
		if (inputMgr->IsPressing(PLAYER_INPUT::MOVE_RIGHT))
		{
			vDelta += vRight;
		}
	}
	else
	{
		if (inputMgr->IsPressing(PLAYER_INPUT::MOVE_LEFT))
		{
			rotation.y -= timedelta;
		}
		if (inputMgr->IsPressing(PLAYER_INPUT::MOVE_RIGHT))
		{
			rotation.y += timedelta;
		}
	}
	XMVECTOR vMovingDir{ XMVector3Normalize(vDelta) };
	f32 len{};
	XMStoreFloat(&len, XMVector3Length(vDelta));
	if (len > 0.f)
	{
		characterContoller->Move(vMovingDir * COMPlayerInput::PLAYER_WALK_SPEED * 0.5f);
	}
	else
	{
		characterContoller->Stop();
	}
	transformComponent->SetRotation(rotation);
	transformComponent->SetPosition(characterContoller->GetPosition());
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		auto const& pos{ transformComponent->GetPosition() };
		//sistream << "X: " << pos.x << ", Y: " << pos.y << ", Z: " << pos.z << "\n";
		//OutputDebugStringA(sistream.str().c_str());
	}
	transformComponent->GenerateTransformMatrix(&transformMatrix);
	characterMesh->SetTransform(transformMatrix);
}

auto PlayerJumpBehavior::Reset(f32 timedelta) -> void 
{
	jumpAnimID_ = JumpAnimID::START;
	auto& object{ GetObjectRef() };
	auto& component{ GetComponentRef() };
	auto meta{ component.GetCharacterMeta() };
	auto renderObjContainer{ object.GetComponent< COMRenderObjectContainer>() };
	auto characterContoller{ object.GetComponent<PhysicsCharacterController>() };
	auto characterMesh{ std::static_pointer_cast<SkinnedXMeshObject>(renderObjContainer->Get(CHARACTER_MESH)) };
	characterContoller->Jump(50.f);
	characterMesh->SetAnimationSet(*meta->GetAnimIndex(L"JUMP_START"), false);
}

_END_NS