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
	};
}