#pragma once
#include "Subsets.hpp"
#include "Subset.hpp"
#include <vector>
#include <wrl.h>
using namespace Microsoft::WRL;
namespace Kumazuma
{
	class OBJMesh;
	class OBJSubset : public Subset
	{
	public:
		u32 GetTriangleCount()const override;
		u32 GetIndexBase()const override;
		std::wstring const& GetMaterial()const override;
		std::wstring const& GetName()const override;
		HRESULT GetVertexBuffer(ID3D11Buffer** out) const override;
		HRESULT GetIndexBuffer(ID3D11Buffer** out)const override;
		Mesh& GetMesh()const override;
		OBJMesh* mesh_;
		u32 triangleCount_;
		u32 indexBase_;
		std::wstring materialName;
		std::wstring name;
		ComPtr<ID3D11Buffer> vertexBuffer;
		ComPtr<ID3D11Buffer> indexBuffer;
	};
	class OBJSubsets : public Subsets
	{
	public:
		virtual u32				GetCount();
		virtual Subset const&	Get(u32 index);
		std::vector<OBJSubset> subsets_;
	};
}