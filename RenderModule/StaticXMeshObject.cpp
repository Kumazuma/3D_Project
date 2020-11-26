#include "pch.h"
#include "StaticXMeshObject.h"
#include <Windows.h>
#include "RenderModule.h"
#include <array>
using namespace DirectX;
StaticXMeshObject::StaticXMeshObject()
{

}

auto StaticXMeshObject::Initialize(RenderModule* pRenderModule, std::wstring const& filePath) -> HRESULT
{
    HRESULT hr{};
    DWORD subsetCount{};
    COMPtr<IDirect3DDevice9> pDevice;
    pRenderModule->GetDevice(&pDevice);
    hr = D3DXLoadMeshFromXW(
        filePath.c_str(),
        D3DXMESH_MANAGED,
        pDevice.Get(),
        &m_pAdjacency,
        &m_pSubset,
        nullptr,
        &subsetCount,
        &m_pMesh
    );
    if (FAILED(hr))
        return hr;
    m_subsetCount = static_cast<u32>(subsetCount);
    DWORD meshFVF = m_pMesh->GetFVF();
    if (!(meshFVF & D3DFVF_NORMAL))
    {
        COMPtr<ID3DXMesh> tmp;
        m_pMesh->CloneMeshFVF(m_pMesh->GetOptions(), meshFVF | D3DFVF_NORMAL, pDevice.Get(), &tmp);
        D3DXComputeNormals(tmp.Get(), reinterpret_cast<DWORD const*>(m_pAdjacency->GetBufferPointer()));
        m_pMesh = tmp;
    }
    meshFVF |= D3DFVF_NORMAL;
    u8 * pVertices{};
    m_vertexCount = m_pMesh->GetNumVertices();
    m_vertices.reserve(m_vertexCount);
    std::array<D3DVERTEXELEMENT9, MAX_FVF_DECL_SIZE> decls{};
    m_pMesh->GetDeclaration(decls.data());
    hr = m_pMesh->LockVertexBuffer(0, &reinterpret_cast<void *&>( pVertices) );
    assert(SUCCEEDED(hr));
    if (FAILED(hr))
        return hr;
    if (pVertices == nullptr)
        return E_FAIL;
    u32 byteOffset{};
    for (auto& decl : decls)
    {
        if (decl.Usage != D3DDECLUSAGE_POSITION)
            continue;
        byteOffset = decl.Offset;
    }
    m_stride = D3DXGetFVFVertexSize(meshFVF);
    for (size_t i = 0; i < m_vertexCount; ++i)
    {
        XMFLOAT3 const* pos =
            reinterpret_cast<XMFLOAT3 const*>(pVertices + i * m_stride + byteOffset);
        assert(pos != nullptr);
        m_vertices.push_back(XMFLOAT3A{ pos->x, pos->y, pos->z });
        
    }
    hr = m_pMesh->UnlockVertexBuffer();
    assert(SUCCEEDED(hr));
    if (FAILED(hr))
        return hr;
    m_pMaterials = reinterpret_cast<D3DXMATERIAL const*>(m_pSubset->GetBufferPointer());
    m_textures.assign(m_subsetCount, COMPtr<IDirect3DTexture9>{});

    return S_OK;
}

auto StaticXMeshObject::Create(RenderModule* pRenderModule, std::wstring const& filePath, StaticXMeshObject** pOut) -> HRESULT
{
    auto* pObj{ new StaticXMeshObject{} };
    HRESULT hr{};
    hr = pObj->Initialize(pRenderModule, filePath);
    assert(SUCCEEDED(hr));
    if (FAILED(hr))
    {
        delete pObj;
        pObj = nullptr;
    }
    *pOut = pObj;
    return hr;
}

auto StaticXMeshObject::Render(RenderModule* pRenderModule) -> void
{ 
    COMPtr<IDirect3DTexture9> pTexture;
    COMPtr<IDirect3DDevice9> pDevice;
    pRenderModule->GetDevice(&pDevice);
    pDevice->SetTransform(D3DTS_WORLD, &reinterpret_cast<D3DMATRIX&>(m_transform));

    for (u32 i = 0; i < m_subsetCount; ++i)
    {
        pTexture = m_textures[i];
        if (pTexture == nullptr)
        {
            pRenderModule->GetDefaultTexture(&pTexture);
        }
        pDevice->SetTexture(0, pTexture.Get());
        m_pMesh->DrawSubset(i);
    }
}

auto StaticXMeshObject::Clone() const -> RenderObject*
{
    return new StaticXMeshObject{ *this };
}
auto StaticXMeshObject::GetMaterialCount() const -> u32
{
    return this->m_subsetCount;
}