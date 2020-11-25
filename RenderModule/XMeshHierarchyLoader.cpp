#include "pch.h"
#include "XMeshHierarchyLoader.h"
using namespace DirectX;
auto __stdcall SkinnedXMeshObject::HierarchyLoader::CreateFrame(LPCSTR szName, D3DXFRAME** ppNewFrame) -> HRESULT 
{
    if (ppNewFrame == nullptr)
    {
        return E_POINTER;
    }
    SkinnedXMeshObject::Frame* pNewFrame{ new SkinnedXMeshObject::Frame {} };
    AllocateName(&pNewFrame->Name, szName);
    XMStoreFloat4x4(&pNewFrame->combinedTransformationMatrix,
        XMMatrixIdentity());
    *ppNewFrame = pNewFrame;
    return S_OK;
}

auto __stdcall SkinnedXMeshObject::HierarchyLoader::CreateMeshContainer(
    LPCSTR szName,
    const D3DXMESHDATA* pMeshData,
    const D3DXMATERIAL* pMaterials,
    const D3DXEFFECTINSTANCE* pEffectInstances,
    DWORD NumMaterials,
    const DWORD* pAdjacency,
    LPD3DXSKININFO pSkinInfo,
    LPD3DXMESHCONTAINER* ppNewMeshContainer) -> HRESULT 
{
    CustomMeshContainer* pNewMeshContainer{ new CustomMeshContainer{} };
    AllocateName(&pNewMeshContainer->Name, szName);
    
    COMPtr<ID3DXMesh> pMesh = MakeCOMPtr(std::move( (ID3DXMesh*) pMeshData->pMesh));
    u32 faceCount{ pMesh->GetNumFaces() };//폴리곤의 갯수
    pNewMeshContainer->adjacency.assign(pAdjacency, pAdjacency + faceCount * 3);
    pNewMeshContainer->pAdjacency = pNewMeshContainer->adjacency.data();
    DWORD fvf = pMesh->GetFVF();
    pMesh->CloneMeshFVF(pMesh->GetOptions(), fvf | D3DFVF_NORMAL, m_pDevice.Get(), &pNewMeshContainer->MeshData.pMesh);
    if (!(fvf & D3DFVF_NORMAL))
    {
        // 각 폴리곤 또는 정점 기준의 법선을 계산하여 삽입해주는 함수
        D3DXComputeNormals(pNewMeshContainer->MeshData.pMesh, pNewMeshContainer->pAdjacency);
    }
    pNewMeshContainer->NumMaterials = (NumMaterials == 0 ? 1 : NumMaterials);

    return S_OK;
}

auto __stdcall SkinnedXMeshObject::HierarchyLoader::DestroyFrame(LPD3DXFRAME pFrameToFree) -> HRESULT 
{
    return E_NOTIMPL;
}

auto __stdcall SkinnedXMeshObject::HierarchyLoader::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree) -> HRESULT 
{
    return E_NOTIMPL;

}

auto SkinnedXMeshObject::HierarchyLoader::AllocateName(char** ppName, char const* pFrameName) -> void
{
}

auto SkinnedXMeshObject::HierarchyLoader::Create(IDirect3DDevice9Ex* pGraphicesDevice, std::wstring const& path, HierarchyLoader** pOut) -> HRESULT
{
    return E_NOTIMPL;
}
