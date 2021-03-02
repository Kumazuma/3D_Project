#include "pch.h"
#include "MeshImpl.hpp"
#include "UnicodeHelper.h"
namespace Kumazuma
{
    struct StaticMeshVertex
    {
        DirectX::XMFLOAT3 vPos;
        DirectX::XMFLOAT2 vTex;
        DirectX::XMFLOAT3 vNormal;
    };
    StaticMeshImpl::StaticMeshImpl(MeshCollection* collection, GraphicsModule* const gmodule, aiScene const* scene, aiMesh* mesh):
        collection{collection}
    {
        ComPtr<ID3D11Device> device;
        std::vector<StaticMeshVertex> vertices;
        std::vector<u32> indices;
        vertices.reserve(mesh->mNumVertices);
        indices.reserve(mesh->mNumFaces * 3);
        gmodule->GetDevice(&device);
        materialName = ConvertUTF8ToWide(scene->mMaterials[ mesh->mMaterialIndex]->GetName().C_Str() );
        name = ConvertUTF8ToWide(mesh->mName.C_Str());
        for (u32 i = 0; i < mesh->mNumVertices; ++i)
        {
            StaticMeshVertex vertex;
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
            vertices.push_back(vertex);
        }
        for (u32 i = 0; i < mesh->mNumFaces; ++i)
        {
            aiFace face = mesh->mFaces[i];

            for (u32 j = 0; j < face.mNumIndices; ++j)
                indices.push_back(face.mIndices[j]);
        }
        auto vertexBufferDesc{ CD3D11_BUFFER_DESC(sizeof(StaticMeshVertex) * vertices.size(), D3D11_BIND_VERTEX_BUFFER) };
        auto indexBufferDesc{ CD3D11_BUFFER_DESC(sizeof(u32) * indices.size(), D3D11_BIND_INDEX_BUFFER) };
        D3D11_SUBRESOURCE_DATA subResource{};
        HRESULT hr{};
        subResource.pSysMem = vertices.data();
        hr = device->CreateBuffer(&vertexBufferDesc, &subResource, &vertexBuffer);

        subResource.pSysMem = indices.data();
        hr = device->CreateBuffer(&indexBufferDesc, &subResource, &indexBuffer);

        indexCount = indices.size();
    }

    u32 StaticMeshImpl::GetIndexCount() const
    {
        return indexCount;
    }

    u32 StaticMeshImpl::GetIndexBase() const
    {
        return 0;
    }

    HRESULT StaticMeshImpl::GetVertexBuffer(ID3D11Buffer** out) const
    {
        if (out == nullptr)return E_POINTER;
        ID3D11Buffer* buffer = vertexBuffer.Get();
        *out = buffer;
        buffer->AddRef();
        return S_OK;
    }

    HRESULT StaticMeshImpl::GetIndexBuffer(ID3D11Buffer** out) const
    {
        if (out == nullptr)return E_POINTER;
        ID3D11Buffer* buffer = indexBuffer.Get();
        *out = buffer;
        buffer->AddRef();
        return S_OK;
    }

    std::wstring const& StaticMeshImpl::GetMaterial() const
    {
        return materialName;
    }

    std::wstring const& StaticMeshImpl::GetName() const
    {
        return name;
    }

    MeshType StaticMeshImpl::GetType() const
    {
        return MeshType::Static;
    }

    HRESULT StaticMeshImpl::SetupIA(ID3D11DeviceContext* context) const
    {
        UINT stride{ static_cast<UINT>(sizeof(StaticMeshVertex)) };
        UINT offset{ 0 };
        ID3D11Buffer* vertexBuffer{ this->vertexBuffer.Get() };
        context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        return S_OK;
    }

    MeshCollection* StaticMeshImpl::GetMeshCollection() const
    {
        return collection;
    }

}
