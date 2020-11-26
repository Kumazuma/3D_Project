#include "pch.h"
#include "SkinnedXMeshObject.h"
#include "RenderModule.h"
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
using namespace DirectX;
inline auto __vectorcall ToFloat4x4(XMMATRIX mat)->XMFLOAT4X4
{
    XMFLOAT4X4 tmp{};
    XMStoreFloat4x4(&tmp, mat);
    return tmp;
}
auto SkinnedXMeshObject::Create(RenderModule* pRenderModule, std::wstring const& filePath, SkinnedXMeshObject** pOut) -> HRESULT
{
    if (pOut == nullptr)
    {
        return E_POINTER;
    }
    auto obj = new SkinnedXMeshObject{};
    HRESULT hr{};
    hr = obj->Initialize(pRenderModule, filePath);
    if (FAILED(hr))
    {
        delete obj;
        obj = nullptr;
    }
    *pOut = obj;
    return hr;
}

auto SkinnedXMeshObject::Render(RenderModule* pRenderModule) -> void
{
    COMPtr<IDirect3DDevice9> pDevice;
    pRenderModule->GetDevice(&pDevice);
    m_pAnimCtrler->AdjustAnimationToFrame();
    UpdateFrameMatrices(
        static_cast<Frame*>(m_pRootFrame),
        ToFloat4x4(XMMatrixRotationY(XMConvertToRadians(180.f)))
    );

    pDevice->SetTransform(D3DTS_WORLD, &reinterpret_cast<D3DMATRIX&>(m_transform));

    for (auto& iter : m_meshContainters)
    {
        for (u32 i = 0; i < iter->boneCount; ++i)
        {
            XMMATRIX mRenderingMatrix{};
            XMMATRIX mFrameOffset{};
            XMMATRIX mFrameCombinedMatrix{};
            mFrameOffset = XMLoadFloat4x4(&iter->frameOffsetMatries[i]);
            mFrameCombinedMatrix = XMLoadFloat4x4(iter->frameCombinedMatries[i].get());
            mRenderingMatrix = mFrameOffset * mFrameCombinedMatrix;
            XMStoreFloat4x4(&iter->renderingMatries[i], mRenderingMatrix);
        }

        void* pSrcVtx = nullptr;
        void* pDestVtx = nullptr;


        iter->pOriginalMesh->LockVertexBuffer(0, &pSrcVtx);
        iter->MeshData.pMesh->LockVertexBuffer(0, &pDestVtx);

        // 소프트웨어 스키닝을 수행하는 함수(스키닝 뿐 아니라 애니메이션 변경 시, 뼈대들과 정점 정보들의 변경을 동시에 수행하기도 함)
        iter->pSkinInfo->UpdateSkinnedMesh(
            reinterpret_cast<D3DXMATRIX const*>(iter->renderingMatries.data()),	// 최종 뼈의 변환상태 행렬
            nullptr,						// 원래 상태로 되돌리기 위한 상태 행렬(원래는 위 행렬의 역행렬을 구해서 넣어줘야 하지만 안넣어줘도 상관 없음)
            pSrcVtx,						// 변하지 않는 원본 메쉬의 정점 정보
            pDestVtx);						// 변환된 정보를 담기 위한 메쉬의 정점 정보


        for (u32 i = 0; i < iter->NumMaterials; ++i)
        {
            pDevice->SetTexture(0, iter->textures[i].Get() );
            iter->MeshData.pMesh->DrawSubset(i);
        }

        iter->pOriginalMesh->UnlockVertexBuffer();
        iter->MeshData.pMesh->UnlockVertexBuffer();
    }
}

auto SkinnedXMeshObject::Clone() const -> RenderObject*
{
    return new SkinnedXMeshObject{ this };
}

auto SkinnedXMeshObject::FindFrameTransfromByName(std::string const& frameName, XMFLOAT4X4 * const pOut) ->HRESULT
{
    if (pOut == nullptr)
    {
        return E_POINTER;
    }
    const auto findResIt{ m_renderedMatrices.find(frameName) };
    if(findResIt == m_renderedMatrices.end())
    {
        return E_FAIL;
    }
    *pOut = findResIt->second;
    return S_OK;

}

auto SkinnedXMeshObject::IsAnimationSetEnd() -> bool
{
    return m_pAnimCtrler->IsAnimationSetEnd();
}

auto SkinnedXMeshObject::SetAnimationSet(u32 idx) -> void
{
    m_pAnimCtrler->PlayAnimationSet(idx);
}

auto SkinnedXMeshObject::PlayAnimation(f32 timeDelta) -> void
{
    m_pAnimCtrler->AdvanceTime(timeDelta);
    
}

auto SkinnedXMeshObject::GetAnimationCount()const -> u32
{
    return m_pAnimCtrler->GetAnimCount();
}

