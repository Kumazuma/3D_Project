#include"pch.h"
#include"include/game/TransformComponent.hpp"
#include"include/game/object.hpp"
using namespace DirectX;
constexpr size_t WORLDMATRIX_FORWARD{2};
constexpr size_t WORLDMATRIX_RIGHT{ 0 };
constexpr size_t WORLDMATRIX_UP{ 1 };
constexpr size_t WORLDMATRIX_POS{ 3 };

namespace Kumazuma
{
	namespace Game
	{
		IMPLEMENT_COMPONENT_CLASS(Kumazuma::Game::TransformComponent)

		TransformComponent::TransformComponent():
			Component{ TAG },
			m_scale{1.f},
			m_position{},
			m_rotation{}
		{
		}

		Component* TransformComponent::Clone() const
		{
			return new TransformComponent{ *this };
		}


		auto TransformComponent::SetPosition(DirectX::XMFLOAT3 const& value) -> void
		{
			m_position = value;
		}

		auto TransformComponent::SetRotation(DirectX::XMFLOAT3 const& value) -> void
		{
			m_rotation = value;
		}

		auto TransformComponent::GetScale() const ->float
		{
			return m_scale;
		}

		auto TransformComponent::GetPosition() const -> DirectX::XMFLOAT3 const&
		{
			return m_position;
		}

		auto TransformComponent::GetRotation() const -> DirectX::XMFLOAT3 const&
		{
			return m_rotation;
		}

		auto TransformComponent::GenerateTransformMatrix(DirectX::XMFLOAT4X4* pOut) const -> void
		{
			
			XMStoreFloat4x4(
				pOut,
				XMMatrixScaling(m_scale, m_scale, m_scale) *
				XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z) *
				XMMatrixTranslation(m_position.x, m_position.y, m_position.z)
			);
		}
		auto TransformComponent::GenerateTransformMatrixWithoutScale(DirectX::XMFLOAT4X4* pOut) const -> void
		{
			XMStoreFloat4x4(
				pOut,
				XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z) *
				XMMatrixTranslation(m_position.x, m_position.y, m_position.z)
			);
		}
		auto TransformComponent::GenerateTransform(DirectX::XMFLOAT4X4* out) const -> void
		{
			XMMATRIX mParent{ XMMatrixIdentity() };
			auto object{ GetObj().lock() };
			if (object != nullptr)
			{
				auto parent{ object->GetParent() };
				if (parent != nullptr)
				{
					auto parentTransform{ parent->GetComponent<TransformComponent>() };
					if (parentTransform != nullptr)
					{
						DirectX::XMFLOAT4X4 parentMatrix{};
						parentTransform->GenerateTransform(&parentMatrix);
						mParent = mParent * XMLoadFloat4x4(&parentMatrix);
					}
				}
			}
			XMStoreFloat4x4(
				out,
				XMMatrixScaling(m_scale, m_scale, m_scale) *
				XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z) *
				XMMatrixTranslation(m_position.x, m_position.y, m_position.z) * mParent
			);
		}
		auto TransformComponent::SetPosition(float x, float y, float z)->void
		{
			m_position = { x,y,z };
		}
		auto TransformComponent::SetRotation(float x, float y, float z)->void
		{
			m_rotation = { x,y,z };
		}
		auto TransformComponent::SetScale(float scale)->void
		{
			m_scale = scale;
		}
	}
}