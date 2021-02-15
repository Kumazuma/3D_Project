#pragma once
#include"Mesh.hpp"
#include"GraphicsModule.hpp"
#include<vector>
namespace Kumazuma
{
	class DLL_CLASS CustomMesh : public Mesh
	{
	public:
		static CustomMesh* Create(GraphicsModule* gmodule, StaticMeshVertex const* vertices, u32 vertexCount);
		static CustomMesh* Create(GraphicsModule* gmodule, std::vector<StaticMeshVertex> const& vertices);
	};

	inline CustomMesh* CustomMesh::Create(GraphicsModule* gmodule, std::vector<StaticMeshVertex> const& vertices)
	{
		return CustomMesh::Create(gmodule, vertices.data(), vertices.size());
	}
}