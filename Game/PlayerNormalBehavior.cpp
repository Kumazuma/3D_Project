#include "PlayerNormalBehavior.hpp"
#include "framework.h"
#include <game/TransformComponent.hpp>
#include "COMRenderObjectContainer.hpp"
#include "SkinnedXMeshObject.h"
#include "PhysicsCharacterController.hpp"

#include "app.h"
#include <sstream>
constexpr wchar_t CHARACTER_MESH[]{ L"CHARACTER" };
using namespace DirectX;
_BEGIN_NS(Kumazuma::Client)
auto PlayerNormalBehavior::Update(f32 timedelta) -> void
{
	auto& object{ GetObjectRef() };
	auto& component{ GetComponentRef() };
	auto transform = object.GetComponent<Game::TransformComponent>();
	auto renderObjContainer{ object.GetComponent<COMRenderObjectContainer>() };
	auto skinnedMesh{ std::static_pointer_cast<SkinnedXMeshObject>(renderObjContainer->Get(CHARACTER_MESH) )};
	auto inputMgr{ InputManager::Instance() };

	XMFLOAT4X4 transformMatrix;
	transform->GenerateTransformMatrixWithoutScale(&transformMatrix);

	XMMATRIX mTransform{ XMLoadFloat4x4(&transformMatrix) };
	XMVECTOR vForward{ mTransform.r[2] };
	XMVECTOR vRight{ mTransform.r[0] };
	XMVECTOR vDelta{};
	XMVECTOR vPosition{ mTransform.r[3] };
	auto rotation{ transform->GetRotation() };
	auto characterContoller{ object.GetComponent<PhysicsCharacterController>() };
	if ((GetAsyncKeyState(VK_SPACE) & 0x8000) != 0)
	{
		SetState(PlayerBehaviorID::Jump);
		return;
	}

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
			XMVECTOR vRot{ XMLoadFloat3(&transform->GetRotation()) };
			vCursorDelta = XMVECTOR{ 0, vCursorDelta.m128_f32[0], 0.f, 0.f };
			vRot = XMVectorAddAngles(vRot, vCursorDelta);
			rotation = StoreF32x3(vRot);
			transform->SetRotation(rotation);
			transform->GenerateTransformMatrixWithoutScale(&transformMatrix);
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
	auto characterMeta{ component.GetCharacterMeta() };
	f32 len{};
	XMStoreFloat(&len, XMVector3Length(vDelta));
	if (len > 0.f)
	{
		characterContoller->Move(vMovingDir * COMPlayerInput::PLAYER_WALK_SPEED);
		skinnedMesh->SetAnimationSet(*characterMeta->GetAnimIndex(L"WALK"));
	}
	else
	{
		characterContoller->Stop();
		skinnedMesh->SetAnimationSet(*characterMeta->GetAnimIndex(L"STANCE"));
	}
	transform->SetRotation(rotation);
	transform->SetPosition(characterContoller->GetPosition());
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		std::stringstream sistream;
		auto const& pos{ transform->GetPosition() };
		sistream << "X: " << pos.x << ", Y: " << pos.y << ", Z: " << pos.z << "\n";
		OutputDebugStringA(sistream.str().c_str());
	}
	transform->GenerateTransformMatrix(&transformMatrix);
	skinnedMesh->SetTransform(transformMatrix);
	skinnedMesh->PlayAnimation(timedelta);
}

auto PlayerNormalBehavior::Reset(f32 timedelta) -> void
{

}
_END_NS 
