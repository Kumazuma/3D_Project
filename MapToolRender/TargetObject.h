#pragma once
#include "MapToolRender.h"
namespace MapToolRender
{
	public ref class TargetObject : public RenderObject
	{
	public:
		TargetObject(GraphicsDevice^ device);
	private:
		TargetObject(TargetObject^ rhs);
	public:
		auto Clone()->RenderObject^ override;
	};
}