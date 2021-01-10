#include "LookForwardState.hpp"
#include<sstream>
#include<Windows.h>
#include"framework.h"
#include "app.h"
#undef min
#undef max
using namespace DirectX;
Kumazuma::Client::LookForwardState::LookForwardState():
	maximumRot_{XMConvertToRadians(180.f)}
{
}

auto Kumazuma::Client::LookForwardState::OnUpdate(f32 timeDelta) -> void 
{
	auto transformComponent{ this->GetTransformComponent() };

	f32x44 cameraTransform{};
	transformComponent->GenerateTransformMatrixWithoutScale(&cameraTransform);
	f32x44 playerTransform{ this->GetPlayerTransform() };
	XMMATRIX mCameraTranform{ XMLoadFloat4x4(&cameraTransform) };
	XMMATRIX mPlayerTransform{ XMLoadFloat4x4(&playerTransform) };
	//플레이어와 카메라의 Y축은 필요 없으니 빼고 정규화한다.
	
	XMVECTOR vPlayerForward{ XMVector3Normalize(XMVectorSetY(mPlayerTransform.r[2], 0.f)) };
	XMVECTOR vPlayerRight{ XMVector3Normalize(XMVectorSetY(mPlayerTransform.r[0], 0.f)) };
	XMVECTOR vCameraForward{ XMVector3Normalize(XMVectorSetY(mCameraTranform.r[2], 0.f)) };
	if (!XMVector3Equal(vPlayerForward, vCameraForward))
	{
		f32 cosValue{};
		f32 dotRes{};
		XMStoreFloat(&cosValue, XMVector3AngleBetweenNormals(vPlayerForward, vCameraForward));
		//XMStoreFloat(&dotRes, XMVector3Dot(vPlayerRight, vCameraForward));
		XMVECTOR vCrossProduct{ XMVector3Cross(vCameraForward ,vPlayerForward) };
		//작으면 예각 크면 둔각

		f32 radian = cosValue;
		f32 rot = std::min(abs(radian), maximumRot_ * timeDelta);
		XMFLOAT3 resultForward{};
		f32 t{ rot / radian };
		
		if (abs(t) > 0.f)
		{
			std::stringstream ss;
			XMVECTOR v1{ vCameraForward };
			XMVECTOR v2{ vPlayerForward };
			vCameraForward = XMVector3Normalize(XMVectorLerp(v1, v2, t));
		}
		else
		{
			vCameraForward = vPlayerForward;
		}
		XMStoreFloat3(&resultForward, vCameraForward);
		radian = atan2f(resultForward.x ,resultForward.z);
		auto rotation{ transformComponent->GetRotation() };
		rotation.y = radian;
		transformComponent->SetRotation(rotation);
	}
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

	XMVECTOR vDelta{ (vCurosrPos - vPrevCursor) * (XM_2PI / 4.f) };
	f32 length{};
	XMStoreFloat(&length, XMVector2Length(vDelta));

	if (length > 0.f)
	{
		XMVECTOR vRot{ XMLoadFloat3(&transformComponent->GetRotation()) };
		vDelta = XMVECTOR{ vDelta.m128_f32[1], 0.f, 0.f, 0.f };
		vRot = XMVectorAddAngles(vRot, vDelta);
		f32x3 rotation{ StoreF32x3(vRot) };
		transformComponent->SetRotation(rotation);
	}


	transformComponent->GenerateTransformMatrixWithoutScale(&cameraTransform);
	mCameraTranform = XMLoadFloat4x4(&cameraTransform);
	vCameraForward = mCameraTranform.r[2];
	XMVECTOR vAt{ mPlayerTransform.r[3] + mPlayerTransform.r[1] * 4.5f };
	XMVECTOR vCameraNewPosition{ vAt - vCameraForward * GetDistance()};
	XMFLOAT3 cameraNewPosition{};
	XMStoreFloat3(&cameraNewPosition, vCameraNewPosition);
	transformComponent->SetPosition(cameraNewPosition);
	XMMATRIX mViewSpace{ XMMatrixLookAtLH(mCameraTranform.r[3], vAt, mCameraTranform.r[1]) };
	f32x44 viewSpaceMatrix{};
	XMStoreFloat4x4(&viewSpaceMatrix, mViewSpace);
	this->SetViewSpace(viewSpaceMatrix);
}

auto Kumazuma::Client::LookForwardState::Reset(f32 timeDelta) -> void 
{
	auto app{ App::Instance() };
	auto& window{ app->GetWindow() };
	auto inputMgr{ InputManager::Instance() };
	inputMgr->LockCursor(window.GetHandle());
}
