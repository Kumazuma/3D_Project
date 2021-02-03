#include "pch.h"
#include "SkinnedXMeshObject.h"
#include "RenderModule.h"
#include <shlwapi.h>
#include "Renderer.h"
#include <mutex>
#include <map>
#include <array>
#pragma comment(lib, "shlwapi.lib")
using namespace DirectX;
inline auto __vectorcall ToFloat4x4(XMMATRIX mat)->XMFLOAT4X4
{
    XMFLOAT4X4 tmp{};
    XMStoreFloat4x4(&tmp, mat);
    return tmp;
}
struct BoneInfo
{
    
    u8  boneIdxs[4];
    XMFLOAT4 weights;
};
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

//auto SkinnedXMeshObject::PrepareRender(IRenderer* pRenderer) -> void
//{
//    pRenderer->AddEntity(RenderModule::Kind::SKINNED, m_entity);
//}
auto SkinnedXMeshObject::Render(RenderModule* pRenderModule, IRenderer* pRenderer) -> void
{
    //if (IsVisible() == false) return;
    //COMPtr<IDirect3DDevice9> pDevice;
    //COMPtr<ID3DXEffect> effect;     
    //pRenderModule->GetDevice(&pDevice);
    //pRenderer->GetEffect(&effect);  
    //                                
    //XMMATRIX mNormalWorld{ XMLoadFloat4x4(&m_transform) };
    //mNormalWorld.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);
    //mNormalWorld = XMMatrixTranspose(XMMatrixInverse(nullptr, mNormalWorld));
    //effect->SetMatrix("g_mNormalWorld", reinterpret_cast<D3DXMATRIX*>(&mNormalWorld));
    //effect->SetMatrix("g_mWorld", reinterpret_cast<D3DXMATRIX*>(&this->m_transform));
    //
    //for (auto& iter : m_meshContainters)
    //{                               
    //    auto& rRenderingMatries{ m_renderedMatrices[iter] };
    //    void* pSrcVtx = nullptr;    
    //    void* pDestVtx = nullptr;   

    //    iter->pOriginalMesh->LockVertexBuffer(0, &pSrcVtx);
    //    iter->MeshData.pMesh->LockVertexBuffer(0, &pDestVtx);

    //    // 소프트웨어 스키닝을 수행하는 함수(스키닝 뿐 아니라 애니메이션 변경 시, 뼈대들과 정점 정보들의 변경을 동시에 수행하기도 함)
    //    iter->pSkinInfo->UpdateSkinnedMesh(
    //        reinterpret_cast<D3DXMATRIX const*>(rRenderingMatries.data()),	// 최종 뼈의 변환상태 행렬
    //        nullptr,						// 원래 상태로 되돌리기 위한 상태 행렬(원래는 위 행렬의 역행렬을 구해서 넣어줘야 하지만 안넣어줘도 상관 없음)
    //        pSrcVtx,						// 변하지 않는 원본 메쉬의 정점 정보
    //        pDestVtx);						// 변환된 정보를 담기 위한 메쉬의 정점 정보
    //    iter->pOriginalMesh->UnlockVertexBuffer();
    //    iter->MeshData.pMesh->UnlockVertexBuffer();
    //    COMPtr<IDirect3DVertexBuffer9> vertexBuffer{};
    //    COMPtr<IDirect3DIndexBuffer9> indexBuffer{};
    //    
    //    iter->MeshData.pMesh->GetVertexBuffer(&vertexBuffer);
    //    iter->MeshData.pMesh->GetIndexBuffer(&indexBuffer);
    //    //pDevice->SetVertexDeclaration(m_vertexDecls[iter].Get());
    //    pDevice->SetFVF(iter->fvf);
    //    pDevice->SetStreamSource(0, vertexBuffer.Get(), 0, iter->vertexSize);
    //    //pDevice->SetStreamSource(1, m_boneInfoBuffers[iter].Get(), 0, sizeof(BoneInfo));
    //    pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | 1u);
    //    //pDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INDEXEDDATA | 1u);

    //    pDevice->SetIndices(indexBuffer.Get());
    //    //effect->SetMatrixArray("g_frameMatrices", reinterpret_cast<D3DXMATRIX const*>(renderingMatrix.data()), 255);
    //    for (u32 i = 0; i < iter->NumMaterials; ++i)
    //    {
    //        auto const& info{ iter->subsetTriangleInfo[i] };
    //        auto rMaterial{ iter->materials[i].MatD3D };
    //        auto specularColor{ rMaterial.Specular };
    //        D3DXVECTOR4 specularVec{ specularColor.r,specularColor.g, specularColor.b, rMaterial.Power };
    //        effect->SetTexture("g_diffuseTexture", iter->textures[i].Get());
    //        effect->SetVector("g_vSpecular", &specularVec);
    //        effect->CommitChanges();
    //        pDevice->DrawIndexedPrimitive(
    //            D3DPT_TRIANGLELIST,
    //            0,
    //            0,
    //            iter->vertexCount,
    //            static_cast<UINT>(std::get<0>(info)),
    //            static_cast<UINT>(std::get<1>(info))
    //        );
    //    }
    //}
}

