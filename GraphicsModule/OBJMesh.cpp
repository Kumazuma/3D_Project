#include "pch.h"
#include "OBJMesh.hpp"
#include "OBJMeshImpl.hpp"
Kumazuma::OBJMesh* Kumazuma::OBJMesh::Create(GraphicsModule* device, wchar_t const* path)
{
    return new OBJMeshImpl{ device, path };
}
