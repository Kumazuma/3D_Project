#include "pch.h"
#include "SkinnedXMeshObject.h"
#include "RenderModule.h"
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
    COMPtr<IDirect3DDevice9Ex> pDevice;
    pRenderModule->GetDevice(&pDevice);
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

        // ����Ʈ���� ��Ű���� �����ϴ� �Լ�(��Ű�� �� �ƴ϶� �ִϸ��̼� ���� ��, ������ ���� �������� ������ ���ÿ� �����ϱ⵵ ��)
        iter->pSkinInfo->UpdateSkinnedMesh(
            reinterpret_cast<D3DXMATRIX const*>(iter->renderingMatries.data()),	// ���� ���� ��ȯ���� ���
            nullptr,						// ���� ���·� �ǵ����� ���� ���� ���(������ �� ����� ������� ���ؼ� �־���� ������ �ȳ־��൵ ��� ����)
            pSrcVtx,						// ������ �ʴ� ���� �޽��� ���� ����
            pDestVtx);						// ��ȯ�� ������ ��� ���� �޽��� ���� ����


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
    //TODO:
    return nullptr;
}

auto SkinnedXMeshObject::Initialize(RenderModule* pRenderModule, std::wstring const& filePath) -> HRESULT
{
    HRESULT hr{};
    COMPtr<IDirect3DDevice9Ex> pDevice;
    HierarchyLoader* pHierarchyLoader{};
    COMPtr<ID3DXAnimationController> pAnimationController{};
    AnimationController* pAnimCtrler{};
    D3DXFRAME* pRootFrame{};
    pRenderModule->GetDevice(&pDevice);
    usize idx = filePath.find_last_of(DIRECTORY_SEPARATOR);
    std::wstring directoryPath{ filePath.substr(0, idx + 1) };
    hr = HierarchyLoader::Create(pDevice.Get(), directoryPath, &pHierarchyLoader);
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
        &pRootFrame,
        &pAnimationController);
    if (FAILED(hr))
    {
        return hr;
    }
    m_pRootFrame.reset(pRootFrame);
    hr = AnimationController::Create(pAnimationController.Get(), &pAnimCtrler);
    if (FAILED(hr))
    {
        return hr;
    }
    m_pAnimCtrler.reset(pAnimCtrler);
    InitializeFrameMatrix(static_cast<Frame*>(m_pRootFrame.get()));
    UpdateFrameMatrices(
        static_cast<Frame*>(m_pRootFrame.get()),
        ToFloat4x4(XMMatrixRotationY(XMConvertToRadians(180.f)))
    );

    return E_NOTIMPL;
}

auto SkinnedXMeshObject::InitializeFrameMatrix(Frame* pFrame)->void
{
    if(pFrame->pMeshContainer != nullptr)
    {
        CustomMeshContainer* pMeshContainer = static_cast<CustomMeshContainer*>(pFrame->pMeshContainer);
        for (u32 i = 0; i < pMeshContainer->boneCount; ++i)
        {
            char const* pBoneName{ pMeshContainer->pSkinInfo->GetBoneName(i) };
            Frame* pBone{ static_cast<Frame*>(D3DXFrameFind(m_pRootFrame.get(), pBoneName)) };
            pMeshContainer->frameCombinedMatries[i] = pBone->combinedTransformationMatrix;
        }
        m_meshContainters.push_back(pMeshContainer);
    }
    if (pFrame->pFrameSibling != nullptr)
        InitializeFrameMatrix(static_cast<Frame*>(pFrame->pFrameSibling));
    if (pFrame->pFrameFirstChild != nullptr)
        InitializeFrameMatrix(static_cast<Frame*>(pFrame->pFrameFirstChild));

}

auto SkinnedXMeshObject::UpdateFrameMatrices(Frame* const pFrame, DirectX::XMFLOAT4X4 const& mat)->void
{
    if (pFrame == nullptr)
        return;
    XMMATRIX mCombindTransform{};
    XMMATRIX mParentTransform{};
    mCombindTransform = XMLoadFloat4x4(pFrame->combinedTransformationMatrix.get());
    mParentTransform = XMLoadFloat4x4(&mat);
    mCombindTransform = mCombindTransform * mParentTransform;
    XMStoreFloat4x4(pFrame->combinedTransformationMatrix.get(), mCombindTransform);
    if (pFrame->pFrameSibling != nullptr)
    {
        UpdateFrameMatrices(static_cast<Frame*>(pFrame->pFrameSibling), mat);
    }
    if (pFrame->pFrameFirstChild != nullptr)
    {
        UpdateFrameMatrices(static_cast<Frame*>(pFrame->pFrameSibling), *pFrame->combinedTransformationMatrix);
    }
}
