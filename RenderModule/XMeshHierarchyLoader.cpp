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
    pNewFrame->combinedTransformationMatrix.reset(new XMFLOAT4X4{});
    XMStoreFloat4x4(pNewFrame->combinedTransformationMatrix.get(),
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
    HRESULT hr{};
    if (ppNewMeshContainer == nullptr)
    {
        return E_POINTER;
    }
    CustomMeshContainer* pNewMeshContainer{ new CustomMeshContainer{} };
    AllocateName(&pNewMeshContainer->Name, szName);
    
    COMPtr<ID3DXMesh> pMesh = MakeCOMPtr(std::move( (ID3DXMesh*) pMeshData->pMesh));
    u32 faceCount{ pMesh->GetNumFaces() };//폴리곤의 갯수
    pNewMeshContainer->adjacency.assign(pAdjacency, pAdjacency + faceCount * 3);
    DWORD fvf = pMesh->GetFVF();
    pMesh->CloneMeshFVF(pMesh->GetOptions(), fvf | D3DFVF_NORMAL, m_pDevice.Get(), &pNewMeshContainer->MeshData.pMesh);
    if (!(fvf & D3DFVF_NORMAL))
    {
        // 각 폴리곤 또는 정점 기준의 법선을 계산하여 삽입해주는 함수
        D3DXComputeNormals(pNewMeshContainer->MeshData.pMesh, pNewMeshContainer->pAdjacency);
    }
    if (NumMaterials == 0)
    {
        D3DXMATERIAL material{};
        material.MatD3D.Diffuse = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
        material.MatD3D.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
        material.MatD3D.Ambient = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
        material.MatD3D.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
        material.MatD3D.Power = 0.f;
        pNewMeshContainer->textures.push_back(COMPtr<IDirect3DTexture9>{});
        pNewMeshContainer->materials.push_back(material);
    }
    else
    {
        pNewMeshContainer->materials.assign(pMaterials, pMaterials + NumMaterials);
        pNewMeshContainer->textures.resize(NumMaterials);
        wchar_t szFullPath[256]{};
        wchar_t szFileName[256]{};

        for (u32 i = 0; i < NumMaterials; ++i)
        {
            MultiByteToWideChar(CP_ACP,
                0,
                pNewMeshContainer->materials[i].pTextureFilename,
                strlen(pNewMeshContainer->pMaterials[i].pTextureFilename),
                szFileName,
                256);
            lstrcpy(szFullPath, m_path.c_str());
            lstrcat(szFullPath, szFileName);
            hr = D3DXCreateTextureFromFileW(
                m_pDevice.Get(),
                szFullPath,
                &pNewMeshContainer->textures[i]
            );
            assert(SUCCEEDED(hr));
            if (FAILED(hr))
            {
                return hr;
            }

        }
    }
    pNewMeshContainer->NumMaterials = pNewMeshContainer->materials.size();
    pNewMeshContainer->pMaterials = pNewMeshContainer->materials.data();
    pNewMeshContainer->pAdjacency = pNewMeshContainer->adjacency.data();
    if (pSkinInfo == nullptr)
    {
        return S_OK;
    }
    pNewMeshContainer->pSkinInfo = pSkinInfo;
    pSkinInfo->AddRef();
    pMesh->CloneMeshFVF(pNewMeshContainer->MeshData.pMesh->GetOptions(), pNewMeshContainer->MeshData.pMesh->GetFVF(), m_pDevice.Get(), &pNewMeshContainer->pOriginalMesh);
    pNewMeshContainer->boneCount = pSkinInfo->GetNumBones();
    XMFLOAT4X4 defaultMatrix{};
    pNewMeshContainer->frameCombinedMatries.assign(pNewMeshContainer->boneCount, nullptr);
    pNewMeshContainer->frameOffsetMatries.assign(pNewMeshContainer->boneCount, defaultMatrix);
    pNewMeshContainer->renderingMatries.assign(pNewMeshContainer->boneCount, defaultMatrix);
    for (u32 i = 0; i < pNewMeshContainer->boneCount; ++i)
    {
        pNewMeshContainer->frameOffsetMatries[i] = 
            reinterpret_cast<XMFLOAT4X4 const&>(*pSkinInfo->GetBoneOffsetMatrix(i));
        // 메쉬를 그리기 위한 뼈대들은 고유의 인덱스 값을 가지고 있음. 그래서 순회하면서 행렬 정보를 차례대로 얻어올 수 있다.

    }
    *ppNewMeshContainer = pNewMeshContainer;
    return S_OK;
}

auto __stdcall SkinnedXMeshObject::HierarchyLoader::DestroyFrame(LPD3DXFRAME const pFrameToFree) -> HRESULT 
{
    if (pFrameToFree == nullptr)return S_OK;
    delete[] pFrameToFree->Name;
    if (pFrameToFree->pMeshContainer != nullptr)
        DestroyMeshContainer(pFrameToFree->pMeshContainer);
    if (pFrameToFree->pFrameSibling != nullptr)
        DestroyFrame(pFrameToFree->pFrameSibling);
    if (pFrameToFree->pFrameFirstChild != nullptr)
        DestroyFrame(pFrameToFree->pFrameFirstChild);
    delete pFrameToFree;
    return S_OK;
}

auto __stdcall SkinnedXMeshObject::HierarchyLoader::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree) -> HRESULT 
{

    pMeshContainerToFree->MeshData.pMesh->Release();
    pMeshContainerToFree->pSkinInfo->Release();

    CustomMeshContainer* pMeshContainer = static_cast<CustomMeshContainer*>(pMeshContainerToFree);
    delete pMeshContainer;
    return E_NOTIMPL;

}

auto SkinnedXMeshObject::HierarchyLoader::AllocateName(char** ppName, char const* pFrameName) -> void
{
    if (nullptr == pFrameName)
        return;
    usize len{ strlen(pFrameName) };
    std::wstring a;
    
    *ppName = new i8[len + 1];
    strcpy_s(*ppName, len + 1, pFrameName);
}

SkinnedXMeshObject::HierarchyLoader::HierarchyLoader(IDirect3DDevice9Ex* pDevice, std::wstring const& path):
    m_pDevice{MakeCOMPtr(std::move(pDevice))},
    m_path{ path }
{

}

auto SkinnedXMeshObject::HierarchyLoader::Create(IDirect3DDevice9Ex* pGraphicesDevice, std::wstring const& path, HierarchyLoader** pOut) -> HRESULT
{
    if (pOut == nullptr)
    {
        return E_POINTER;
    }
    auto obj{ new HierarchyLoader(pGraphicesDevice, path) };
    *pOut = obj;
    return S_OK;
}
