#pragma once
#include "common.hpp"
#include "typedef.hpp"
#include <d3d11.h>
namespace Kumazuma
{
	class Subsets;
	class Texture2D;
	enum class MeshType:int
	{
		Static,
		Skinned
	};
	class DLL_CLASS Mesh
	{
	public:
		virtual ~Mesh();
		virtual void SetupIA(ID3D11DeviceContext* device) = 0;
		virtual Subsets const& GetSubsetsRef() = 0;
		virtual Texture2D* GetMaterialTexture(wchar_t const* materialName) = 0;
		virtual MeshType GetType() = 0;
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
	struct Triangle
	{
		uint32_t indices[3];
		template<typename T>
		auto operator[](T&& index)->u32&
		{
			return indices[index];
		}
		template<typename T>
		auto operator[](T&& index)const->u32 const&
		{
			return indices[index];
		}
	};
}