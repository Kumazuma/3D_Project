#pragma once
#include "Mesh.hpp"
namespace Kumazuma
{
	class OBJSubsets;
	class GraphicsModule;
	class DLL_CLASS OBJMesh : public Mesh
	{
	public:
		static OBJMesh* Create(GraphicsModule* device, wchar_t const* path);
	};
}