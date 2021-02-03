#include "pch.h"
#include "GameRendererImpl.hpp"
#include <RenderModule.h>
#include <mutex>
#include <WinAPIResourceLoader.hpp>
#include "resource.h"
#include<iostream>
#include"include/RenderComponent.hpp"
#include"include/Effect.hpp"
#include<algorithm>
#include<TextureRenderTarget.h>
using namespace DirectX;
struct PPVertexFVF { XMFLOAT3 xyz; XMFLOAT2 uv; };
static constexpr DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;

wchar_t constexpr LIGHT_SPECULAR[] = L"light_specular";
wchar_t constexpr LIGHT_DIFFUSE[] = L"light_diffuse";
char constexpr ID_TEX_NORMAL_MAP[] = "g_normalMap";
char constexpr ID_TEX_SPECULAR_MAP[]= "g_specularMap" ;
char constexpr ID_CONST_INVSERSE_VIEW_PORJ_MATRIX[]{ "g_mInverseViewProj" };
char constexpr ID_TEX_DEPTH_MAP[]{ "g_depthMap" };
DWORD constexpr shaderFlag{ D3DXSHADER_OPTIMIZATION_LEVEL3 };

_BEGIN_NS(Kumazuma::Graphics)
GameRendererImpl::GameRendererImpl(RenderModule* renderModule)
{
    renderModule_ = renderModule;
    HRESULT hr{};
    ComPtr<IDirect3DDevice9> device;
    renderModule->GetDevice(&device);
    auto const width{ renderModule->GetWidth() };
    auto const height{ renderModule->GetHeight() };
    renderModule->CreateRenderTarget(RENDER_TARGET_ALBEDO, width, height, D3DFMT_A8R8G8B8);
    renderModule->CreateRenderTarget(RENDER_TARGET_NORMAL, width, height, D3DFMT_A16B16G16R16F);
    renderModule->CreateRenderTarget(RENDER_TARGET_SPECULAR, width, height, D3DFMT_A16B16G16R16F);
    renderModule->CreateRenderTarget(RENDER_TARGET_DEPTH, width, height, D3DFMT_G32R32F);
    renderModule->CreateRenderTarget(LIGHT_SPECULAR, width, height, D3DFMT_A16B16G16R16F);
    renderModule->CreateRenderTarget(LIGHT_DIFFUSE, width, height, D3DFMT_A16B16G16R16F);
    D3DXCreateSprite(device.Get(), &sprite_);
    WinAPIResourceLoader resourceLoader{ g_hDLLModule };
    resourceLoader.Open(MAKEINTRESOURCEW(IDR_FX_INGAME), L"fx");
    ComPtr<ID3DXEffect> deferredEffect;
    ComPtr<ID3DXEffect> effectEffect;
    ComPtr<ID3DXEffect> uiEffect;

    hr = CreateEffectFromRC(device.Get(), MAKEINTRESOURCEW(IDR_FX_INGAME), L"fx", &deferredEffect);
    if (FAILED(hr))
    {
        throw hr;
    }
    effectTable_.emplace(L"DEFERRED_EFFECT", deferredEffect);

    hr = CreateEffectFromRC(device.Get(), MAKEINTRESOURCEW(IDR_FX_EFFECT_1), L"fx", &effectEffect);
    if (FAILED(hr))
    {
        throw hr;
    }
    effectTable_.emplace(L"EFFECT1", effectEffect);

    hr = CreateEffectFromRC(device.Get(), MAKEINTRESOURCEW(IDR_FX_UI), L"fx", &uiEffect);
    if (FAILED(hr))
    {
        throw hr;
    }
    effectTable_.emplace(L"UI", uiEffect);

    hr = CreateEffectFromRC(device.Get(), MAKEINTRESOURCEW(IDR_FX_COMBINE), L"fx", &combineShader_);
    if (FAILED(hr))
    {
        throw hr;
    }

    hr = CreateEffectFromRC(device.Get(), MAKEINTRESOURCEW(IDR_FX_LIGHTING), L"fx", &lightingShader_);
    if (FAILED(hr))
    {
        throw hr;
    }
    if (FAILED(hr))
    {
        throw hr;
    }

    device->CreateVertexBuffer(sizeof(PPVertexFVF) * 4, 0, FVF, D3DPOOL_MANAGED, &vertexBuffer_ , nullptr);
    device->CreateIndexBuffer(sizeof(int) * 6, 0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &indexBuffer_, nullptr);
    PPVertexFVF* pVertices;
    vertexBuffer_->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);
    pVertices[0].uv = XMFLOAT2{ 0.f, 0.f };
    pVertices[0].xyz = XMFLOAT3{ -1.f, 1.f, 0.f };

    pVertices[1].uv = XMFLOAT2{ 1.f, 0.f };
    pVertices[1].xyz = XMFLOAT3{ 1.f, 1.f, 0.f };

    pVertices[2].uv = XMFLOAT2{ 1.f, 1.f };
    pVertices[2].xyz = XMFLOAT3{ 1.f, -1.f, 0.f };

    pVertices[3].uv = XMFLOAT2{ 0.f, 1.f };
    pVertices[3].xyz = XMFLOAT3{ -1.f, -1.f, 0.f };
    vertexBuffer_->Unlock();
    int* pIndices;
    indexBuffer_->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);

    pIndices[0] = 0;
    pIndices[1] = 1;
    pIndices[2] = 3;

    pIndices[3] = 1;
    pIndices[4] = 2;
    pIndices[5] = 3;

    indexBuffer_->Unlock();
    minZ_ = 0.1f;
    maxZ_ = 400.f;

    SkyBoxObject* skyBox;
    SkyBoxObject::Create(renderModule_, &skyBox);
    skybox_.reset(skyBox);
}
GameRendererImpl::GameRendererImpl(GameRendererImpl&& rhs) noexcept:
    renderModule_{std::move(rhs.renderModule_)},
    viewMatrix_{ std::move(rhs.viewMatrix_)},
    projMatrix_{std::move(rhs.projMatrix_)},
    combineShader_{std::move(rhs.combineShader_)},
    lightingShader_{std::move(rhs.lightingShader_)},
    renderGroups_{std::move(rhs.renderGroups_)},
    effectTable_{std::move(rhs.effectTable_)},
    vertexBuffer_{std::move(rhs.vertexBuffer_)},
    indexBuffer_{std::move(rhs.indexBuffer_)},
    minZ_{std::move(rhs.minZ_)},
    maxZ_{std::move(rhs.maxZ_)},
    skybox_{std::move(rhs.skybox_)},
    sprite_{std::move(rhs.sprite_)}
{

}

