#include "pch.h"
#include "MapToolRenderer.h"
#include "TextureRenderTarget.h"
#include"RenderEntity.h"
#include <iostream>
using namespace DirectX;
struct PPVertexFVF { XMFLOAT3 xyz; XMFLOAT2 uv; };

MapToolRenderer::MapToolRenderer(RenderModule* pRenderModule, u32 width, u32 height):
    m_viewMatrix{  },
    m_projMatrix{  },
    m_frustum{}
{
    COMPtr<IDirect3DDevice9> pDevice{};
    pRenderModule->GetDevice(&pDevice);
    m_viewMatrix(0, 0) = m_viewMatrix(1, 1) = m_viewMatrix(2, 2) = m_viewMatrix(3, 3)  = 1.f;
    m_projMatrix = m_viewMatrix;
    HRESULT hr;
    TextureRenderTarget* pAlbedo{};
    TextureRenderTarget* pNormalMap{};
    TextureRenderTarget* pMaterialSpecularMap{};
    TextureRenderTarget* pLightSpecularMap{};
    TextureRenderTarget* pLightDiffuseMap{};
    TextureRenderTarget* pDepthMap{};

    //Diffuse
    hr = TextureRenderTarget::Create(pRenderModule, width, height, D3DFMT_A8R8G8B8, &pAlbedo);
    if (FAILED(hr))throw hr;
    m_albedoSurface.reset(pAlbedo);
    //normal
    hr = TextureRenderTarget::Create(pRenderModule, width, height, D3DFMT_A32B32G32R32F, &pNormalMap);
    if (FAILED(hr))throw hr;
    m_normalSurface.reset(pNormalMap);
    //specular
    hr = TextureRenderTarget::Create(pRenderModule, width, height, D3DFMT_A8R8G8B8, &pMaterialSpecularMap);
    if (FAILED(hr))throw hr;
    m_materialSpecularSurface.reset(pMaterialSpecularMap);

    hr = TextureRenderTarget::Create(pRenderModule, width, height, D3DFMT_A16B16G16R16F, &pLightDiffuseMap);
    if (FAILED(hr))throw hr;
    m_lightDiffuseSurface.reset(pLightDiffuseMap);

    hr = TextureRenderTarget::Create(pRenderModule, width, height, D3DFMT_R32F, &pDepthMap);
    if (FAILED(hr))throw hr;
    m_depthSurface.reset(pDepthMap);

    hr = TextureRenderTarget::Create(pRenderModule, width, height, D3DFMT_A16B16G16R16F, &pLightSpecularMap);
    if (FAILED(hr))throw hr;
    m_lightSpecularSurface.reset(pLightSpecularMap);

    DWORD shaderFlag{ D3DXSHADER_DEBUG };


    COMPtr<ID3DXBuffer> pBuffer;
    hr = D3DXCreateEffectFromFileW(pDevice.Get(), L"./maptool.fx", nullptr, nullptr, shaderFlag, nullptr, &m_effect, &pBuffer);
    if (pBuffer != nullptr)
    {
        char* msg{ reinterpret_cast<char*>(pBuffer->GetBufferPointer()) };
        std::clog << __FILE__ << ":" << __LINE__ << " shader error!\n" << pBuffer->GetBufferPointer() << '\n';
    }
    if (FAILED(hr))
    {
        throw hr;
    }
    hr = D3DXCreateEffectFromFileW(pDevice.Get(), L"./maptool_lighting.fx", nullptr, nullptr, shaderFlag, nullptr, &m_lightingEffect, &pBuffer);
    if (pBuffer != nullptr)
    {
        char* msg{ reinterpret_cast<char*>(pBuffer->GetBufferPointer()) };
        std::clog << __FILE__ << ":" << __LINE__ << " shader error!\n" << pBuffer->GetBufferPointer() << '\n';
    }
    if (FAILED(hr))
    {
        throw hr;
    }

    hr = D3DXCreateEffectFromFileW(pDevice.Get(), L"./maptool_combine.fx", nullptr, nullptr, shaderFlag, nullptr, &m_combineEffect, &pBuffer);
    if (pBuffer != nullptr)
    {
        char* msg{ reinterpret_cast<char*>(pBuffer->GetBufferPointer()) };
        std::clog << __FILE__ << ":" << __LINE__ << " shader error!\n" << pBuffer->GetBufferPointer() << '\n';
    }
    if (FAILED(hr))
    {
        throw hr;
    }
    pDevice->CreateVertexBuffer(sizeof(PPVertexFVF) * 4, 0, FVF, D3DPOOL_MANAGED, &m_pVertexBuffer, nullptr);
    pDevice->CreateIndexBuffer(sizeof(int) * 6, 0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_pIndexBuffer, nullptr);
    PPVertexFVF* pVertices;
    m_pVertexBuffer->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);
    pVertices[0].uv = XMFLOAT2{ 0.f, 0.f };
    pVertices[0].xyz = XMFLOAT3{ -1.f, 1.f, 0.f };
    
    pVertices[1].uv = XMFLOAT2{ 1.f, 0.f };
    pVertices[1].xyz = XMFLOAT3{ 1.f, 1.f, 0.f };

    pVertices[2].uv = XMFLOAT2{ 1.f, 1.f };
    pVertices[2].xyz = XMFLOAT3{ 1.f, -1.f, 0.f };

    pVertices[3].uv = XMFLOAT2{ 0.f, 1.f };
    pVertices[3].xyz = XMFLOAT3{ -1.f, -1.f, 0.f };
    m_pVertexBuffer->Unlock();
    int* pIndices;
    m_pIndexBuffer->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);

    pIndices[0] = 0;
    pIndices[1] = 1; 
    pIndices[2] = 3;

    pIndices[3] = 1;
    pIndices[4] = 2;
    pIndices[5] = 3;

    m_pIndexBuffer->Unlock();

    D3DXCreateSprite(pDevice.Get(), &m_sprite);
    D3DLIGHT9 light{};
    light.Type = D3DLIGHT_DIRECTIONAL;
    light.Ambient = D3DCOLORVALUE{ 0.1f, 0.1f, 0.1f, 0.1f };
    light.Diffuse = D3DCOLORVALUE{ 1.0f, 1.0f, 1.0f, 1.0f };
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&light.Direction), XMVector3Normalize(XMVectorSet(0.f, -2.f, 0.f, 0.f)));
    m_lights.emplace(L"global_light", light);
}

