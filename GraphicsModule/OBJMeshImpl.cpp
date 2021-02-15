#include "pch.h"
#include <vector>
#include "OBJMeshImpl.hpp"
#include "TextureManager.hpp"
#include "GraphicsModule.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <unordered_map>
#include <filesystem>
#include "UnicodeHelper.h"
#include "Texture2D.hpp"
#undef max
#undef min
using namespace DirectX;
namespace std
{
    template<>
    struct hash<DirectX::XMUINT3>
    {
        hash<u32> hasher;
        size_t operator()(const DirectX::XMUINT3& _Keyval)const noexcept {
            return
                hasher(_Keyval.x) ^
                hasher(_Keyval.y) ^
                hasher(_Keyval.z);
        }
    };
    template<>
    struct equal_to<DirectX::XMUINT3>
    {
        bool operator()(const DirectX::XMUINT3& _Left, const DirectX::XMUINT3& _Right) const {
            return
                _Left.x == _Right.x &&
                _Left.y == _Right.y &&
                _Left.z == _Right.z;
        }
    };
}
template<size_t stride, size_t count>
inline __m256 __vectorcall Load256(void* source)
{
    __m256 res{};
    unsigned char* s{ reinterpret_cast<unsigned char*>(source) };
    for (size_t i = 0; i < count; ++i)
    {
        res.m256_f32[i] = *reinterpret_cast<float*>(s);
        s += stride;
    }
    return res;
}
template<size_t stride, size_t count>
inline void __vectorcall Store256(__m256 source, void* dest)
{
    unsigned char* destination{ reinterpret_cast<unsigned char*>(dest) };
    for (size_t i = 0; i < count; ++i)
    {
        *reinterpret_cast<float*>(destination) = source.m256_f32[i];
        destination += stride;
    }
}
namespace Kumazuma
{
	OBJMeshImpl::OBJMeshImpl(GraphicsModule* gmodule, wchar_t const* objFilePath)
	{
		std::vector<StaticMeshVertex> vetices;

        HRESULT hr{};
        std::vector<XMFLOAT3A> positions;
        std::vector<XMFLOAT2A> UVs;
        std::vector<XMFLOAT3A> normals;
        std::filesystem::path path{ objFilePath };
        std::filesystem::path dir{ path.parent_path() };

        std::unordered_map<DirectX::XMUINT3, size_t> itemIndexTable;
        tinyobj::ObjReaderConfig readerConfig;
        readerConfig.mtl_search_path = dir.generic_u8string();
        readerConfig.triangulate = true;
        tinyobj::ObjReader reader;
        if (!reader.ParseFromFile(ConvertWideToUTF8(path), readerConfig))
        {
            throw E_FAIL;
        }
        auto& attrib = reader.GetAttrib();
#pragma region push_attribute_in
        positions.reserve(attrib.vertices.size() / 3);
        for (auto i = 0; i < attrib.vertices.size() / 3; ++i)
        {
            XMFLOAT3A pos{
                attrib.vertices[3 * i + 0],
                attrib.vertices[3 * i + 1],
                attrib.vertices[3 * i + 2]
            };
            positions.emplace_back(pos);
        }

        UVs.reserve(attrib.texcoords.size() / 2);
        for (auto i = 0; i < attrib.texcoords.size() / 2; ++i)
        {
            XMFLOAT2A uv{
                attrib.texcoords[2 * i + 0],
                attrib.texcoords[2 * i + 1]
            };
            UVs.emplace_back(uv);
        }

        normals.reserve(attrib.normals.size() / 3);
        for (auto i = 0; i < attrib.normals.size() / 3; ++i)
        {
            XMFLOAT3A pos{
                attrib.normals[3 * i + 0],
                attrib.normals[3 * i + 1],
                attrib.normals[3 * i + 2]
            };
            normals.emplace_back(pos);
        }
#pragma endregion
#pragma region modify_position

        //Z축을 뒤집는다.
        __m256 _1{ -1.f ,-1.f , -1.f , -1.f , -1.f , -1.f, -1.f ,-1.f };
        __m256 Zs{};
        assert(positions.size() <= std::numeric_limits<u32>::max());
        size_t length = positions.size() / 8;
        for (size_t i = 0; i < length; ++i)
        {
            Zs = Load256<sizeof(DirectX::XMFLOAT3A), 8>(&positions[i * 8].z);
            Zs = _mm256_mul_ps(Zs, _1);
            Store256<sizeof(DirectX::XMFLOAT3A), 8>(Zs, &positions[i * 8].z);
        }
        length *= 8;
        for (size_t i = 0; i < positions.size() - length; ++i)
        {
            Zs.m256_f32[i] = positions[i + length].z;
        }
        Zs = _mm256_mul_ps(Zs, _1);
        for (size_t i = 0; i < positions.size() - length; ++i)
        {
            positions[i + length].z = Zs.m256_f32[i];
        }
#pragma endregion
#pragma region modify normal vector
        //normal의 Z값을 뒤집어 줘야 한다.
        length = normals.size() / 8;
        for (size_t i = 0; i < length; ++i)
        {
            Zs = Load256<sizeof(DirectX::XMFLOAT3A), 8>(&normals[i * 8].z);
            Zs = _mm256_mul_ps(Zs, _1);
            Store256<sizeof(DirectX::XMFLOAT3A), 8>(Zs, &normals[i * 8].z);
        }
        length *= 8;
        for (size_t i = 0; i < normals.size() - length; ++i)
        {
            Zs.m256_f32[i] = normals[i + length].z;
        }
        Zs = _mm256_mul_ps(Zs, _1);
        for (size_t i = 0; i < normals.size() - length; ++i)
        {
            normals[i + length].z = Zs.m256_f32[i];
        }
#pragma endregion
#pragma region modify uv
        //-1에서 1로 바꾸고
        //UV의 V를 뒤집어 줘야 한다.
        _1 = _mm256_mul_ps(_1, _1);
        length = UVs.size() / 8;
        for (size_t i = 0; i < length; ++i)
        {
            Zs = Load256<sizeof(DirectX::XMFLOAT2A), 8>(&UVs[i * 8].y);
            Zs = _mm256_sub_ps(_1, Zs);
            Store256<sizeof(DirectX::XMFLOAT2A), 8>(Zs, &UVs[i * 8].y);
        }
        length *= 8;
        for (size_t i = 0; i < UVs.size() - length; ++i)
        {
            Zs.m256_f32[i] = UVs[i + length].y;
        }
        Zs = _mm256_sub_ps(_1, Zs);
        for (size_t i = 0; i < UVs.size() - length; ++i)
        {
            UVs[i + length].y = Zs.m256_f32[i];
        }
#pragma endregion
        auto& shapes = reader.GetShapes();
        auto& materials = reader.GetMaterials();
        //버텍스 버퍼를 구축하자.
        std::vector<StaticMeshVertex> vertices;
        std::vector<u32> indices;
        vertices.reserve(positions.size());
        size_t indexCount{ 0 };
        for (auto& shape : shapes)
        {
            indexCount += shape.mesh.num_face_vertices.size();
        }
        indices.reserve(indexCount);
        for (auto& shape : shapes)
        {
            size_t indexBase{ indices.size() };
            for (size_t faceIndex = 0; faceIndex < shape.mesh.num_face_vertices.size(); ++faceIndex)
            {
                int fv = shape.mesh.num_face_vertices[faceIndex];

                for (size_t v = 0; v < fv; ++v)
                {
                    tinyobj::index_t idx = shape.mesh.indices[faceIndex*3 + v];
                    XMUINT3 index;
                    index.x = static_cast<u32>(idx.vertex_index);
                    index.y = static_cast<u32>(idx.normal_index);
                    index.z = static_cast<u32>(idx.texcoord_index);

                    auto findIt = itemIndexTable.find(index);
                    if (findIt == itemIndexTable.end())
                    {
                        size_t newVertexIndex{ vertices.size() };
                        auto& pos = positions[index.x];
                        auto& normal = normals[index.y];
                        auto& uv = UVs[index.z];
                        StaticMeshVertex newVertex{};
                        newVertex.vNormal = normal;
                        newVertex.vTex = uv;
                        newVertex.vPos = pos;

                        vertices.emplace_back(newVertex);
                        vertexPositions.emplace_back(pos);
                        findIt = itemIndexTable.emplace(index, newVertexIndex).first;
                    }
                    indices.emplace_back(findIt->second);
                }
            }

            std::wstring name = ConvertUTF8ToWide(shape.name);
            std::wstring materialName = ConvertUTF8ToWide(reader.GetMaterials()[shape.mesh.material_ids[0]].name);
            OBJSubset objSubset;
            objSubset.indexBase_ = indexBase;
            objSubset.triangleCount_ = shape.mesh.num_face_vertices.size();
            objSubset.materialName = materialName;
            objSubset.name = name;
            subsets_.subsets_.emplace_back(std::move(objSubset));
        }
        for (size_t i = 0; i < indices.size() / 3; ++i)
        {
            std::swap(indices[i * 3], indices[i * 3 + 2]);
        }
        vertices.shrink_to_fit();
        indices.shrink_to_fit();
        ComPtr<ID3D11Device> device;
        gmodule->GetDevice(&device);
        
        CD3D11_BUFFER_DESC vertexBufferDesc = CD3D11_BUFFER_DESC(sizeof(StaticMeshVertex) * vertexPositions.size(), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT);
        CD3D11_BUFFER_DESC indexBufferDesc = CD3D11_BUFFER_DESC(sizeof(u32) * indices.size(), D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT);
        D3D11_SUBRESOURCE_DATA subresourceData{};
        subresourceData.pSysMem = vertices.data();
        hr = device->CreateBuffer(&vertexBufferDesc, &subresourceData, &vertexBuffer_);
        assert(SUCCEEDED(hr));
        subresourceData.pSysMem = indices.data();
        hr = device->CreateBuffer(&indexBufferDesc, &subresourceData, &indexBuffer_);
        assert(SUCCEEDED(hr));
        TextureManager& manager = gmodule->GetTextureManager();
        for (auto& material : reader.GetMaterials())
        {
            auto textureName{ ConvertUTF8ToWide(material.diffuse_texname) };
            auto textureDirPath{ dir / textureName };
            auto textureDir{ textureDirPath.generic_wstring() };
            auto materialName{ ConvertUTF8ToWide(material.name) };
            auto texture{ std::unique_ptr<Texture2D>{manager.Load(textureDir.c_str())} };
            this->materialTextures_.emplace(materialName, std::move(texture));
        }
        for (OBJSubset& subset : subsets_.subsets_)
        {
            subset.vertexBuffer = vertexBuffer_;
            subset.indexBuffer = indexBuffer_;
            subset.mesh_ = this;
        }
	}
    //OBJSubsets subsets_;
    //ComPtr<ID3D11Buffer> vertexBuffer_;
    //ComPtr<ID3D11Buffer> indexBuffer_;
    //std::vector<DirectX::XMFLOAT3A> vertexPositions;
    //std::vector<u32> indices;
    //std::unordered_map<std::wstring, std::unique_ptr<Texture2D> > materialTextures_;
    OBJMeshImpl::OBJMeshImpl(OBJMeshImpl&& rhs) noexcept:
        subsets_{std::move(rhs.subsets_)},
        vertexBuffer_{std::move(rhs.vertexBuffer_)},
        indexBuffer_{std::move(rhs.indexBuffer_)},
        vertexPositions{std::move(rhs.vertexPositions)},
        indices{std::move(rhs.indices)},
        materialTextures_{std::move(rhs.materialTextures_)}
    {
    }

    OBJMeshImpl::~OBJMeshImpl()
    {
    }

	void OBJMeshImpl::SetupIA(ID3D11DeviceContext* device)
	{
		UINT stride{ static_cast<UINT>(sizeof(StaticMeshVertex)) };
		UINT offset{ 0 };
		device->IASetIndexBuffer(indexBuffer_.Get(), DXGI_FORMAT_R32_UINT, sizeof(u32));
		device->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
		device->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	Subsets const& OBJMeshImpl::GetSubsetsRef()
	{
		return subsets_;
	}
    Texture2D* OBJMeshImpl::GetMaterialTexture(wchar_t const* materialName)
    {
        auto it{ materialTextures_.find(materialName) };
        if (it != materialTextures_.end())
        {
            return it->second.get();
        }
        return nullptr;
    }
}
