#include "FreeViewingState.hpp"
#include "app.h"
#include "framework.h"
using namespace DirectX;
auto Kumazuma::Client::FreeViewingState::OnUpdate(f32 timeDelta) -> void 
{
	auto app{ App::Instance() };
	auto& window{ app->GetWindow() };
	auto rc{ app->GetWindow().GetRect() };
	auto inputMgr{ InputManager::Instance() };
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

	XMVECTOR vDelta{ (vCurosrPos - vPrevCursor) *  (XM_2PI / 4.f)} ;
	f32 length{};
	XMStoreFloat(&length,XMVector2Length(vDelta));
	auto transformComponent{ this->GetTransformComponent() };

	if (length > 0.f)
	{
		XMVECTOR vRot{ XMLoadFloat3(&transformComponent->GetRotation()) };
		vDelta = XMVECTOR{ vDelta.m128_f32[1], vDelta.m128_f32[0], 0.f, 0.f };
		vRot = XMVectorAddAngles(vRot, vDelta);
		f32x3 rotation{ StoreF32x3(vRot) };
		transformComponent->SetRotation(rotation);
	}
	
	f32x44 transform{};
	transformComponent->GenerateTransformMatrixWithoutScale(&transform);
	XMMATRIX mTransfrom{ LoadF32x44(transform) };
	XMVECTOR vCameraForward{ mTransfrom.r[2] };
	XMVECTOR vCameraUp{ mTransfrom.r[1] };
	XMMATRIX mPlayerTransform{ LoadF32x44(this->GetPlayerTransform()) };
	XMVECTOR vPlayerPosition{ mPlayerTransform .r[3]};
	XMVECTOR vFocusAt{ vPlayerPosition + mPlayerTransform.r[1] * 4.5f };
	XMVECTOR vCameraNewPosition{ vFocusAt - vCameraForward * GetDistance() };
	transformComponent->SetPosition(StoreF32x3(vCameraNewPosition));
	XMMATRIX mViewSpace{ XMMatrixLookAtLH(vCameraNewPosition, vFocusAt, vCameraUp) };
	this->SetViewSpace(StoreF32x44(mViewSpace));
}

auto Kumazuma::Client::FreeViewingState::Reset(f32 timeDelta) -> void 
{
	auto app{ App::Instance() };
	auto& window{ app->GetWindow() };
	auto inputMgr{ InputManager::Instance() };
	inputMgr->LockCursor(window.GetHandle());
}