MapToolRenderer::MapToolRenderer(MapToolRenderer&& rhs) noexcept:
    m_albedoSurface{std::move(rhs.m_albedoSurface)},
    m_normalSurface{std::move(rhs.m_normalSurface)},
    m_materialSpecularSurface{std::move(rhs.m_materialSpecularSurface)},
    m_lightDiffuseSurface{std::move(rhs.m_lightDiffuseSurface)},
    m_renderEntities{std::move(rhs.m_renderEntities)},
    m_viewMatrix{std::move(rhs.m_viewMatrix)},
    m_projMatrix{ std::move(rhs.m_projMatrix) },
    m_frustum{std::move(rhs.m_frustum)},
    m_effect{std::move(rhs.m_effect)},
    m_lightingEffect{std::move(rhs.m_lightingEffect)},
    m_combineEffect{std::move(rhs.m_combineEffect)},
    m_sprite{std::move(rhs.m_sprite)},
    m_pVertexBuffer{std::move(rhs.m_pVertexBuffer)},
    m_pIndexBuffer{std::move(rhs.m_pIndexBuffer)},
    m_depthSurface{std::move(rhs.m_depthSurface)},
    m_lightSpecularSurface{std::move(rhs.m_lightSpecularSurface)},
    m_lights{std::move(rhs.m_lights)}
{

}

auto MapToolRenderer::Create(RenderModule* pRenderModule, u32 width, u32 height, MapToolRenderer** pOut) -> HRESULT
{
    try
    {
        if(pOut == nullptr)
        {
            std::clog << __FILE__ << ":" << __LINE__ << " pOut is null!\n";
            throw E_POINTER;
        }
        MapToolRenderer obj{ pRenderModule, width, height };
        *pOut = new MapToolRenderer{ std::move(obj) };
        return S_OK;
    }
    catch (HRESULT hr)
    {
        return hr;
    }
    return E_NOTIMPL;
}