SkinnedXMeshObject::SkinnedXMeshObject():
    m_pRootFrame{}
{

}

SkinnedXMeshObject::SkinnedXMeshObject(SkinnedXMeshObject const* rhs):
    m_pAnimCtrler{ rhs->m_pAnimCtrler->Clone() },
    m_pHierarchyLoader{rhs->m_pHierarchyLoader},
    m_meshContainters{rhs->m_meshContainters},
    m_pRootFrame{rhs->m_pRootFrame}
{
    //TODO: 깊은 복사를 해야한다. HOWTO?
}

auto SkinnedXMeshObject::Initialize(RenderModule* pRenderModule, std::wstring const& filePath) -> HRESULT
{

    HRESULT hr{};
    COMPtr<IDirect3DDevice9> pDevice;
    HierarchyLoader* pHierarchyLoader{};
    COMPtr<ID3DXAnimationController> pAnimationController{};
    AnimationController* pAnimCtrler{};
    D3DXFRAME* pRootFrame{};
    pRenderModule->GetDevice(&pDevice);
    usize idx = filePath.find_last_of(DIRECTORY_SEPARATOR);
    usize idx2 = filePath.find_last_of(L'/');
    usize idx3{};
    std::wstring directory{};
    
    if (idx2 == std::wstring::npos)
    {
        idx3 = idx;
    }
    else if (idx == std::wstring::npos)
    {
        idx3 = idx2;
    }
    else
    {
        idx3 = idx > idx2 ? idx : idx2;
    }
    directory = filePath.substr(0, idx3 + 1);
    hr = HierarchyLoader::Create(pDevice.Get(), directory, &pHierarchyLoader);
    if(FAILED(hr))
    {
        return hr;
    }
    m_pHierarchyLoader.reset(pHierarchyLoader);

    hr = D3DXLoadMeshHierarchyFromXW(
        filePath.c_str(),
        D3DXMESH_MANAGED,
        pDevice.Get(),
        m_pHierarchyLoader.get(),
        nullptr,
        &m_pRootFrame,
        &pAnimationController);
    if (FAILED(hr))
    {
        return hr;
    }
    hr = AnimationController::Create(pAnimationController.Get(), &pAnimCtrler);
    
    if (FAILED(hr))
    {
        return hr;
    }
    m_pAnimCtrler.reset(pAnimCtrler);
    UpdateFrameMatrices(
        static_cast<Frame*>(m_pRootFrame),
        ToFloat4x4(XMMatrixRotationY(XMConvertToRadians(180.f)))
    );
    InitializeFrameMatrix(static_cast<Frame*>(m_pRootFrame));

    return S_OK;
}

auto SkinnedXMeshObject::InitializeFrameMatrix(Frame* pFrame)->void
{
    if(pFrame->pMeshContainer != nullptr)
    {
        CustomMeshContainer* pMeshContainer = static_cast<CustomMeshContainer*>(pFrame->pMeshContainer);
        for (u32 i = 0; i < pMeshContainer->boneCount; ++i)
        {
            char const* pBoneName{ pMeshContainer->pSkinInfo->GetBoneName(i) };
            Frame* pBone{ static_cast<Frame*>(D3DXFrameFind(m_pRootFrame, pBoneName)) };
            pMeshContainer->frameCombinedMatries[i] = pBone->combinedTransformationMatrix;
        }
        m_meshContainters.push_back(pMeshContainer);
    }
    if (pFrame->pFrameSibling != nullptr)
        InitializeFrameMatrix(static_cast<Frame*>(pFrame->pFrameSibling));
    if (pFrame->pFrameFirstChild != nullptr)
        InitializeFrameMatrix(static_cast<Frame*>(pFrame->pFrameFirstChild));

}

auto SkinnedXMeshObject::UpdateFrameMatrices(Frame* const pFrame, DirectX::XMFLOAT4X4 const& parentTransform)->void
{
    if (pFrame == nullptr)
        return;
    XMMATRIX mCombindTransform{};
    XMMATRIX mParentTransform{};
    mCombindTransform = XMLoadFloat4x4(&reinterpret_cast<XMFLOAT4X4&>(pFrame->TransformationMatrix));
    mParentTransform = XMLoadFloat4x4(&parentTransform);
    mCombindTransform = mCombindTransform * mParentTransform;
    XMStoreFloat4x4(pFrame->combinedTransformationMatrix.get(), mCombindTransform);
    
    if (pFrame->pFrameSibling != nullptr)
    {
        UpdateFrameMatrices(static_cast<Frame*>(pFrame->pFrameSibling), parentTransform);
    }
    if (pFrame->pFrameFirstChild != nullptr)
    {
        UpdateFrameMatrices(static_cast<Frame*>(pFrame->pFrameFirstChild), *pFrame->combinedTransformationMatrix);
    }
}
