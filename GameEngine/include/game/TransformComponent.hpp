#pragma once
#include"component.hpp"
#include<DirectXMath.h>
namespace Kumazuma
{
	namespace Game
	{
		class TransformComponent : public Component
		{
		public:
			static const ComponentTag<TransformComponent> TAG;
			TransformComponent();
			Component* Clone()const override;
			//Setter
		public:
			auto SetScale(DirectX::XMFLOAT3 const& value)->void;
			auto SetPosition(DirectX::XMFLOAT3 const& value)->void;
			auto SetRotation(DirectX::XMFLOAT3 const& value)->void;
			//Getter
		public:
			auto GetScale() const->DirectX::XMFLOAT3 const&;
			auto GetPosition() const->DirectX::XMFLOAT3 const&;
			auto GetRotation() const->DirectX::XMFLOAT3 const&;
			auto GenerateTransformMatrix(DirectX::XMFLOAT4X4 * pOut)const->void;
			auto GenerateTransformMatrixWithoutScale(DirectX::XMFLOAT4X4* pOut)const->void;

		private:
			DirectX::XMFLOAT3 m_scale;
			DirectX::XMFLOAT3 m_position;
			DirectX::XMFLOAT3 m_rotation;
		};
	}
}