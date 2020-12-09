#pragma once
#include"MapToolRender.h"
class Ray;
namespace DirectX
{
	struct XMFLOAT3;
}
namespace MapToolRender
{
	ref class Line3D;
	ref class GraphicsDevice;
	public ref class Line3D: public RenderObject
	{
	public:
		Line3D(GraphicsDevice^ graphicsDevice, MapToolCore::Position startPosition, MapToolCore::Position endPosition);
		Line3D(GraphicsDevice^ graphicsDevice, DirectX::XMFLOAT3* startPosition, DirectX::XMFLOAT3* endPosition);
		Line3D(Line3D^ rhs);
	public:
		auto Clone()->RenderObject^ override;

	};
}