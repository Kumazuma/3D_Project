#pragma once
#include "common.hpp"
#include "typedef.hpp"
#include <d3d11.h>
namespace Kumazuma
{
	class Subsets;
	enum class MeshType
	{
		Static,
		Skinned
	};
	class DLL_CLASS Mesh
	{
	public:
		virtual ~Mesh() {}
		virtual void SetupIA(ID3D11DeviceContext* device) = 0;
		virtual Subsets const& GetSubsetsRef() = 0;
		virtual Texture2D* GetMaterialTexture(wchar_t const* materialName) = 0;
	};
	struct StaticMeshVertex
	{
		DirectX::XMFLOAT3 vPos;
		DirectX::XMFLOAT2 vTex;
		DirectX::XMFLOAT3 vNormal;
	};
	struct SkinnedMeshVertex
	{
		DirectX::XMFLOAT3 vPos;
		DirectX::XMFLOAT2 vTex;
		DirectX::XMFLOAT3 vNormal;
		DirectX::XMUINT4  vBones;
		DirectX::XMFLOAT4 vWeights;
	};
}