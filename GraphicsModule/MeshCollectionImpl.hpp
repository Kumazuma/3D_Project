#pragma once
#include "MeshCollection.hpp"
#include "GraphicsModule.hpp"
#include "Texture2D.hpp"
#include <vector>
#include <memory>
#include <unordered_map>
namespace Kumazuma
{
	class MeshCollectionImpl : public MeshCollection
	{
	public:
		static MeshCollectionImpl* Load(GraphicsModule* const gmodule, wchar_t const* const path);
		MeshCollectionImpl();
		u32	GetCount() override;
		Mesh* GetMesh(u32 index) override;
		Texture2D* GetMaterialTexture(wchar_t const* materialName) override;
		HRESULT  SetMaterialTexture(wchar_t const* materialName, wchar_t const* texturePath) override;

	private:
		std::vector<std::unique_ptr<Mesh> > meshs;
		std::unordered_map<std::wstring, std::unique_ptr<Texture2D> > diffuseTextures;
		GraphicsModule* gmodule;
	};
}