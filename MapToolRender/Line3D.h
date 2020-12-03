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
		Line3D(Position^ rayStart, Position^ rayAt);
		Line3D(DirectX::XMFLOAT3* rayStart, DirectX::XMFLOAT3* rayAt);
		!Ray();
		auto Create3DLine(GraphicsDevice^ graphicsDev)->Line3D^;
	public:
		property ::Ray* Handle {auto get()->::Ray* { return m_pNative; }}
	private:
		::Ray* m_pNative;
	};
}