#include "IdleViewingState.hpp"
#include "framework.h"
#include "app.h"
using namespace DirectX;
auto Kumazuma::Client::IdleViewingState::OnUpdate(f32 timeDelta) -> void
{
	auto transformComponent{ GetTransformComponent() };
	f32x44 transform{};
	transformComponent->GenerateTransformMatrixWithoutScale(&transform);
	XMMATRIX mTransfrom{ LoadF32x44(transform) };
	XMVECTOR vCameraForward{ mTransfrom.r[2] };
	XMVECTOR vCameraUp{ mTransfrom.r[1] };
	XMMATRIX mPlayerTransform{ LoadF32x44(this->GetPlayerTransform()) };
	XMVECTOR vPlayerPosition{ mPlayerTransform.r[3] };
	XMVECTOR vFocusAt{ vPlayerPosition + mPlayerTransform.r[1] * 4.5f };
	XMVECTOR vCameraNewPosition{ vFocusAt - vCameraForward * GetDistance() };
	transformComponent->SetPosition(StoreF32x3(vCameraNewPosition));
	XMMATRIX mViewSpace{ XMMatrixLookAtLH(vCameraNewPosition, vFocusAt, vCameraUp) };
	this->SetViewSpace(StoreF32x44(mViewSpace));
}

auto Kumazuma::Client::IdleViewingState::Reset(f32 timeDelta) -> void
{
	auto app{ App::Instance() };
	auto& window{ app->GetWindow() };
	auto inputMgr{ InputManager::Instance() };
	inputMgr->UnlockCursor();
}