auto GameRendererImpl::GetEffect(std::wstring const& effectName, ID3DXEffect** out) -> HRESULT
{
    if (out == nullptr)
    {
        return E_POINTER;
    }
    if (auto it = effectTable_.find(effectName); it != effectTable_.end())
    {
        *out = it->second.Get();
        it->second->AddRef();
    }
    return E_FAIL;
}

auto GameRendererImpl::SetSkyBox(IDirect3DBaseTexture9* texture) -> void
{
    skyboxTexture_ = texture;
}

auto Kumazuma::Graphics::GameRendererImpl::AddRender(RenderGroupID groupID, std::shared_ptr<RenderComponent> const& renderCom) -> void
{
    std::lock_guard<decltype(locker_)> guard{ locker_ };
    this->renderGroups_[groupID].push_back(renderCom);
}

auto Kumazuma::Graphics::GameRendererImpl::AddUI(std::shared_ptr<RenderComponent> const& renderCom, u32 zOrder) -> void
{
    std::lock_guard<decltype(locker_)> guard{ locker_ };
    UIs_[zOrder].emplace_back(renderCom);
}

auto Kumazuma::Graphics::GameRendererImpl::AddEffect(std::shared_ptr<Effect> const& effect) -> void
{
    std::lock_guard<decltype(locker_)> guard{ locker_ };
    effects_.push_back(effect);
}

