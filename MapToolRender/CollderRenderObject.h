#pragma once
#include "MapToolRender.h"
namespace MapToolRender
{
	ref class SkinnedXMeshObj;
	public ref  class ColliderRenderObject abstract : public RenderObject
	{
	protected:
		ColliderRenderObject()
		{
		}
		ColliderRenderObject(ColliderRenderObject^ rhs):
			RenderObject(rhs)
		{

		}
	public:
		virtual auto SetFrameMatrix(SkinnedXMeshObj^ skinnedMesh, String^ frameName)->void = 0;
		virtual auto SetAttribute(MapToolCore::ColliderAttribute^ attribute)->void = 0;
	public:
		property MapToolCore::Offset Offset
		{
			virtual auto get()->MapToolCore::Offset = 0;
			virtual auto set(MapToolCore::Offset offset)->void = 0;
		}
	};
}