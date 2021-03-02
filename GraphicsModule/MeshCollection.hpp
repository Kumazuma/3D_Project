#pragma once
#include "common.hpp"
#include "GraphicsModule.hpp"
#include <string>
namespace Kumazuma
{
	class Mesh;
	enum class MeshType
	{
		Static,
		Skinned
	};
	class DLL_CLASS MeshCollection
	{
	public:
		static MeshCollection* Load(GraphicsModule* gmodule, wchar_t const* path);
		virtual ~MeshCollection();
		virtual u32	GetCount() = 0;
		virtual Mesh*		GetMesh(u32 index) = 0;
		virtual HRESULT  SetMaterialTexture(wchar_t const* materialName, wchar_t const* texturePath) = 0;
		virtual Texture2D*	GetMaterialTexture(wchar_t const* materialName) = 0;
	};

	class DLL_CLASS Mesh
	{
	public:
		virtual ~Mesh();
		virtual MeshType GetType()const = 0;
		virtual HRESULT SetupIA(ID3D11DeviceContext* context)const = 0;
		virtual u32 GetIndexCount()const = 0;
		virtual u32 GetIndexBase()const = 0;

		virtual MeshCollection* GetMeshCollection() const = 0;
		virtual HRESULT GetVertexBuffer(ID3D11Buffer** out)const = 0;
		virtual HRESULT GetIndexBuffer(ID3D11Buffer** out)const = 0;
		virtual std::wstring const& GetMaterial()const = 0;
		virtual std::wstring const& GetName()const = 0;
	};
}