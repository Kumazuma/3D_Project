#pragma once
#include "IRenderable.hpp"
#include "RenderObject.hpp"
namespace MaptoolRenderer
{
	ref class BoxMesh;
	public ref class BoxMeshObject :
		public MeshObject,
		public IRenderEntity
	{
	public:
		BoxMeshObject(BoxMesh^ mesh);
	public:
		// IRenderable을(를) 통해 상속됨
		virtual auto PrepereRender(GraphicsDevice^ renderer) -> void override;
		// IRenderEntity을(를) 통해 상속됨
		virtual auto Render(GraphicsDevice^ renderer) -> void;
	public:
		
	internal:
		BoxMesh^ mesh_;
	};
}