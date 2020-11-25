#pragma once
#include"SkinnedXMeshObject.h"
class SkinnedXMeshObject::HierarchyLoader : public ID3DXAllocateHierarchy
{
public:
	auto __stdcall CreateFrame(LPCSTR szName, D3DXFRAME** ppNewFrame)->HRESULT override;
	auto __stdcall CreateMeshContainer(
        LPCSTR Name,
        CONST D3DXMESHDATA* pMeshData,
        CONST D3DXMATERIAL* pMaterials,
        CONST D3DXEFFECTINSTANCE* pEffectInstances,
        DWORD NumMaterials,
        CONST DWORD* pAdjacency,
        LPD3DXSKININFO pSkinInfo,
        LPD3DXMESHCONTAINER* ppNewMeshContainer
	)->HRESULT override;
    auto __stdcall DestroyFrame(LPD3DXFRAME pFrameToFree)->HRESULT override;
    auto __stdcall DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)->HRESULT override;
private:
    auto AllocateName(char** ppName, char const* pFrameName)->void;
    HierarchyLoader(IDirect3DDevice9Ex* pDevice, std::wstring const& path);
private:
    COMPtr<IDirect3DDevice9Ex> m_pDevice;
    std::wstring m_path;
public:
    static auto Create(IDirect3DDevice9Ex* pGraphicesDevice, std::wstring const& path, HierarchyLoader** pOut)->HRESULT;
public:

};
