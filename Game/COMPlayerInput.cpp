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
		Component{ TAG },
		m_gravity{}
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
			f32 const speed{ event.GetDelta() * 100.f };
			vDelta = vMovingDir * speed;
			XMVECTOR vNowPosition{ vPosition + vDelta };
			XMVECTOR vUP{ 0.f, 1.f, 0.f, 0.f };
			//��Ȯ�ϰ� �ﰢ�� ���� �ִٸ�
			//������ �� ���� �ִ´�.
			if (heightMap->GetTriangle(vNowPosition, vUP) != std::nullopt)
			{
				XMFLOAT3 newPosition{};
				XMStoreFloat3(&newPosition, vNowPosition);
				transform->SetPosition(newPosition);
				m_gravity = 0.f;
			}
			//Ȥ�� �����̱� ������ ��ġ�� �ﰢ�� ���� �־��°�?
			else if (auto op = heightMap->GetTriangle(vPosition, vUP); op != std::nullopt)
			{
				//�ﰢ���� �־����� �����̵� ���͸� ���ؼ� �̵���Ų��.
				XMVECTOR vNormal{ XMLoadFloat3A(&op->normalVector) };
				XMVECTOR vS{XMVector3Normalize( vMovingDir - vNormal * XMVector3Dot(vNormal, vMovingDir)) };
				//�����̵� ���ͷ� ������ŷ�ؼ� ª�� �Ÿ��� ���ؾ� �Ѵ�.
				f32 const moveT{ heightMap->RayPicking(vPosition, vS).value_or(speed) };

				vNowPosition = vPosition + vS * std::min(moveT, speed);
				XMFLOAT3 newPosition{};
				XMStoreFloat3(&newPosition, vNowPosition);
				transform->SetPosition(newPosition);
				m_gravity = 0.f;
			}
			//�׷��� �ʴٸ�, ���Ͻ�Ų��.
			else
			{
				m_gravity = 9.8f * event.GetDelta();
				XMVECTOR vGravity{ XMVectorSet(0.f, -m_gravity, 0.f, 0.f )};
				XMVECTOR vVelocity{ vGravity + vDelta };
				vMovingDir = XMVector3Normalize(vVelocity);
				auto t = heightMap->RayPicking(vPosition, vMovingDir);
				//�÷��̾� ������ ������ ���ȴµ�, ��� ���� ������, �ؿ� �ƹ��͵� ���� ���̴�, �׳� ��������
				if (t != std::nullopt)
				{
					//���� �ִٸ� �ش� t�� 
					f32 speed{};
					XMStoreFloat(&speed, XMVector3Length(vVelocity));
					if (speed > t)
					{
						m_gravity = 0.f;
					}
					vNowPosition = vPosition + std::min(speed, *t) * vMovingDir;
				}
				else
				{
					vNowPosition = vPosition + vVelocity;
				}
				XMFLOAT3 newPosition{};
				XMStoreFloat3(&newPosition, vNowPosition);
				transform->SetPosition(newPosition);
			}
			transform->GenerateTransformMatrix(&transformMatrix);
			renderObj->SetTransform(transformMatrix);
		}
	}
}