auto SkinnedXMeshObject::Render(IRendererBase* renderer, ID3DXEffect* effect) -> void
{
    if (IsVisible() == false) return;
    COMPtr<IDirect3DDevice9> pDevice;
    renderer->GetDevice(&pDevice);
    for (auto& iter : m_meshContainters)
    {                               
        auto& rRenderingMatries{ m_renderedMatrices[iter] };
        void* pSrcVtx = nullptr;    
        void* pDestVtx = nullptr;   

        iter->pOriginalMesh->LockVertexBuffer(0, &pSrcVtx);
        iter->MeshData.pMesh->LockVertexBuffer(0, &pDestVtx);

        // 소프트웨어 스키닝을 수행하는 함수(스키닝 뿐 아니라 애니메이션 변경 시, 뼈대들과 정점 정보들의 변경을 동시에 수행하기도 함)
        iter->pSkinInfo->UpdateSkinnedMesh(
            reinterpret_cast<D3DXMATRIX const*>(rRenderingMatries.data()),	// 최종 뼈의 변환상태 행렬
            nullptr,						// 원래 상태로 되돌리기 위한 상태 행렬(원래는 위 행렬의 역행렬을 구해서 넣어줘야 하지만 안넣어줘도 상관 없음)
            pSrcVtx,						// 변하지 않는 원본 메쉬의 정점 정보
            pDestVtx);						// 변환된 정보를 담기 위한 메쉬의 정점 정보
        iter->pOriginalMesh->UnlockVertexBuffer();
        iter->MeshData.pMesh->UnlockVertexBuffer();
        COMPtr<IDirect3DVertexBuffer9> vertexBuffer{};
        COMPtr<IDirect3DIndexBuffer9> indexBuffer{};
    
        iter->MeshData.pMesh->GetVertexBuffer(&vertexBuffer);
        iter->MeshData.pMesh->GetIndexBuffer(&indexBuffer);
        //pDevice->SetVertexDeclaration(m_vertexDecls[iter].Get());
        pDevice->SetFVF(iter->fvf);
        pDevice->SetStreamSource(0, vertexBuffer.Get(), 0, iter->vertexSize);
        //pDevice->SetStreamSource(1, m_boneInfoBuffers[iter].Get(), 0, sizeof(BoneInfo));
        pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | 1u);
        //pDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INDEXEDDATA | 1u);

        pDevice->SetIndices(indexBuffer.Get());
        //effect->SetMatrixArray("g_frameMatrices", reinterpret_cast<D3DXMATRIX const*>(renderingMatrix.data()), 255);
        for (u32 i = 0; i < iter->NumMaterials; ++i)
        {
            auto const& info{ iter->subsetTriangleInfo[i] };
            auto rMaterial{ iter->materials[i].MatD3D };
            auto specularColor{ rMaterial.Specular };
            D3DXVECTOR4 specularVec{ specularColor.r,specularColor.g, specularColor.b, rMaterial.Power };
            effect->SetTexture("g_diffuseTexture", iter->textures[i].Get());
            effect->SetVector("g_vSpecular", &specularVec);
            effect->CommitChanges();
            pDevice->DrawIndexedPrimitive(
                D3DPT_TRIANGLELIST,
                0,
                0,
                iter->vertexCount,
                static_cast<UINT>(std::get<0>(info)),
                static_cast<UINT>(std::get<1>(info))
            );
        }
    }
}

auto SkinnedXMeshObject::Clone() const -> RenderObject*
{
    return new SkinnedXMeshObject{ this };
}

auto SkinnedXMeshObject::FindFrameTransfromByName(std::wstring const& frameName, XMFLOAT4X4 * const pOut) ->HRESULT
{
    if (pOut == nullptr)
    {
        return E_POINTER;
    }
    const auto findResIt{ m_combinedOffsetMatrices.find(frameName) };
    if(findResIt == m_combinedOffsetMatrices.end())
    {
        return E_FAIL;
    }

    *pOut = findResIt->second;
    return S_OK;
}

