#pragma once
#include "IRenderable.hpp"
#include "RenderObject.hpp"
namespace MaptoolRenderer
{
	ref class SkinnedXMesh;
	public ref class SkinnedXMeshObject:
		public MeshObject,
		public IRenderEntity
	{
	public:
		SkinnedXMeshObject(SkinnedXMesh^ mesh);
	public:
		// IRenderable��(��) ���� ��ӵ�
		virtual auto PrepereRender(GraphicsDevice^ renderer) -> void override;

		// IRenderEntity��(��) ���� ��ӵ�
		virtual auto Render(GraphicsDevice^ renderer) -> void;
	public:
		auto SetAnimationSet(int i)->void;
		auto PlayAnimation(float timeDelta)->void;
	internal:
		SkinnedXMesh^ mesh_;
	};
}