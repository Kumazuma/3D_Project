#include "pch.h"
#include "OBJMesh.hpp"
#include "OBJSubsets.hpp"
namespace Kumazuma
{
    u32 OBJSubset::GetTriangleCount()const
    {
        return triangleCount_;
    }

    u32 OBJSubset::GetIndexBase()const
    {
        return indexBase_;
    }

    std::wstring const& OBJSubset::GetMaterial()const
    {
        return materialName;
    }

    std::wstring const& OBJSubset::GetName()const
    {
        return name;
    }

    HRESULT OBJSubset::GetVertexBuffer(ID3D11Buffer** out)const 
    {
        if (out == nullptr)
        {
            return E_POINTER;
        }
        
        ID3D11Buffer* buffer = vertexBuffer.Get();
        if (buffer == nullptr)
        {
            return E_FAIL;
        }
        *out = buffer;
        buffer->AddRef();
        return S_OK;
    }

    HRESULT OBJSubset::GetIndexBuffer(ID3D11Buffer** out)const
    {
        if (out == nullptr)
        {
            return E_POINTER;
        }

        ID3D11Buffer* buffer = indexBuffer.Get();
        if (buffer == nullptr)
        {
            return E_FAIL;
        }
        *out = buffer;
        buffer->AddRef();
        return S_OK;
    }

    Mesh& OBJSubset::GetMesh() const
    {
        return *mesh_;
    }

    u32 OBJSubsets::GetCount()
    {
        return static_cast<u32>( subsets_.size());
    }

    Subset const& OBJSubsets::Get(u32 index)
    {
        return subsets_[index];
    }
}

