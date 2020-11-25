#pragma once
#include"SkinnedXMeshObject.h"
#include<unordered_set>
#include<unordered_map>
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
    ~HierarchyLoader();
private:
    auto AllocateName(char** ppName, char const* pFrameName)->void;
    HierarchyLoader(IDirect3DDevice9* pDevice, std::wstring const& path);
private:
    COMPtr<IDirect3DDevice9> m_pDevice;
    std::wstring m_path;
    std::unordered_map<CustomMeshContainer* , std::unique_ptr<CustomMeshContainer>> m_containers;
    std::unordered_map<Frame*, std::unique_ptr<Frame> > m_frames;
    
public:
    static auto Create(IDirect3DDevice9* pGraphicesDevice, std::wstring const& path, HierarchyLoader** pOut)->HRESULT;
public:

};
