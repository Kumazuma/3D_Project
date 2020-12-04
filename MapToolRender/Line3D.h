#pragma once
#include"MapToolRender.h"
class Ray;
namespace DirectX
{
	struct XMFLOAT3;
}
namespace MapToolRender
{
	ref class Position;
	ref class Rotation;
	ref class Line3D;
	ref class GraphicsDevice;
	public ref class Line3D: public RenderObject
	{
	public:
		Line3D(GraphicsDevice^ graphicsDevice, Position^ startPosition, Position^ endPosition);
		Line3D(GraphicsDevice^ graphicsDevice, DirectX::XMFLOAT3* startPosition, DirectX::XMFLOAT3* endPosition);
		Line3D(Line3D^ rhs);
	public:
		auto Clone()->RenderObject^ override;

	};
}