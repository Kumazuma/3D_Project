#pragma once
#pragma unmanaged
#pragma warning(push)
#pragma warning(once : 4793)
#include<DirectXMath.h>
#pragma warning(pop)
#pragma managed
#include"Transform.h"
namespace MapToolRender
{
	
	public ref class Camera
	{
	public:
		Camera();
		~Camera();
		!Camera();
		auto MoveUp(float delta)->void;
		auto MoveForward(float delta)->void;
		auto MoveRight(float delta)->void;
		auto RotationX(float delta)->void;
		auto RotationY(float delta)->void;
		property DirectX::XMFLOAT3* PositionPtr
		{
			auto get()->DirectX::XMFLOAT3*;
		}
		property DirectX::XMFLOAT3* RotationPtr
		{
			auto get()->DirectX::XMFLOAT3*;
		}
		property Position^ Position
		{
			auto get()->MapToolRender::Position^;
			auto set(MapToolRender::Position^ value)->void;
		}
		property Rotation^ Rotation
		{
			auto get()->MapToolRender::Rotation^;
			auto set(MapToolRender::Rotation^ value)->void;
		}
		
	private:
		MapToolRender::Position^ m_position;
		MapToolRender::Rotation^ m_rotation;
		DirectX::XMFLOAT3* m_pPosition;
		DirectX::XMFLOAT3* m_pRotation;
	};
}