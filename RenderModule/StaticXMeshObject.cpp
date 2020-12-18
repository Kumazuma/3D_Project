#include "pch.h"
#include "StaticXMeshObject.h"
#include <Windows.h>
#include "RenderModule.h"
#include <array>
#include "Renderer.h"
using namespace DirectX;
StaticXMeshObject::StaticXMeshObject()
{

}
//COMPtr<ID3DXMesh> m_pMesh;// 노말 정보를 삽입하여 변환시킨 메쉬 컴객체
//COMPtr<ID3DXBuffer> m_pAdjacency;
//COMPtr<ID3DXBuffer> m_pSubset;
//std::vector<DirectX::XMFLOAT3A> m_vertices;
//std::vector<std::shared_ptr<Entity> > m_entities;
StaticXMeshObject::StaticXMeshObject(StaticXMeshObject const& rhs):
    XMeshObject{ rhs },
    m_pAdjacency{rhs.m_pAdjacency},
    m_pMesh{rhs.m_pMesh},
    m_pSubset{rhs.m_pSubset},
    m_pVertices{rhs.m_pVertices}
{
    for (u32 i = 0; i < m_subsetCount; ++i)
    {
        m_entities.emplace_back(new Entity{ this, i });
    }
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
    m_pVertices.reset(new std::vector < XMFLOAT3A>{});
    m_pVertices->reserve(m_vertexCount);
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
        m_pVertices->push_back(XMFLOAT3A{ pos->x, pos->y, pos->z });
        
    }
    hr = m_pMesh->UnlockVertexBuffer();
    assert(SUCCEEDED(hr));
    if (FAILED(hr))
        return hr;
    m_pMaterials = reinterpret_cast<D3DXMATERIAL const*>(m_pSubset->GetBufferPointer());
    m_textures.assign(m_subsetCount, COMPtr<IDirect3DTexture9>{});
    for (u32 i = 0; i < m_subsetCount; ++i)
    {
        m_entities.emplace_back(new Entity{ this, i });
    }
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

auto StaticXMeshObject::PrepareRender(IRenderer* pRenderer) -> void
{ 
    for (auto it : m_entities)
    {
        if (it->IsEnableAlpha())
        {
            pRenderer->AddEntity(RenderModule::Kind::ALPHA, it);
        }
        else
        {
            pRenderer->AddEntity(RenderModule::Kind::NONALPHA, it);
        }
    }
}

auto StaticXMeshObject::Clone() const -> RenderObject*
{
    return new StaticXMeshObject{ *this };
}
auto StaticXMeshObject::SetEnableSubsetAlpha(u32 idx, bool enable) -> void
{
    m_entities[idx]->EnableAlpha(enable);
}
auto StaticXMeshObject::GetMaterialCount() const -> u32
{
    return this->m_subsetCount;
}

StaticXMeshObjectSubset::StaticXMeshObjectSubset(StaticXMeshObject* mesh, u32 idx)
{
    m_pMeshObject = mesh;
    m_subsetIndex = idx;
    m_enableAlpha = false;
}

auto StaticXMeshObjectSubset::Render(RenderModule* pRenderModule, IRenderer* pRenderer) -> void
{
    COMPtr<IDirect3DTexture9> pTexture;
    COMPtr<IDirect3DDevice9> pDevice;
    COMPtr<ID3DXEffect> pEffect;
    pRenderModule->GetDevice(&pDevice);
    pRenderer->GetEffect(&pEffect);
    
    pDevice->SetTransform(D3DTS_WORLD, &reinterpret_cast<D3DMATRIX&>(m_pMeshObject->m_transform));
    pTexture = m_pMeshObject->m_textures[m_subsetIndex];
    if (pTexture == nullptr)
    {
        pRenderModule->GetDefaultTexture(&pTexture);
    }
    auto& rMaterial{ m_pMeshObject->m_pMaterials[m_subsetIndex].MatD3D };
    auto specularColor{ rMaterial.Specular };
    D3DXVECTOR4 specularVec{ specularColor.r,specularColor.g, specularColor.b, rMaterial.Power };

    XMMATRIX mNormalWorld{ XMLoadFloat4x4(&m_pMeshObject->m_transform) };
    mNormalWorld.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);
    mNormalWorld = XMMatrixTranspose(XMMatrixInverse(nullptr, mNormalWorld));
    pEffect->SetMatrix("g_mNormalWorld", reinterpret_cast<D3DXMATRIX*>(&mNormalWorld));
    pEffect->SetVector("g_vSpecular", &specularVec);
    pEffect->SetMatrix("g_mWorld", &reinterpret_cast<D3DXMATRIX&>(m_pMeshObject->m_transform));
    pEffect->SetTexture("g_diffuseTexture", pTexture.Get());
    
    pEffect->CommitChanges();
    m_pMeshObject->m_pMesh->DrawSubset(m_subsetIndex);
}

auto StaticXMeshObjectSubset::EnableAlpha(bool enabled) -> void
{
    m_enableAlpha = enabled;
}

auto StaticXMeshObjectSubset::IsEnableAlpha() const -> bool 
{
    return m_enableAlpha;
}
