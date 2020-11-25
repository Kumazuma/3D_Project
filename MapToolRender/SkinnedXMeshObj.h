#pragma once
#include "MapToolRender.h"
#include "TextureCollection.h"
namespace MapToolRender
{
	public ref class SkinnedXMeshObj : public RenderObject
	{
	public:
		SkinnedXMeshObj(GraphicsDevice^ device, System::String^ filePath);
		auto Clone()->RenderObject^ override;
		auto PlayAnimIdx(unsigned int idx)->void;
		virtual auto Update(int ms)->void override;
	protected:
		SkinnedXMeshObj(SkinnedXMeshObj^ const& rhs);
	};
}