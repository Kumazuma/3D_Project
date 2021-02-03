#pragma once
#include "IRenderable.hpp"
#include "OBJMesh.hpp"
namespace MaptoolRenderer {
	ref class OBJSubset;
	public ref class OBJObject : public IRenderable
	{
	public:
		auto PrepereRender(GraphicsDevice^ renderer)->void override;

	};
	private ref class OBJSubset: public IRenderEntity
	{
	public:
		auto Render(GraphicsDevice^ renderer)->void override;
	};
}