auto SkinnedXMeshObject::FindFrameTransfromByName(std::wstring const& frameName) -> DirectX::XMFLOAT4X4 const*
{
    const auto findResIt{ m_combinedOffsetMatrices.find(frameName) };
    if (findResIt == m_combinedOffsetMatrices.end())
    {
        return nullptr;
    }
    return &findResIt->second;
}

auto SkinnedXMeshObject::IsAnimationSetEnd() -> bool
{
    return m_pAnimCtrler->IsAnimationSetEnd();
}

auto SkinnedXMeshObject::SetAnimationSet(u32 idx, bool repeat) -> void
{
    m_repeat = repeat;
    m_pAnimCtrler->PlayAnimationSet(idx);
    std::lock_guard<SpinLock> guard{ *m_spinLock };

    UpdateFrameMatrices(
        static_cast<Frame*>(m_pRootFrame),
        ToFloat4x4(XMMatrixIdentity())
    );
    for (auto& iter : m_meshContainters)
    {
        auto& rRenderingMatries{ m_renderedMatrices[iter] };
        for (u32 i = 0; i < iter->boneCount; ++i)
        {
            XMMATRIX mRenderingMatrix{};
            XMMATRIX mFrameOffset{};
            XMMATRIX mFrameCombinedMatrix{};
            mFrameOffset = XMLoadFloat4x4(&iter->frameOffsetMatries[i]);
            mFrameCombinedMatrix = XMLoadFloat4x4(iter->frameCombinedMatries[i].get());
            mRenderingMatrix = mFrameOffset * mFrameCombinedMatrix;
            XMStoreFloat4x4(&rRenderingMatries[i], mRenderingMatrix);
        }
    }
}

auto SkinnedXMeshObject::PlayAnimation(f32 timeDelta) -> void
{
    std::lock_guard<SpinLock> guard{ *m_spinLock };
    f32 const length{ m_pAnimCtrler->GetAnimationSetLength() };
    f32 const seek{ m_pAnimCtrler->GetSeek() };
    if (seek + timeDelta >=  length && m_repeat == false)
    {
        //timeDelta = length - seek;
    }
    m_pAnimCtrler->AdvanceTime(timeDelta);
    m_pAnimCtrler->AdjustAnimationToFrame();
    UpdateFrameMatrices(
        static_cast<Frame*>(m_pRootFrame),
        ToFloat4x4(XMMatrixIdentity())
    );
    for (auto& iter : m_meshContainters)
    {
        auto& rRenderingMatries{ m_renderedMatrices[iter] };
        for (u32 i = 0; i < iter->boneCount; ++i)
        {
            XMMATRIX mRenderingMatrix{};
            XMMATRIX mFrameOffset{};
            XMMATRIX mFrameCombinedMatrix{};
            mFrameOffset = XMLoadFloat4x4(&iter->frameOffsetMatries[i]);
            mFrameCombinedMatrix = XMLoadFloat4x4(iter->frameCombinedMatries[i].get());
            mRenderingMatrix = mFrameOffset * mFrameCombinedMatrix;
            XMStoreFloat4x4(&rRenderingMatries[i], mRenderingMatrix);
        }
    }
}
auto SkinnedXMeshObject::GetCurrentAnimSetLength()const->f32
{
    return this->m_pAnimCtrler->GetAnimationSetLength();

}
auto SkinnedXMeshObject::GetCurrentSeek()const->f32
{
    return this->m_pAnimCtrler->GetSeek();

}
auto SkinnedXMeshObject::GetAnimationCount()const -> u32
{
    return m_pAnimCtrler->GetAnimCount();
}

auto SkinnedXMeshObject::GetFrameNamesRef() const -> std::set<std::wstring> const&
{
    // TODO: 여기에 return 문을 삽입합니다.
    return *m_pFrameNames;
}

SkinnedXMeshObject::SkinnedXMeshObject():
    m_pRootFrame{},
    m_entity{ new SkinnedMeshEntity{this} }
{

}

