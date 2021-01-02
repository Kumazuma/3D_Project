#include "COMPlayerInput.hpp"
#include <Game/object.hpp>
#include <Game/TransformComponent.hpp>
#include "COMRenderObjectContainer.hpp"
#include "COMHeightmap.hpp"
#include "HeightMap.hpp"
namespace Kumazuma::Client
{
	using namespace Game;
	using namespace DirectX;
	const Game::ComponentTag<COMPlayerInput> COMPlayerInput::TAG{"COM_PLAYER_INPUT"};
	COMPlayerInput::COMPlayerInput():
		Component{ TAG }
	{
		Bind(EVT_UPDATE, &COMPlayerInput::Update);
	}
	auto COMPlayerInput::Clone() const -> Game::Component*
	{
		return new COMPlayerInput{ *this };
	}
	auto COMPlayerInput::Update(Game::UpdateEvent const& event) -> void
	{
		if (auto obj{ GetObj().lock() }; obj != nullptr)
		{
			auto transform = obj->GetComponent<Game::TransformComponent>();
			auto renderObjContainer{ obj->GetComponent<COMRenderObjectContainer>() };
			auto renderObj{ renderObjContainer->Get(L"") };
			if (!renderObj)return;
			auto skinnedMesh{ std::static_pointer_cast<SkinnedXMeshObject>(renderObj) };
			XMFLOAT4X4 transformMatrix;
			
			transform->GenerateTransformMatrixWithoutScale(&transformMatrix);

			XMMATRIX mTransform{XMLoadFloat4x4(&transformMatrix)};
			XMVECTOR vForward{ mTransform.r[2] };
			XMVECTOR vRight{ mTransform.r[0] };
			XMVECTOR vDelta{};
			XMVECTOR vPosition{ mTransform.r[3] };
			if ((GetAsyncKeyState('I') & 0x8000) != 0)
			{
				vDelta += vForward;
			}
			if ((GetAsyncKeyState('K') & 0x8000) != 0)
			{
				vDelta -= vForward;
			}
			if ((GetAsyncKeyState('J') & 0x8000) != 0)
			{
				vDelta -= vRight;
			}
			if ((GetAsyncKeyState('L') & 0x8000) != 0)
			{
				vDelta += vRight;
			}
			skinnedMesh->PlayAnimation(event.GetDelta());

			XMVECTOR vMovingDir{ XMVector3Normalize(vDelta) };
			auto heightMapCom{ obj->GetComponent<COMHeightMap>() };
			auto heightMap{ heightMapCom->GetHeightMap() };

			vDelta = vMovingDir * (event.GetDelta() * 100.f);
			XMVECTOR vNowPosition{ vPosition + vDelta };
			XMVECTOR vUP{ 0.f, 1.f, 0.f, 0.f };
			//정확하게 삼각형 위에 있다면
			//얌전히 그 위에 있는다.
			if (heightMap->IsOnTriangles(vNowPosition, vUP))
			{
				XMFLOAT3 newPosition{};
				XMStoreFloat3(&newPosition, vNowPosition);
				transform->SetPosition(newPosition);
			}
			else//그렇지 않다면,
			{

			}
			transform->GenerateTransformMatrix(&transformMatrix);
			renderObj->SetTransform(transformMatrix);
			
		}
	}
}