auto MapToolRenderer::Render(RenderModule* const pRenderModule) -> void 
{
    COMPtr<IDirect3DDevice9> pDevice;
    XMStoreFloat4x4(&m_viewProjMatrix, XMLoadFloat4x4(&m_viewMatrix) * XMLoadFloat4x4(&m_projMatrix));
    pRenderModule->GetDevice(&pDevice);
    if (!pRenderModule->Renderable())
    {
        ClearEntityTable();
        return;
    }
    pDevice->SetTransform(D3DTS_VIEW, reinterpret_cast<D3DMATRIX*>(&m_viewMatrix));
    pDevice->SetTransform(D3DTS_PROJECTION, reinterpret_cast<D3DMATRIX*>(&m_projMatrix));
    pDevice->Clear(0, nullptr, D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, D3DCOLOR_COLORVALUE(0.f, 0.f, 1.f, 0.f), 1.f, 0);
    
    pDevice->BeginScene();
    for (auto& it : m_renderEntities[Kind::ENVIRONMENT])
    {
        it->Render(pRenderModule, this);
    }

    DefferedRender(pRenderModule);
    Lighting(pRenderModule);
    Combine(pRenderModule);
    pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    for (auto& it : m_renderEntities[Kind::ALPHA])
    {
        it->Render(pRenderModule, this);
    }
    for (auto& it : m_renderEntities[Kind::NAVIMASH])
    {
        it->Render(pRenderModule, this);
    }
    for (auto& it : m_renderEntities[Kind::UI])
    {
        it->Render(pRenderModule, this);
    }

    pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_sprite->Begin(D3DXSPRITE_ALPHABLEND);
    RECT rc{};
    COMPtr<IDirect3DTexture9> pNormalMapTexture;
    COMPtr<IDirect3DTexture9> pSpecularMapTexture;
    COMPtr<IDirect3DTexture9> pAlbedoMapTexture;
    COMPtr<IDirect3DTexture9> pShadeMapTexture;
    COMPtr<IDirect3DTexture9> pDepthMapTexture;
    COMPtr<IDirect3DTexture9> pLightSpecularMapTexture;
    m_normalSurface->GetTexture(&pNormalMapTexture);
    m_materialSpecularSurface->GetTexture(&pSpecularMapTexture);
    m_albedoSurface->GetTexture(&pAlbedoMapTexture);
    m_lightDiffuseSurface->GetTexture(&pShadeMapTexture);
    m_lightSpecularSurface->GetTexture(&pLightSpecularMapTexture);
    m_depthSurface->GetTexture(&pDepthMapTexture);
    D3DXMATRIX tmpMat;
    D3DXMATRIX tmpMat2;
    D3DXMatrixScaling(&tmpMat, .2f, .2f, .2f);
    D3DXMatrixTranslation(&tmpMat2, pRenderModule->GetWidth() * 0.2f , 0 , 0 );

    m_sprite->SetTransform(&tmpMat);
    m_sprite->Draw(pNormalMapTexture.Get(), nullptr, nullptr, nullptr, D3DCOLOR_COLORVALUE(1.f, 1.f, 1.f, 1.f));

    tmpMat = tmpMat * tmpMat2;

    m_sprite->SetTransform(&tmpMat);
    m_sprite->Draw(pSpecularMapTexture.Get(), nullptr, nullptr, nullptr, D3DCOLOR_COLORVALUE(1.f, 1.f, 1.f, 1.f));

    tmpMat = tmpMat * tmpMat2;
    m_sprite->SetTransform(&tmpMat);
    m_sprite->Draw(pAlbedoMapTexture.Get(), nullptr, nullptr, nullptr, D3DCOLOR_COLORVALUE(1.f, 1.f, 1.f, 1.f));

    tmpMat = tmpMat * tmpMat2;
    m_sprite->SetTransform(&tmpMat);
    m_sprite->Draw(pShadeMapTexture.Get(), nullptr, nullptr, nullptr, D3DCOLOR_COLORVALUE(1.f, 1.f, 1.f, 1.f));

    tmpMat = tmpMat * tmpMat2;
    m_sprite->SetTransform(&tmpMat);
    m_sprite->Draw(pDepthMapTexture.Get(), nullptr, nullptr, nullptr, D3DCOLOR_COLORVALUE(1.f, 1.f, 1.f, 1.f));

    m_sprite->End();
    pDevice->EndScene();

    ClearEntityTable();
}

auto MapToolRenderer::AddEntity(RenderModule::Kind kind, std::shared_ptr<RenderEntity> const& entity) -> void 
{
    m_renderEntities[kind].push_back(entity);
}

auto MapToolRenderer::SetProjMatrix(DirectX::XMFLOAT4X4 const& mat) -> void 
{
    m_projMatrix = mat;
    m_frustum.MakeFrustum(XMLoadFloat4x4(&m_viewMatrix), XMLoadFloat4x4(&m_projMatrix));
}

auto MapToolRenderer::SetViewMatrix(DirectX::XMFLOAT4X4 const& mat) -> void 
{
    m_viewMatrix = mat;
    m_frustum.MakeFrustum(XMLoadFloat4x4(&m_viewMatrix), XMLoadFloat4x4(&m_projMatrix));
}