SkinnedXMeshObject::SkinnedXMeshObject(SkinnedXMeshObject const* rhs):
    m_pAnimCtrler{ rhs->m_pAnimCtrler->Clone() },
    m_pHierarchyLoader{rhs->m_pHierarchyLoader},
    m_meshContainters{rhs->m_meshContainters},
    m_pRootFrame{rhs->m_pRootFrame},
    m_entity{new SkinnedMeshEntity{this}},
    m_spinLock{rhs->m_spinLock},
    m_combinedOffsetMatrices{rhs->m_combinedOffsetMatrices},
    m_renderedMatrices{rhs->m_renderedMatrices},
    m_pFrameNames{rhs->m_pFrameNames}
    //m_boneInfoBuffers{rhs->m_boneInfoBuffers},
    //m_vertexDecls{ rhs->m_vertexDecls }
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
    std::string errmsg{};
    switch (hr)
    {
    case D3DERR_INVALIDCALL:
        errmsg = "D3DERR_INVALIDCALL";
        break;
    case E_OUTOFMEMORY:
        errmsg = "E_OUTOFMEMORY";
        break;
    }
    if (FAILED(hr))
    {
        OutputDebugStringA(errmsg.c_str());
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
        ToFloat4x4(XMMatrixIdentity())
    );
    m_pFrameNames.reset(new std::set<std::wstring>{});
    InitializeFrameMatrix(static_cast<Frame*>(m_pRootFrame));
    XMFLOAT4X4 identity{};
    identity(0, 0) = identity(1, 1)= identity(2, 2)= identity(3, 3)= 1.f;
    for (auto& iter : m_meshContainters)
    {
        m_renderedMatrices[iter].reserve(256);
        m_renderedMatrices[iter].assign(iter->boneCount, XMFLOAT4X4{});
    }
    UpdateFrameMatrices(
        static_cast<Frame*>(m_pRootFrame),
        ToFloat4x4(XMMatrixIdentity())
    );
    for (auto& iter : m_meshContainters)
    {
        auto& rRenderingMatries{ m_renderedMatrices[iter] };
        for (u32 i = 0; i < iter->boneCount; ++i)
        {
            XMMATRIX mRenderingMatrix{};
            XMMATRIX mFrameOffset{};
            XMMATRIX mFrameCombinedMatrix{};
            mFrameOffset = XMLoadFloat4x4(&iter->frameOffsetMatries[i]);
            mFrameCombinedMatrix = XMLoadFloat4x4(iter->frameCombinedMatries[i].get());
            mRenderingMatrix = mFrameOffset * mFrameCombinedMatrix;
            XMStoreFloat4x4(&rRenderingMatries[i], mRenderingMatrix);
        }
        //auto skinInfo{ iter->pSkinInfo };
        //u32 const boneCount{ static_cast<u32>(iter->pSkinInfo->GetNumBones()) };
        //std::unordered_map<DWORD, std::unordered_map<DWORD,f32> > vertexBoneWeightTable;
        //for (u32 i = 0; i < boneCount; ++i)
        //{
        //    u32 const vertexCount{ static_cast<u32>(skinInfo->GetNumBoneInfluences(static_cast<DWORD>(i))) };
        //    std::vector<DWORD> vertices;
        //    std::vector<f32> weights;
        //    vertices.assign(vertexCount, 0);
        //    weights.assign(vertexCount, 0.f);
        //    
        //    skinInfo->GetBoneInfluence(static_cast<DWORD>(i), vertices.data(), weights.data());
        //    for (u32 j = 0; j < vertexCount; ++j)
        //    {
        //        DWORD const vertexID{ vertices[j] };
        //        f32 const weight{ weights[j] };
        //        vertexBoneWeightTable[vertexID][i] = weight;
        //    }
        //}
        //for (auto& it : vertexBoneWeightTable)
        //{
        //    auto& table{ it.second };
        //    //영향을 받는 뼈의 갯수가 4개 이하면 상관없다.
        //    if (table.size() <= 4)
        //    {
        //        continue;
        //    }
        //    char s[100];
        //    _itoa_s(table.size(), s, 10);
        //    OutputDebugStringA(s);
        //    std::map<f32, DWORD, std::greater<f32> > weights;
        //    for (auto const& it : table) weights.emplace(it.second, it.first);
        //    f32 sum{};
        //    auto it2{ weights.begin() };
        //    //상위 4개만 한다.
        //    table.clear();

        //    for (u32 i = 0; i < 4; ++i)
        //    {
        //        table.emplace(it2->second, it2->first);
        //        sum += it2->first;
        //        ++it2;
        //    }
        //    for (auto& it : table)
        //    {
        //        it.second = it.second / sum;
        //    }
        //}
        //u32 const vertexCount{ iter->MeshData.pMesh->GetNumVertices() };
        //std::vector<BoneInfo> boneInfos;
        //boneInfos.assign(vertexCount, {});
        //for (auto& it : vertexBoneWeightTable)
        //{
        //    BoneInfo boneInfo{};
        //    std::array<f32, 4>& weights{
        //        reinterpret_cast<std::array<f32, 4>&>(boneInfo.weights) };
        //    std::array<u8, 4>& boneIdxs{
        //        reinterpret_cast<std::array<u8, 4>&>(boneInfo.boneIdxs) };
        //    auto const& table{ it.second };
        //    auto tableIt{ table.begin() };
        //    auto weightIt{ weights.begin() };
        //    auto boneIdxsIt{ boneIdxs.begin() };
        //    for (; tableIt != table.end(); ++tableIt)
        //    {
        //        *weightIt = tableIt->second;
        //        *boneIdxsIt = static_cast<u8>(tableIt->first);
        //        ++boneIdxsIt;
        //        ++weightIt;
        //    }
        //    boneInfos[it.first] = boneInfo;
        //}
        //D3DVERTEXELEMENT9 xMeshDecl[65];
        ////iter->pSkinInfo->ConvertToBlendedMesh()
        //iter->MeshData.pMesh->GetDeclaration(xMeshDecl);
        //D3DVERTEXELEMENT9 decls[65]{};
        //u32 slot2StartIdx{ };
        //for (auto const& decl : xMeshDecl)
        //{
        //    if (decl.Stream == 0xFF)
        //    {
        //        break;
        //    }
        //    decls[slot2StartIdx] = decl;
        //    ++slot2StartIdx;
        //}
        //decls[slot2StartIdx + 0].Stream = 1;
        //decls[slot2StartIdx + 0].Offset = 0;
        //decls[slot2StartIdx + 0].Type = D3DDECLTYPE_UBYTE4;
        //decls[slot2StartIdx + 0].Method = D3DDECLMETHOD_DEFAULT;
        //decls[slot2StartIdx + 0].Usage = D3DDECLUSAGE_BLENDINDICES;
        //decls[slot2StartIdx + 0].UsageIndex = 0;
        //
        //decls[slot2StartIdx + 1].Stream = 1;
        //decls[slot2StartIdx + 1].Offset = sizeof(BoneInfo::boneIdxs);
        //decls[slot2StartIdx + 1].Type = D3DDECLTYPE_FLOAT4;
        //decls[slot2StartIdx + 1].Method = D3DDECLMETHOD_DEFAULT;
        //decls[slot2StartIdx + 1].Usage = D3DDECLUSAGE_BLENDWEIGHT;
        //decls[slot2StartIdx + 1].UsageIndex = 0;

        //decls[slot2StartIdx + 2] = D3DDECL_END();
        //COMPtr<IDirect3DVertexDeclaration9> vertexDecls;
        //COMPtr<IDirect3DVertexBuffer9> boneInfoBuffer;
        //
        //hr = pDevice->CreateVertexBuffer(
        //    boneInfos.size() * sizeof(BoneInfo),
        //    0,
        //    0,
        //    D3DPOOL_MANAGED,
        //    &boneInfoBuffer,
        //    nullptr
        //);
        //if (FAILED(hr))
        //{
        //    return E_FAIL;
        //}
        //hr = pDevice->CreateVertexDeclaration(decls, &vertexDecls);
        //if (FAILED(hr))
        //{
        //    return E_FAIL;
        //}
        //BoneInfo* boneInfoBufferPtr{};
        //boneInfoBuffer->Lock(0, 0, reinterpret_cast<void**>(&boneInfoBufferPtr), 0);
        //memcpy(boneInfoBufferPtr, boneInfos.data(), sizeof(BoneInfo)* boneInfos.size());
        //boneInfoBuffer->Unlock();
        //m_vertexDecls.emplace(iter, vertexDecls);
        //m_boneInfoBuffers.emplace(iter, boneInfoBuffer);
    }

    m_spinLock = std::shared_ptr<SpinLock>(new SpinLock{});
    return S_OK;
}

auto SkinnedXMeshObject::InitializeFrameMatrix(Frame* pFrame)->void
{
    if (!pFrame->name.empty())
    {
        m_pFrameNames->emplace(pFrame->name);
    }
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
    XMStoreFloat4x4(&m_combinedOffsetMatrices[pFrame->name], mCombindTransform);
    if (pFrame->pFrameSibling != nullptr)
    {
        UpdateFrameMatrices(static_cast<Frame*>(pFrame->pFrameSibling), parentTransform);
    }
    if (pFrame->pFrameFirstChild != nullptr)
    {
        UpdateFrameMatrices(static_cast<Frame*>(pFrame->pFrameFirstChild), *pFrame->combinedTransformationMatrix);
    }
}

SkinnedMeshEntity::SkinnedMeshEntity(SkinnedXMeshObject* pObj) :
    m_pObj{pObj}
{
}

auto SkinnedMeshEntity::Render(RenderModule* pRenderModule, IRenderer* pRenderer) -> void
{
    m_pObj->Render(pRenderModule, pRenderer);
}
