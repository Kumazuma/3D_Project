#include "pch.h"

#include "MeshCollectionImpl.hpp"
#undef min
#undef max

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <filesystem>
#include "UnicodeHelper.h"
#include <vector>
#include "MeshImpl.hpp"
#include "SkinnedMeshImpl.hpp"
#include "TextureManager.hpp"
namespace Kumazuma
{
	MeshCollectionImpl* MeshCollectionImpl::Load(GraphicsModule* const gmodule, wchar_t const* const path)
	{
		Assimp::Importer importer;
		std::string filename{ ConvertWideToUTF8(path) };
		std::filesystem::path filePath{ filename };
		aiProcess_ConvertToLeftHanded;
		const aiScene* pScene = importer.ReadFile(filename,
			aiProcess_Triangulate |
			aiProcess_MakeLeftHanded |
			aiProcess_FlipUVs|
			aiProcess_FlipWindingOrder );
		if (pScene == nullptr)
		{
			return nullptr;
		}
		aiMesh** it{ pScene->mMeshes };
		aiMesh** const end{ pScene->mMeshes + pScene->mNumMeshes };
		std::vector <std::unique_ptr<Mesh>> meshs;
		MeshCollectionImpl* collection = new MeshCollectionImpl{};
		for (; it != end; ++it)
		{
			aiMesh* mesh{ *it };
			if (mesh->HasBones())
			{
				//TODO Skinned Mesh
				auto skinnedMesh{ std::unique_ptr<Mesh>{new SkinnedMeshImpl(collection, gmodule,pScene, mesh) } };
				meshs.emplace_back(std::move(skinnedMesh));
			}
			else
			{
				//Static mesh
				auto staticMesh{ std::unique_ptr<Mesh>{new StaticMeshImpl(collection, gmodule,pScene, mesh) } };
				meshs.emplace_back(std::move(staticMesh));
			}
		}
		auto materialIt = pScene->mMaterials;
		auto const materialEnd = pScene->mMaterials + pScene->mNumMaterials;
		std::unordered_map<std::wstring, std::unique_ptr<Texture2D> > textures;
		for (; materialIt != materialEnd; ++materialIt)
		{
			auto mat = *materialIt;
			auto matName{ ConvertUTF8ToWide(mat->GetName().C_Str()) };
			for (UINT i = 0; i < mat->GetTextureCount(aiTextureType_DIFFUSE); i++)
			{
				aiString str;
				mat->GetTexture(aiTextureType_DIFFUSE, i, &str);
				
				std::wstring textureName{ ConvertUTF8ToWide(str.C_Str()) };
				auto texturePath = std::filesystem::absolute(filePath).parent_path() / textureName;
				auto texture{ std::unique_ptr<Texture2D>{gmodule->GetTextureManager().Load(texturePath.generic_wstring().c_str())} };
				if (texture != nullptr)
				{
					textures.emplace(matName, std::move(texture) );
				}
			}
		}


		collection->gmodule = gmodule;
		collection->meshs.swap(meshs);
		collection->diffuseTextures.swap(textures);
		return collection;
	}
	MeshCollectionImpl::MeshCollectionImpl()
	{
	}
	u32 MeshCollectionImpl::GetCount()
	{
		return static_cast<u32>(meshs.size());
	}
	Mesh* MeshCollectionImpl::GetMesh(u32 index)
	{
		if (meshs.size() <= index)
		{
			return nullptr;
		}
		return meshs[index].get();
	}
	Texture2D* MeshCollectionImpl::GetMaterialTexture(wchar_t const* materialName)
	{
		auto it = diffuseTextures.find(materialName);
		if(it == diffuseTextures.end())
		{
			return nullptr;
		}
		return it->second.get();
	}
	HRESULT MeshCollectionImpl::SetMaterialTexture(wchar_t const* materialName, wchar_t const* texturePath)
	{
		auto texture = gmodule->GetTextureManager().Load(texturePath);
		if (texture != nullptr)
		{
			diffuseTextures[materialName] = std::unique_ptr<Texture2D>{ texture };
		}
		return S_OK;
	}
}
