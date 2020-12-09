#pragma once
#include "MapToolRender.h"
namespace MapToolRender
{
	public ref class NaviMesh : public RenderObject
	{
	public:
		NaviMesh(GraphicsDevice^ device);
	//properties
	public:
		property float WeldDistance
		{
			auto get()->float;
			auto set(float)->void;
		}
	protected:
		NaviMesh(NaviMesh^ const& rhs);
	public:
		auto Clone()->RenderObject^ override;
		auto PushPoint(float x, float y, float z, Camera^ camera)->void;
		property cli::array<MapToolCore::Position>^ Vertices
		{
			auto get()->cli::array<MapToolCore::Position>^;
		}
		property cli::array<unsigned short>^ Indices
		{
			auto get()->cli::array<unsigned short>^;
		}
	};
}