#include "pch.h"
#include "MeshCollection.hpp"
#include "MeshCollectionImpl.hpp"
namespace Kumazuma
{
	MeshCollection::~MeshCollection()
	{
	}
	MeshCollection* MeshCollection::Load(GraphicsModule* gmodule, wchar_t const* path)
	{
		return MeshCollectionImpl::Load(gmodule,path);
	}
	Mesh::~Mesh()
	{

	}
}