auto Kumazuma::Graphics::GameRendererImpl::AddPostprocessing(PostProcessing* postprocessing) -> void
{

}


auto GameRendererImpl::SetProjMatrix(DirectX::XMFLOAT4X4 const& mat) -> void
{
    projMatrix_ = mat;
    frustum_.MakeFrustum(XMLoadFloat4x4(&viewMatrix_), XMLoadFloat4x4(&projMatrix_));

}

auto GameRendererImpl::SetViewMatrix(DirectX::XMFLOAT4X4 const& mat) -> void
{
    viewMatrix_ = mat;
    frustum_.MakeFrustum(XMLoadFloat4x4(&viewMatrix_), XMLoadFloat4x4(&projMatrix_));

}

auto GameRendererImpl::Render() -> void
{
    /*Deferred,
    Alpha,
    Effect,
    UI*/
    ComPtr<IDirect3DDevice9> pDevice{};
    renderModule_->GetDevice(&pDevice);
    XMMATRIX mViewProj{};
    XMFLOAT4X4 viewPorjMatrix{};
    XMMATRIX mView{ XMLoadFloat4x4(&viewMatrix_) };
    XMMATRIX mProj{ XMLoadFloat4x4(&projMatrix_) };
    mViewProj = mView * mProj;
    XMStoreFloat4x4(&viewPorjMatrix, mViewProj);
    pDevice->SetTransform(D3DTS_PROJECTION, reinterpret_cast<D3DMATRIX*>(&projMatrix_));
    pDevice->SetTransform(D3DTS_VIEW, reinterpret_cast<D3DMATRIX*>(&viewMatrix_));
    for (auto& it : effectTable_)
    {
        it.second->SetMatrix("g_mView", reinterpret_cast<D3DXMATRIX*>(&viewMatrix_));
        it.second->SetMatrix("g_mProj", reinterpret_cast<D3DXMATRIX*>(&projMatrix_));
        it.second->SetMatrix("g_mViewProj", reinterpret_cast<D3DXMATRIX*>(&viewPorjMatrix));
    }
    pDevice->BeginScene();
    ComPtr<IDirect3DSwapChain9> pSwapChain{};
    ComPtr<IDirect3DSurface9> pBackbuffer{};
    renderModule_->GetDefaultSwapChain(&pSwapChain);
    pSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackbuffer);

    pDevice->SetRenderTarget(0, pBackbuffer.Get());
    pDevice->Clear(0, nullptr, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_COLORVALUE(0.f, 0.f, 0.f, 0.f), 1.f, 0);
    if (skyboxTexture_ != nullptr)
    {
        skybox_->SetDiffuseTexture(reinterpret_cast<IDirect3DCubeTexture9*>(skyboxTexture_.Get()));
        skybox_->Render(this, nullptr);
    }
    
    ClearRenderTargets(renderModule_);
    DeferredRender(renderModule_);
    RenderEffect(renderModule_);
    RenderWireframe(renderModule_);
    RenderPostCombine();
    RenderUI();
    D3DXMATRIX tmpMat;
    D3DXMATRIX tmpMat2;
    D3DXMatrixScaling(&tmpMat, .2f, .2f, .2f);
    D3DXMatrixTranslation(&tmpMat2, renderModule_->GetWidth() * 0.2f, 0, 0);

    ComPtr<IDirect3DTexture9> pLightSpecularMapTexture;
    ComPtr<IDirect3DTexture9> pLightDiffuseMapTexture;
    ComPtr<IDirect3DTexture9> pAlbedoMapTexture;
    ComPtr<IDirect3DTexture9> pDepthMapTexture;
    ComPtr<IRenderTarget> textureRenderTarget;
    renderModule_->GetDefaultSwapChain(&pSwapChain);
    pSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackbuffer);

    renderModule_->GetRenderTarget(RENDER_TARGET_ALBEDO, textureRenderTarget.ReleaseAndGetAddressOf());
    static_cast<TextureRenderTarget*>(textureRenderTarget.Get())->GetTexture(&pAlbedoMapTexture);

    renderModule_->GetRenderTarget(LIGHT_SPECULAR, textureRenderTarget.ReleaseAndGetAddressOf());
    static_cast<TextureRenderTarget*>(textureRenderTarget.Get())->GetTexture(&pLightSpecularMapTexture);

    renderModule_->GetRenderTarget(LIGHT_DIFFUSE, textureRenderTarget.ReleaseAndGetAddressOf());
    static_cast<TextureRenderTarget*>(textureRenderTarget.Get())->GetTexture(&pLightDiffuseMapTexture);

    renderModule_->GetRenderTarget(RENDER_TARGET_DEPTH, textureRenderTarget.ReleaseAndGetAddressOf());
    static_cast<TextureRenderTarget*>(textureRenderTarget.Get())->GetTexture(&pDepthMapTexture);

    sprite_->Begin(D3DXSPRITE_ALPHABLEND);
    sprite_->SetTransform(&tmpMat);
    //sprite_->Draw(pAlbedoMapTexture.Get(), nullptr, nullptr, nullptr, D3DCOLOR_COLORVALUE(1.f, 1.f, 1.f, 1.f));
    sprite_->End();
    pDevice->EndScene();
    //pDevice->Present(nullptr, nullptr, NULL, nullptr);
    this->renderGroups_.clear();
}
auto Kumazuma::Graphics::GameRendererImpl::GetRenderModuleRef() -> RenderModule&
{
    // TODO: 여기에 return 문을 삽입합니다.
    return *renderModule_;
}
auto Kumazuma::Graphics::GameRendererImpl::GetDevice(IDirect3DDevice9** out) -> HRESULT
{
    renderModule_->GetDevice(out);
    return S_OK;
}
auto Kumazuma::Graphics::GameRendererImpl::GetProjMatrix(DirectX::XMFLOAT4X4* pOut) -> HRESULT
{
    *pOut = this->projMatrix_;
    return S_OK;
}
auto GameRendererImpl::ClearRenderTargets(RenderModule* renderModule) -> void
{
    ComPtr<IDirect3DDevice9> pDevice{};

    ComPtr<IRenderTarget> albedo;
    ComPtr<IRenderTarget> normal;
    ComPtr<IRenderTarget> specular;
    ComPtr<IRenderTarget> depth;
    ComPtr<IRenderTarget> lightSpecular;
    ComPtr<IRenderTarget> lightDiffuse;

    ComPtr<IDirect3DSurface9> albedoSurface;
    ComPtr<IDirect3DSurface9> normalSurface;
    ComPtr<IDirect3DSurface9> specularSurface;
    ComPtr<IDirect3DSurface9> depthSurface;
    ComPtr<IDirect3DSurface9> lightSpecularSurface;
    ComPtr<IDirect3DSurface9> lightDiffuseSurface;
    renderModule->GetDevice(&pDevice);

    renderModule->GetRenderTarget(RENDER_TARGET_ALBEDO, &albedo);
    renderModule->GetRenderTarget(RENDER_TARGET_NORMAL, &normal);
    renderModule->GetRenderTarget(RENDER_TARGET_SPECULAR, &specular);
    renderModule->GetRenderTarget(RENDER_TARGET_DEPTH, &depth);
    renderModule->GetRenderTarget(LIGHT_SPECULAR, &lightSpecular);
    renderModule->GetRenderTarget(LIGHT_DIFFUSE, &lightDiffuse);

    albedo->GetSurface(&albedoSurface);
    normal->GetSurface(&normalSurface);
    specular->GetSurface(&specularSurface);
    depth->GetSurface(&depthSurface);

    pDevice->SetRenderTarget(0, albedoSurface.Get());
    pDevice->SetRenderTarget(1, normalSurface.Get());
    pDevice->SetRenderTarget(2, specularSurface.Get());
    pDevice->SetRenderTarget(3, depthSurface.Get());
    pDevice->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_COLORVALUE(0.f, 0.f, 0.f, 0.f), 1.f, 0);

}
auto GameRendererImpl::DeferredRender(RenderModule* renderModule) -> void
{
    auto& list{ renderGroups_[RenderGroupID::Deferred] };
    for (auto& com : list)
    {
        com->PrepareRender(*renderModule, *this);
        com->Render(*renderModule, *this);
    }
    DeferredLighting(renderModule);
    DeferredCombine(renderModule);
}
auto Kumazuma::Graphics::GameRendererImpl::RenderWireframe(RenderModule* pRenderModule) -> void
{
    ComPtr<IDirect3DSwapChain9> pSwapChain{};
    ComPtr<IDirect3DSurface9> pBackbuffer{};
    ComPtr<IDirect3DDevice9> pDevice{};
    renderModule_->GetDevice(&pDevice);
    renderModule_->GetDefaultSwapChain(&pSwapChain);
    pSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackbuffer);
    pDevice->SetRenderTarget(0, pBackbuffer.Get());
    auto& list{ renderGroups_[RenderGroupID::Wireframe] };
    for (auto& com : list)
    {
        com->PrepareRender(*renderModule_, *this);
        com->Render(*renderModule_, *this);
    }
}
auto Kumazuma::Graphics::GameRendererImpl::RenderPostCombine() -> void
{
    auto& list{ renderGroups_[RenderGroupID::PostCombine] };
    for (auto& com : list)
    {
        com->PrepareRender(*renderModule_, *this);
        com->Render(*renderModule_, *this);
    }
}
auto Kumazuma::Graphics::GameRendererImpl::RenderUI() -> void
{
    auto const  width{ renderModule_->GetWidth() };
    auto const  height{ renderModule_->GetHeight() };
    auto        mViewSpace{ XMMatrixIdentity() };
    auto        mProjSpace{ XMMatrixOrthographicLH(width, height, 0.f, 1000.f) };
    ComPtr<IDirect3DDevice9> device{};
    renderModule_->GetDevice(&device);

    for (auto& it : effectTable_)
    {
        it.second->SetMatrix("g_mView", reinterpret_cast<D3DXMATRIX*>(&mViewSpace));
        it.second->SetMatrix("g_mProj", reinterpret_cast<D3DXMATRIX*>(&mProjSpace));
        it.second->SetMatrix("g_mViewProj", reinterpret_cast<D3DXMATRIX*>(&mProjSpace));
    }
    device->SetTransform(D3DTS_PROJECTION, reinterpret_cast<D3DMATRIX*>(&mProjSpace));
    device->SetTransform(D3DTS_VIEW, reinterpret_cast<D3DMATRIX*>(&mViewSpace));

    auto        it{ UIs_.rbegin() };
    auto const  end{ UIs_.rend() };
    for (; it != end; ++it)
    {
        for (auto const& renderCom : it->second)
        {
            renderCom->PrepareRender(*renderModule_, *this);
            renderCom->Render(*renderModule_, *this);
        }
    }
    UIs_.clear();

}
auto GameRendererImpl::CreateEffectFromRC(
    IDirect3DDevice9* device,
    wchar_t const* resourceName,
    wchar_t const* resourceType,
    ID3DXEffect** effect) -> HRESULT
{
    HRESULT hr{};
    ComPtr<ID3DXBuffer> errMsgBuffer;

    WinAPIResourceLoader resourceLoader{ g_hDLLModule };
    resourceLoader.Open(resourceName, resourceType);
    hr = D3DXCreateEffect(device,
        reinterpret_cast<char*>(resourceLoader.Ptr()),
        resourceLoader.Size(),
        nullptr,
        nullptr,
        shaderFlag,
        nullptr,
        effect,
        &errMsgBuffer);
    if (errMsgBuffer != nullptr)
    {
        char* msg{ reinterpret_cast<char*>(errMsgBuffer->GetBufferPointer()) };
        MessageBoxA(nullptr, msg, nullptr, MB_OK);
        std::clog << __FILE__ << ":" << __LINE__ << " shader error!\n" << errMsgBuffer->GetBufferPointer() << '\n';
    }
    return hr;
}
auto GameRendererImpl::DeferredCombine(RenderModule* pRenderModule) -> void
{
    ComPtr<IDirect3DSwapChain9> pSwapChain{};
    ComPtr<IDirect3DSurface9> pBackbuffer{};
    ComPtr<IDirect3DDevice9> pDevice;
    ComPtr<IDirect3DTexture9> pLightSpecularMapTexture;
    ComPtr<IDirect3DTexture9> pLightDiffuseMapTexture;
    ComPtr<IDirect3DTexture9> pAlbedoMapTexture;
    ComPtr<IDirect3DTexture9> pDepthMapTexture;
    ComPtr<IRenderTarget> textureRenderTarget;
    pRenderModule->GetDefaultSwapChain(&pSwapChain);
    pRenderModule->GetDevice(&pDevice);
    pSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackbuffer);
    
    pRenderModule->GetRenderTarget(RENDER_TARGET_ALBEDO, textureRenderTarget.ReleaseAndGetAddressOf());
    static_cast<TextureRenderTarget*>(textureRenderTarget.Get())->GetTexture(&pAlbedoMapTexture);

    pRenderModule->GetRenderTarget(LIGHT_SPECULAR, textureRenderTarget.ReleaseAndGetAddressOf());
    static_cast<TextureRenderTarget*>(textureRenderTarget.Get())->GetTexture(&pLightSpecularMapTexture);

    pRenderModule->GetRenderTarget(LIGHT_DIFFUSE, textureRenderTarget.ReleaseAndGetAddressOf());
    static_cast<TextureRenderTarget*>(textureRenderTarget.Get())->GetTexture(&pLightDiffuseMapTexture);

    pRenderModule->GetRenderTarget(RENDER_TARGET_DEPTH, textureRenderTarget.ReleaseAndGetAddressOf());
    static_cast<TextureRenderTarget*>(textureRenderTarget.Get())->GetTexture(&pDepthMapTexture);

    pDevice->SetRenderTarget(0, pBackbuffer.Get());
    pDevice->SetRenderTarget(1, nullptr);
    pDevice->SetRenderTarget(2, nullptr);
    pDevice->SetRenderTarget(3, nullptr);
    UINT passCount{};

    pDevice->SetFVF(FVF);
    pDevice->SetIndices(indexBuffer_.Get());
    pDevice->SetStreamSource(0, vertexBuffer_.Get(), 0, sizeof(PPVertexFVF));

    combineShader_->Begin(&passCount, 0);
    combineShader_->SetFloat("g_farZ", maxZ_ - minZ_);
    combineShader_->SetTexture("g_shadeMap", pLightDiffuseMapTexture.Get());
    combineShader_->SetTexture("g_specularMap", pLightSpecularMapTexture.Get());
    combineShader_->SetTexture("g_albedoMap", pAlbedoMapTexture.Get());
    combineShader_->SetTexture("g_depthMap", pDepthMapTexture.Get());
    combineShader_->BeginPass(0);
    pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
    combineShader_->EndPass();
    combineShader_->End();
}
auto Kumazuma::Graphics::GameRendererImpl::RenderEffect(RenderModule* pRenderModule) -> void
{
    ComPtr<IDirect3DSwapChain9> pSwapChain{};
    ComPtr<IDirect3DSurface9> pBackbuffer{};
    ComPtr<IDirect3DDevice9> pDevice{};
    ComPtr<IRenderTarget> textureRenderTarget;
    ComPtr<IDirect3DTexture9> pDepthMapTexture;
    XMMATRIX mViewSpace{};
    XMVECTOR vZAxis{};
    //내게 필요한 것은 뷰 스페이스 상의 Z축값, 벡터 내적을 이용하면 된다.
    mViewSpace = XMLoadFloat4x4(&viewMatrix_);
    vZAxis = XMMatrixTranspose(mViewSpace).r[2];

    renderModule_->GetDevice(&pDevice);
    renderModule_->GetDefaultSwapChain(&pSwapChain);

    pRenderModule->GetRenderTarget(RENDER_TARGET_DEPTH, textureRenderTarget.ReleaseAndGetAddressOf());
    static_cast<TextureRenderTarget*>(textureRenderTarget.Get())->GetTexture(&pDepthMapTexture);

    pSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackbuffer);
    pDevice->SetRenderTarget(0, pBackbuffer.Get());
    
    std::vector<std::pair<f32,raw_ptr<Effect> > > sortedEffect;
    sortedEffect.reserve(effects_.size());
    for (auto const& effect : effects_)
    {
        f32x3 pos{};
        effect->GetCenter(&pos);
        XMVECTOR vPosition{XMVectorSetW( XMLoadFloat3(&pos), 1.f) };
        f32 z{};
        XMStoreFloat(&z, XMVector4Dot(vPosition, vZAxis));
        sortedEffect.push_back(std::pair<f32, raw_ptr<Effect>>{z, effect.get()});
    }
    //Z가 큰 것을 앞에 둬서 먼저 그리게 만든다.
    auto sortFunction =
        [](std::pair<f32, raw_ptr<Effect>> const& lhs,
            std::pair<f32, raw_ptr<Effect>> const& rhs)->bool {
                return lhs.first > rhs.first;
    };
    std::sort (sortedEffect.begin(), sortedEffect.end(), sortFunction);
    for (auto& zAndEffect : sortedEffect)
    {
        zAndEffect.second->Render(*renderModule_, *this);
    }
    effects_.clear();
}
auto Kumazuma::Graphics::GameRendererImpl::GetFrustum(Frustum* const pOutFrustum) -> HRESULT
{
    if (pOutFrustum == nullptr) return E_POINTER;
    *pOutFrustum = frustum_;
    return S_OK;
}
auto Kumazuma::Graphics::GameRendererImpl::SetNearFar(f32 nearZ, f32 farZ) -> void
{
    minZ_ = nearZ;
    maxZ_ = farZ;
}
auto Kumazuma::Graphics::GameRendererImpl::AddLight(D3DLIGHT9 const& light) -> void
{
    lights_.emplace_back(light);
}
auto Kumazuma::Graphics::GameRendererImpl::GetViewMatrix(DirectX::XMFLOAT4X4* pOut) -> HRESULT
{
    *pOut = this->viewMatrix_;
    return S_OK;
}
auto GameRendererImpl::DeferredLighting(RenderModule* pRenderModule) -> void
{
    HRESULT hr{};
    XMMATRIX mViewProj{};
    XMFLOAT4X4 viewPorjMatrix{};
    XMMATRIX mView{ XMLoadFloat4x4(&viewMatrix_) };
    XMMATRIX mProj{ XMLoadFloat4x4(&projMatrix_) };
    mViewProj = mView * mProj;
    XMStoreFloat4x4(&viewPorjMatrix, mViewProj);

    ComPtr<IDirect3DDevice9> pDevice;
    ComPtr<IDirect3DSurface9> lightDiffuseTarget;
    ComPtr<IDirect3DSurface9> lightSpecularTarget;
    ComPtr<IDirect3DTexture9> depthMapTexture{};
    ComPtr<IDirect3DTexture9> normalMapTexture{};
    ComPtr<IDirect3DTexture9> specularMapTexture{};

    ComPtr<IRenderTarget> renderTarget;

    pRenderModule->GetRenderTarget(LIGHT_DIFFUSE, renderTarget.ReleaseAndGetAddressOf());
    renderTarget->GetSurface(&lightDiffuseTarget);
    
    pRenderModule->GetRenderTarget(LIGHT_SPECULAR, renderTarget.ReleaseAndGetAddressOf());
    renderTarget->GetSurface(&lightSpecularTarget);
    
    pRenderModule->GetRenderTarget(RENDER_TARGET_NORMAL, renderTarget.ReleaseAndGetAddressOf());
    static_cast<TextureRenderTarget*>(renderTarget.Get())->GetTexture(&normalMapTexture);

    pRenderModule->GetRenderTarget(RENDER_TARGET_SPECULAR, renderTarget.ReleaseAndGetAddressOf());
    static_cast<TextureRenderTarget*>(renderTarget.Get())->GetTexture(&specularMapTexture);

    pRenderModule->GetRenderTarget(RENDER_TARGET_DEPTH, renderTarget.ReleaseAndGetAddressOf());
    static_cast<TextureRenderTarget*>(renderTarget.Get())->GetTexture(&depthMapTexture);

    pRenderModule->GetDevice(&pDevice);

    XMVECTOR vCameraPsotion = XMVectorSetW(XMMatrixInverse(nullptr, mView).r[3], 1.f);
    pDevice->SetRenderTarget(0, lightDiffuseTarget.Get());
    pDevice->SetRenderTarget(1, lightSpecularTarget.Get());
    pDevice->SetRenderTarget(2, nullptr);
    pDevice->SetRenderTarget(3, nullptr);
    pDevice->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_COLORVALUE(0.f, 0.f, 0.f, 0.f), 1.f, 0);
    pDevice->SetFVF(FVF);
    pDevice->SetIndices(indexBuffer_.Get());
    pDevice->SetStreamSource(0, vertexBuffer_.Get(), 0, sizeof(PPVertexFVF));

    XMMATRIX mInserseViewProj{ XMMatrixInverse(nullptr, mViewProj) };
    D3DXMATRIX InverseViewProMatrix{};
    XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&InverseViewProMatrix), mInserseViewProj);
    lightingShader_->SetMatrix(ID_CONST_INVSERSE_VIEW_PORJ_MATRIX, &InverseViewProMatrix);
    lightingShader_->SetTexture(ID_TEX_NORMAL_MAP, normalMapTexture.Get());
    lightingShader_->SetTexture(ID_TEX_SPECULAR_MAP, specularMapTexture.Get());
    lightingShader_->SetTexture(ID_TEX_DEPTH_MAP, depthMapTexture.Get());

    UINT passCount{};
    lightingShader_->Begin(&passCount, 0);

    hr = lightingShader_->SetVector("g_vCameraPosition", reinterpret_cast<D3DXVECTOR4 const*>(&vCameraPsotion));

    for (auto& light : lights_)
    {
        int passNum{ -1 };
        switch (light.Type)
        {
        case D3DLIGHT_DIRECTIONAL:
            passNum = 0;
            {
                XMVECTOR vLightDir{ XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&light.Direction)) };
                vLightDir = XMVectorSetW(vLightDir, 1.f);
                lightingShader_->SetVector("g_vLightDirectionAndPower", reinterpret_cast<D3DXVECTOR4 const*>(&vLightDir));
                lightingShader_->SetVector("g_vLightAmbient", reinterpret_cast<D3DXVECTOR4 const*>(&light.Ambient));
                lightingShader_->SetVector("g_vLightDiffuse", reinterpret_cast<D3DXVECTOR4 const*>(&light.Diffuse));
            }
            break;
        }
        if (passNum == -1)continue;
        lightingShader_->BeginPass(passNum);
        pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
        lightingShader_->EndPass();
    }
    lightingShader_->End();
    lights_.clear();
}
_END_NS