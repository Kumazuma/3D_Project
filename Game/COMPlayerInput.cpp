#include "COMPlayerInput.hpp"
#include <Game/object.hpp>
#include <Game/TransformComponent.hpp>
#include "COMRenderObjectContainer.hpp"
#include "COMHeightmap.hpp"
#include "HeightMap.hpp"
#include "framework.h"
#include "app.h"
#include "PhysicsCharacterController.hpp"
constexpr StringLiteral<wchar_t> CHARACTER_MESH{ L"CHARACTER" };
namespace Kumazuma::Client
{
	using namespace DirectX;
	Game::ComponentTag<COMPlayerInput> const COMPlayerInput::TAG{"COM_PLAYER_INPUT"};
	f32 constexpr PLAYER_SPEED{ 40.f };
	COMPlayerInput::COMPlayerInput():
		Component{ TAG },
		m_gravity{}
	{
		Bind(Game::EVT_UPDATE, &COMPlayerInput::Update);
		auto& window{ App::Instance()->GetWindow() };
		auto rc{ window.GetRect() };
		POINT cursorPos{ window.GetClientCursorPos() };
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

		XMStoreFloat2(&prevMousePosition_, vCurosrPos);
	}
	auto COMPlayerInput::Clone() const -> Game::Component*
	{
		return new COMPlayerInput{ *this };
	}
	auto COMPlayerInput::Update(Game::UpdateEvent const& event) -> void
	{
		auto inputMgr{ InputManager::Instance() };
		if (auto obj{ GetObj().lock() }; obj != nullptr)
		{
			auto transform = obj->GetComponent<Game::TransformComponent>();
			auto renderObjContainer{ obj->GetComponent<COMRenderObjectContainer>() };
			auto renderObj{ renderObjContainer->Get(CHARACTER_MESH) };
			if (!renderObj)return;
			auto skinnedMesh{ std::static_pointer_cast<SkinnedXMeshObject>(renderObj) };
			XMFLOAT4X4 transformMatrix;
			
			transform->GenerateTransformMatrixWithoutScale(&transformMatrix);

			XMMATRIX mTransform{XMLoadFloat4x4(&transformMatrix)};
			XMVECTOR vForward{ mTransform.r[2] };
			XMVECTOR vRight{ mTransform.r[0] };
			XMVECTOR vDelta{};
			XMVECTOR vPosition{ mTransform.r[3] };
			auto rotation{ transform->GetRotation() };

			if ((GetAsyncKeyState(VK_SPACE) & 0x8000) != 0)
			{
				//TODO:
				//obj->GetComponent<COMMoveController>()->Jump(event.GetDelta());

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
					XMStoreFloat2(&prevMousePosition_, vCurosrPos);
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
					rotation.y -= event.GetDelta();
				}
				if (inputMgr->IsPressing(PLAYER_INPUT::MOVE_RIGHT))
				{
					rotation.y += event.GetDelta();
				}
			}
			XMVECTOR vMovingDir{ XMVector3Normalize(vDelta) };
			f32 len{};
			XMStoreFloat(&len, XMVector3Length(vDelta));
			if (len > 0.f)
			{
				obj->GetComponent<PhysicsCharacterController>()->Move(vMovingDir * PLAYER_SPEED);
				skinnedMesh->SetAnimationSet(0);
			}
			else
			{
				obj->GetComponent<PhysicsCharacterController>()->Stop();
				skinnedMesh->SetAnimationSet(1);
			}
			transform->SetRotation(rotation);
			transform->GenerateTransformMatrix(&transformMatrix);
			renderObj->SetTransform(transformMatrix);
			skinnedMesh->PlayAnimation(event.GetDelta());
			return;
		}
	}
}
