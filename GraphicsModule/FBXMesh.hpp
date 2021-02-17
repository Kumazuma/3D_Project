#pragma once
#include "common.hpp"
#include "typedef.hpp"
#include "Mesh.hpp"
namespace Kumazuma
{
	class GraphicsModule;
	class DLL_CLASS FBXMesh : public Mesh
	{
	public:
		static FBXMesh* Create(GraphicsModule* device, wchar_t const* path);
	};
}