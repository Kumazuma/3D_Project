#pragma once
#include"framework.h"
#include "SkinnedMesh.hpp"
#include "MeshCollection.hpp"
#include <assimp\scene.h>
#include <assimp\mesh.h>
using namespace Microsoft::WRL;
namespace Kumazuma
{
	class SkinnedMeshImpl : public SkinnedMesh
	{
	public:
		SkinnedMeshImpl(MeshCollection* collection, GraphicsModule* const gmodule, aiScene const* scene, aiMesh* mesh);
		u32 GetIndexCount()const override;
		u32 GetIndexBase()const override;
		HRESULT GetVertexBuffer(ID3D11Buffer** out)const override;
		HRESULT GetIndexBuffer(ID3D11Buffer** out)const  override;
		std::wstring const& GetMaterial()const  override;
		std::wstring const& GetName()const  override;
		MeshType GetType()const override;
		HRESULT SetupIA(ID3D11DeviceContext* context)const override;
		MeshCollection* GetMeshCollection() const override;

		MeshCollection* collection;
		ComPtr<ID3D11Buffer> indexBuffer;
		ComPtr<ID3D11Buffer> vertexBuffer;
		std::wstring name;
		std::wstring materialName;
		u32 indexCount;
	};
}