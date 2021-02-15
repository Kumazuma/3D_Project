#pragma once
#include "CustomMesh.hpp"
#include <d3d11.h>
#include <wrl.h>
namespace Kumazuma
{
	using namespace Microsoft::WRL;
	class CustomMeshImpl : public CustomMesh
	{
	public:
		CustomMeshImpl(GraphicsModule* gmodule, StaticMeshVertex const* vertices, u32 count);
		ComPtr<ID3D11Buffer> vertexBuffer_;
		ComPtr<ID3D11Buffer> indexBuffer_;
	};
}