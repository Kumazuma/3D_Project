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
		// IRenderable을(를) 통해 상속됨
		virtual auto PrepereRender(GraphicsDevice^ renderer) -> void override;

		// IRenderEntity을(를) 통해 상속됨
		virtual auto Render(GraphicsDevice^ renderer) -> void;
		property unsigned int AnimationSet
		{
			auto get()->unsigned int;
			auto set(unsigned int)->void;
		}
		property unsigned int AnimationCount
		{
			auto get()->unsigned int;
		}
		property String^ FilePath
		{
			auto get()->String^;
		}
	public:
		auto SetAnimationSet(int i)->void;
		auto PlayAnimation(float timeDelta)->void;
	internal:
		SkinnedXMesh^ mesh_;
		int animationSet_;
	};
}