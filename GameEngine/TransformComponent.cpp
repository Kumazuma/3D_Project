#include"pch.h"
#include"include/game/TransformComponent.hpp"
using namespace DirectX;
constexpr size_t WORLDMATRIX_FORWARD{2};
constexpr size_t WORLDMATRIX_RIGHT{ 0 };
constexpr size_t WORLDMATRIX_UP{ 1 };
constexpr size_t WORLDMATRIX_POS{ 3 };

namespace Kumazuma
{
	namespace Game
	{
		const ComponentTag<TransformComponent> TransformComponent::TAG{ "TransformComponent" };
		
		TransformComponent::TransformComponent():
			Component{ TAG },
			m_scale{1.f, 1.f, 1.f},
			m_position{},
			m_rotation{}
		{
		}

		Component* TransformComponent::Clone() const
		{
			return new TransformComponent{ *this };
		}

		auto TransformComponent::SetScale(DirectX::XMFLOAT3 const& value) -> void
		{
			m_scale = value;
		}

		auto TransformComponent::SetPosition(DirectX::XMFLOAT3 const& value) -> void
		{
			m_position = value;
		}

		auto TransformComponent::SetRotation(DirectX::XMFLOAT3 const& value) -> void
		{
			m_rotation = value;
		}

		auto TransformComponent::GetScale() const -> DirectX::XMFLOAT3 const&
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
				XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z) *
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
	}
}