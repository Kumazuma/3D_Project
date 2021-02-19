#pragma once
#include "IRenderable.hpp"
#include "RenderObject.hpp"
namespace MapToolCore
{
	ref class Collider;
}

namespace MaptoolRenderer
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace MapToolCore;
	ref class Mesh;
	ref class SkinnedXMeshObject;
	public ref class SphereObject abstract:
		public IRenderable,
		public IRenderEntity
	{
	public:
		SphereObject();
	public:
		// IRenderable을(를) 통해 상속됨
		virtual auto PrepereRender(GraphicsDevice^ renderer) -> void override;
		// IRenderEntity을(를) 통해 상속됨
		virtual auto Render(GraphicsDevice^ renderer) -> void = 0;
	protected:
		auto DoRender(GraphicsDevice^ renderer, float radius, MapToolCore::Transform^ transform)->void;

	internal:
		Mesh^ mesh_;
	};
}