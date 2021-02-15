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
		// IRenderable��(��) ���� ��ӵ�
		virtual auto PrepereRender(GraphicsDevice^ renderer) -> void override;
		// IRenderEntity��(��) ���� ��ӵ�
		virtual auto Render(GraphicsDevice^ renderer) -> void;
	public:
		
	internal:
		BoxMesh^ mesh_;
	};
}