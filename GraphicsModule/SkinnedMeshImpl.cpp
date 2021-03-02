#include "pch.h"
#include "SkinnedMeshImpl.hpp"
#include<vector>
#include<UnicodeHelper.h>
#include<array>
namespace Kumazuma
{
    struct SkinnedMeshVertex
    {
        DirectX::XMFLOAT3 vPos;
        DirectX::XMFLOAT2 vTex;
        DirectX::XMFLOAT3 vNormal;
        std::array<f32,4> boneWeights;
        std::array<u32,4> boneIndices;
    };
    SkinnedMeshImpl::SkinnedMeshImpl(MeshCollection* collection, GraphicsModule* const gmodule, aiScene const* scene, aiMesh* mesh) : collection{ collection }
    {
        ComPtr<ID3D11Device> device;
        std::vector<SkinnedMeshVertex> vertices;
        std::vector<u32> indices;
        vertices.reserve(mesh->mNumVertices);
        indices.reserve(mesh->mNumFaces * 3);
        gmodule->GetDevice(&device);
        materialName = ConvertUTF8ToWide(scene->mMaterials[mesh->mMaterialIndex]->GetName().C_Str());
        name = ConvertUTF8ToWide(mesh->mName.C_Str());
        for (u32 i = 0; i < mesh->mNumVertices; ++i)
        {
            SkinnedMeshVertex vertex;
            vertex.vPos = {
                mesh->mVertices[i].x,
                mesh->mVertices[i].y,
                mesh->mVertices[i].z
            };
            vertex.vTex = {
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            };
            vertex.vNormal = {
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z
            };
            std::vector<std::pair<u32, f32> > weights;
            weights.reserve(8);
            u32 meshBoneCount = mesh->mNumBones;
            for (u32 j = 0; j < meshBoneCount; ++j)
            {
                auto bone{ mesh->mBones[j] };
                f32 weight{};
                bool found{ false };
                u32 weightCount{ bone->mNumWeights };
                for (u32 k = 0; k < weightCount; ++k)
                {
                    if (bone->mWeights[k].mVertexId == i)
                    {
                        found = true;
                        weight = bone->mWeights[k].mWeight;
                    }
                }
                if (found)
                {
                    weights.emplace_back(j, weight);
                }
            }
            if (weights.size() > 4)
            {
                std::sort(weights.begin(), weights.end(),
                    [](std::pair<u32, f32> const& lhs, std::pair<u32, f32> const& rhs)->bool
                    {
                        return lhs.second > rhs.second;
                    });
                std::array<std::pair<u32, f32>, 4> newWeight( *reinterpret_cast<std::array<std::pair<u32, f32>,4> const*>( weights.data()) );
                f32 sum{};
                for (std::pair<u32, f32> it : newWeight)
                {
                    sum += it.second;
                }
                for (std::pair<u32, f32>& it : newWeight)
                {
                    it.second = it.second / sum;
                }
                weights = std::vector<std::pair<u32, f32>>(newWeight.begin(), newWeight.end());
            }
            u32 boneCount = std::min(weights.size(), (size_t)4);
            for (u32 i = 0; i < boneCount; ++i)
            {
                vertex.boneIndices[i] = weights[i].first;
                vertex.boneWeights[i] = weights[i].second;
            }
            vertices.push_back(vertex);
        }
        for (u32 i = 0; i < mesh->mNumFaces; ++i)
        {
            aiFace face = mesh->mFaces[i];

            for (u32 j = 0; j < face.mNumIndices; ++j)
                indices.push_back(face.mIndices[j]);
        }
        auto vertexBufferDesc{ CD3D11_BUFFER_DESC(sizeof(SkinnedMeshVertex) * vertices.size(), D3D11_BIND_VERTEX_BUFFER) };
        auto indexBufferDesc{ CD3D11_BUFFER_DESC(sizeof(u32) * indices.size(), D3D11_BIND_INDEX_BUFFER) };
        D3D11_SUBRESOURCE_DATA subResource{};
        HRESULT hr{};
        subResource.pSysMem = vertices.data();
        hr = device->CreateBuffer(&vertexBufferDesc, &subResource, &vertexBuffer);

        subResource.pSysMem = indices.data();
        hr = device->CreateBuffer(&indexBufferDesc, &subResource, &indexBuffer);

        indexCount = indices.size();
    }
    u32 SkinnedMeshImpl::GetIndexCount() const
    {
        return indexCount;
    }

    u32 SkinnedMeshImpl::GetIndexBase() const
    {
        return 0;
    }

    HRESULT SkinnedMeshImpl::GetVertexBuffer(ID3D11Buffer** out) const
    {
        if (out == nullptr)return E_POINTER;
        ID3D11Buffer* buffer = vertexBuffer.Get();
        *out = buffer;
        buffer->AddRef();
        return S_OK;
    }

    HRESULT SkinnedMeshImpl::GetIndexBuffer(ID3D11Buffer** out) const
    {
        if (out == nullptr)return E_POINTER;
        ID3D11Buffer* buffer = indexBuffer.Get();
        *out = buffer;
        buffer->AddRef();
        return S_OK;

    }

    std::wstring const& SkinnedMeshImpl::GetMaterial() const
    {
        return materialName;
    }

    std::wstring const& SkinnedMeshImpl::GetName() const
    {
        return name;
    }

    MeshType Kumazuma::SkinnedMeshImpl::GetType() const
    {
        return MeshType::Skinned;
    }

    HRESULT SkinnedMeshImpl::SetupIA(ID3D11DeviceContext* context) const
    {
        UINT stride{ static_cast<UINT>(sizeof(SkinnedMeshVertex)) };
        UINT offset{ 0 };
        ID3D11Buffer* vertexBuffer{ this->vertexBuffer.Get() };
        context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        return S_OK;
    }

    MeshCollection* SkinnedMeshImpl::GetMeshCollection() const
    {
        return collection;
    }

}
