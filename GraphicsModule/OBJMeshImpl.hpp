#pragma once
#include "OBJMesh.hpp"
#include "OBJSubsets.hpp"
#include <wrl.h>
#include <memory>
#include <vector>
#include <unordered_map>
#include "Texture2D.hpp"
using namespace Microsoft::WRL;
namespace Kumazuma
{
	class Texture2D;
	class OBJMeshImpl : public OBJMesh
	{
	public:
		OBJMeshImpl(GraphicsModule* device, wchar_t const* path);
		OBJMeshImpl(OBJMeshImpl&& rhs)noexcept;
		~OBJMeshImpl();
		virtual void				SetupIA(ID3D11DeviceContext* device) override;
		virtual Subsets const&		GetSubsetsRef();
		virtual Texture2D*			GetMaterialTexture(wchar_t const* materialName) override;

		OBJSubsets subsets_;
		ComPtr<ID3D11Buffer> vertexBuffer_;
		ComPtr<ID3D11Buffer> indexBuffer_;
		std::vector<DirectX::XMFLOAT3A> vertexPositions;
		std::vector<Triangle> triangles;
		std::unordered_map<std::wstring, std::unique_ptr<Texture2D> > materialTextures_;
	};
}