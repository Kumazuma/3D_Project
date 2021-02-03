#pragma once
#include"component.hpp"
#include<DirectXMath.h>
namespace Kumazuma
{
	namespace Game
	{
		class DLL_CLASS TransformComponent : public Component
		{
		public:
			TransformComponent();
			Component* Clone()const override;
			//Setter
		public:
			auto SetPosition(DirectX::XMFLOAT3 const& value)->void;
			auto SetRotation(DirectX::XMFLOAT3 const& value)->void;

			auto SetPosition(float x, float y, float z)->void;
			auto SetRotation(float x, float y, float z)->void;
			auto SetScale(float scale)->void;
			//Getter
		public:
			auto GetScale() const->float;
			auto GetPosition() const->DirectX::XMFLOAT3 const&;
			auto GetRotation() const->DirectX::XMFLOAT3 const&;
			auto GenerateTransformMatrix(DirectX::XMFLOAT4X4 * pOut)const->void;
			auto GenerateTransformMatrixWithoutScale(DirectX::XMFLOAT4X4* pOut)const->void;
			auto GenerateTransform(DirectX::XMFLOAT4X4* out)const->void;
		private:
			float m_scale;
			DirectX::XMFLOAT3 m_position;
			DirectX::XMFLOAT3 m_rotation;
			DECLARE_COMPONENT_CLASS(Kumazuma::Game::TransformComponent)
		};
	}
}