auto MapToolRenderer::GetEffect(ID3DXEffect** const pOut) -> HRESULT
{
    if (pOut == nullptr)return E_POINTER;
    *pOut = this->m_effect.Get();
    this->m_effect->AddRef();
    return S_OK;
}

auto MapToolRenderer::GetProjMatrix(DirectX::XMFLOAT4X4* const pOut) -> HRESULT
{
    if (pOut == nullptr) return E_POINTER;
    *pOut = m_projMatrix;
    return S_OK;
}

auto MapToolRenderer::GetViewMatrix(DirectX::XMFLOAT4X4* const pOut) -> HRESULT 
{
    if (pOut == nullptr) return E_POINTER;
    *pOut = m_viewMatrix;
    return S_OK;
}

auto MapToolRenderer::GetFrustum(Frustum* const pOutFrustum) -> HRESULT 
{
    if (pOutFrustum == nullptr) return E_POINTER;
    *pOutFrustum = m_frustum;
    return S_OK;
}

auto MapToolRenderer::Delete() -> void 
{
    delete this;
}

auto MapToolRenderer::DefferedRender(RenderModule* pRenderModule) -> void
{
    COMPtr<IDirect3DDevice9> pDevice;
    COMPtr<IDirect3DSurface9> albedoTarget;
    COMPtr<IDirect3DSurface9> normalTarget;
    COMPtr<IDirect3DSurface9> specularTarget;
    COMPtr<IDirect3DSurface9> shadeTarget;
    COMPtr<IDirect3DSurface9> depthTarget;
    m_albedoSurface->GetSurface(&albedoTarget);
    m_normalSurface->GetSurface(&normalTarget);
    m_materialSpecularSurface->GetSurface(&specularTarget);
    m_lightDiffuseSurface->GetSurface(&shadeTarget);
    m_depthSurface->GetSurface(&depthTarget);
    pRenderModule->GetDevice(&pDevice);
    
    pDevice->SetRenderTarget(0, albedoTarget.Get());
    pDevice->SetRenderTarget(1, normalTarget.Get());
    pDevice->SetRenderTarget(2, specularTarget.Get());
    pDevice->SetRenderTarget(3, depthTarget.Get());
    pDevice->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_COLORVALUE(0.f, 0.f, 0.f, 0.f), 1.f, 0);

    COMPtr<IDirect3DSurface9> backbuffer;
    UINT passCount{};
    D3DXVECTOR4 defaultSpecular{ 1.0f,1.0f,1.0f,1.f };
    m_effect->SetMatrix("g_mView", reinterpret_cast<D3DXMATRIX*>(&m_viewMatrix));
    m_effect->SetMatrix("g_mViewProj", reinterpret_cast<D3DXMATRIX*>(&m_viewProjMatrix));
    m_effect->SetVector("g_vSpecular", &defaultSpecular);
    m_effect->Begin(&passCount, 0);
    m_effect->BeginPass(0);
    for (auto& it : m_renderEntities[Kind::TERRAIN])
    {
        it->Render(pRenderModule, this);
        m_effect->SetVector("g_vSpecular", &defaultSpecular);
    }
    for (auto& it : m_renderEntities[Kind::NONALPHA])
    {
        it->Render(pRenderModule, this);
        m_effect->SetVector("g_vSpecular", &defaultSpecular);
    }
    m_effect->EndPass();
    m_effect->End();
    pDevice->SetVertexShader(nullptr);
    pDevice->SetPixelShader(nullptr);
}

