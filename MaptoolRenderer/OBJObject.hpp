#pragma once
#include "IRenderable.hpp"
#include "OBJMesh.hpp"
#include "RenderObject.hpp"
namespace DirectX
{
	struct XMFLOAT4X4;
}
namespace MaptoolRenderer {
	ref class OBJSubset;
	using namespace System::ComponentModel;
	using namespace System::Collections::Generic;
	using namespace System;
	[TypeConverter(EnumConverter::typeid)]
	public enum class OBJMeshObjectUsage {
		Terrain,
		Structure
	};
	[TypeConverter(EnumConverter::typeid)]
	public enum class OBJSubsetRenderKind {
		Deferred,
		AlphaTest
	};
	ref class OBJSubsetCollection;
	public ref class OBJObject :
		public MeshObject
	{
	public:
		OBJObject(OBJMesh^ mesh);
		OBJObject(OBJObject^ rhs);
	public:
		virtual auto PrepereRender(GraphicsDevice^ renderer)->void override;
		property OBJSubsetCollection^ Subsets {
			auto get()->OBJSubsetCollection^;
		}
		[TypeConverter(ExpandableObjectConverter::typeid)]
		property MaptoolRenderer::Mesh^ Mesh {
			auto get()->MaptoolRenderer::Mesh^;
		}
		property OBJMeshObjectUsage Usage
		{
			auto get()->OBJMeshObjectUsage;
			auto set(OBJMeshObjectUsage)->void;
		}
	internal:
		OBJMesh^ mesh_;
		OBJMeshObjectUsage usage_;
		IList<OBJSubset^>^ subsets_;
	private:

	};
	public ref class OBJSubset: public IRenderEntity
	{
	public:
		OBJSubset(OBJObject^ parent, String^ name);
		property OBJSubsetRenderKind RenderKind {
			auto get()->OBJSubsetRenderKind;
			auto set(OBJSubsetRenderKind kind)->void;
		}
		property String^ Name {
			auto get()->String^;
		}
		virtual auto Render(GraphicsDevice^ renderer)->void override;
	private:
		OBJSubsetRenderKind renderKind_;
		OBJObject^ objObject_;
		String^ name_;
	};
}