auto MapToolRenderer::Lighting(RenderModule* pRenderModule) -> void
{
    HRESULT hr{};
    COMPtr<IDirect3DDevice9> pDevice;
    COMPtr<IDirect3DSurface9> lightDiffuseTarget;
    COMPtr<IDirect3DSurface9> lightSpecularTarget;
    COMPtr<IDirect3DSurface9> depthTarget;
    COMPtr<IDirect3DTexture9> normalDepthMapTexture{};
    COMPtr<IDirect3DTexture9> specularMapTexture{};
    m_lightDiffuseSurface->GetSurface(&lightDiffuseTarget);
    m_lightSpecularSurface->GetSurface(&lightSpecularTarget);
    m_materialSpecularSurface->GetTexture(&specularMapTexture);
    m_normalSurface->GetTexture(&normalDepthMapTexture);
    pRenderModule->GetDevice(&pDevice);

    XMMATRIX mView{ XMLoadFloat4x4(&m_viewMatrix) };
    XMVECTOR vCameraPsotion = XMVectorSetW(XMMatrixInverse(nullptr, mView).r[3], 1.f);
    pDevice->SetRenderTarget(0, lightDiffuseTarget.Get());
    pDevice->SetRenderTarget(1, lightSpecularTarget.Get());
    pDevice->SetRenderTarget(2, nullptr);
    pDevice->SetRenderTarget(3, nullptr);
    pDevice->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_COLORVALUE(0.f, 0.f, 0.f, 0.f), 1.f, 0);
    pDevice->SetFVF(FVF);
    pDevice->SetIndices(m_pIndexBuffer.Get());
    pDevice->SetStreamSource(0, m_pVertexBuffer.Get(), 0, sizeof(PPVertexFVF));

    XMMATRIX mInserseViewProj{ XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_viewProjMatrix)) };
    m_lightingEffect->SetMatrix("g_mInverseViewProj", reinterpret_cast<D3DXMATRIX const*>(&mInserseViewProj));
    m_lightingEffect->SetTexture("g_normalMap", normalDepthMapTexture.Get());
    m_lightingEffect->SetTexture("g_specularMap", specularMapTexture.Get());
    
    UINT passCount{};
    m_lightingEffect->Begin(&passCount, 0);
    

    hr = m_lightingEffect->SetVector("g_vCameraPosition", reinterpret_cast<D3DXVECTOR4 const*>(&vCameraPsotion));
    if (FAILED(hr))
    {
        std::clog << "asd\n";
    }
    m_lightingEffect->SetFloat("g_fPower", 90.f);
    for (auto& lightItem : m_lights)
    {
        auto light{ lightItem.second };
        int passNum{-1};
        switch (light.Type)
        {
        case D3DLIGHT_DIRECTIONAL:
            passNum = 0;
            {
                XMVECTOR vLightDir{ XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&light.Direction)) };
                vLightDir = XMVectorSetW(vLightDir, 1.f);
                m_lightingEffect->SetVector("g_vLightDirectionAndPower", reinterpret_cast<D3DXVECTOR4 const*>(&vLightDir));
                m_lightingEffect->SetVector("g_vLightAmbient", reinterpret_cast<D3DXVECTOR4 const*>(&light.Ambient));
                m_lightingEffect->SetVector("g_vLightDiffuse", reinterpret_cast<D3DXVECTOR4 const*>(&light.Diffuse));
                m_lightingEffect->SetFloat("g_fPower", 90.f);
            }
            break;
        }
        if (passNum == -1)continue;
        m_lightingEffect->BeginPass(passNum);
        pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
        m_lightingEffect->EndPass();
    }
    m_lightingEffect->End();
}

auto MapToolRenderer::Combine(RenderModule* pRenderModule) -> void
{

    COMPtr<IDirect3DSwapChain9> pSwapChain{};
    COMPtr<IDirect3DSurface9> pBackbuffer{};
    COMPtr<IDirect3DDevice9> pDevice;
    COMPtr<IDirect3DTexture9> pLightSpecularMapTexture;
    COMPtr<IDirect3DTexture9> pLightDiffuseMapTexture;
    COMPtr<IDirect3DTexture9> pAlbedoMapTexture;
    pRenderModule->GetDefaultSwapChain(&pSwapChain);
    pRenderModule->GetDevice(&pDevice);
    pSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackbuffer);
    m_albedoSurface->GetTexture(&pAlbedoMapTexture);
    m_lightSpecularSurface->GetTexture(&pLightSpecularMapTexture);
    m_lightDiffuseSurface->GetTexture(&pLightDiffuseMapTexture);

    pDevice->SetRenderTarget(0, pBackbuffer.Get());
    pDevice->SetRenderTarget(1, nullptr);
    pDevice->SetRenderTarget(2, nullptr);
    pDevice->SetRenderTarget(3, nullptr);
    UINT passCount{};
    
    pDevice->SetFVF(FVF);
    pDevice->SetIndices(m_pIndexBuffer.Get());
    pDevice->SetStreamSource(0, m_pVertexBuffer.Get(), 0, sizeof(PPVertexFVF));

    m_combineEffect->Begin(&passCount, 0);
    m_combineEffect->SetTexture("g_shadeMap", pLightDiffuseMapTexture.Get());
    m_combineEffect->SetTexture("g_specularMap", pLightSpecularMapTexture.Get());
    m_combineEffect->SetTexture("g_albedoMap", pAlbedoMapTexture.Get());
    m_combineEffect->BeginPass(0);
    pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
    m_combineEffect->EndPass();
    m_combineEffect->End();
}

auto MapToolRenderer::ClearEntityTable() -> void
{
    m_renderEntities.clear();
}