#include "pch.h"
#include "GraphicsModuleImpl.hpp"
#include "RenderSystemImpl.hpp"
#include "TextureManagerImpl.h"
#include "TextureManager.hpp"
#include "Texture2DBuilder.hpp"
#include <d3dcompiler.h>
Kumazuma::GraphicsModuleImpl::GraphicsModuleImpl( )
{
    UINT creationFlags{ D3D11_CREATE_DEVICE_BGRA_SUPPORT };
    D3D_FEATURE_LEVEL featureLevel{ D3D_FEATURE_LEVEL_11_0 };

#if defined(_DEBUG)
    // If the project is in a debug build, enable the debug layer.
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        &featureLevel,
        1,
        D3D11_SDK_VERSION,
        &device_,
        nullptr,
        &deviceContext_
    );
    CreateDXGIFactory(__uuidof(IDXGIFactory), &dxgiFactory_);
    //swapChainTexture_.reset(new SwapChainTexture2D(device_.Get(), swapChain_.Get()));
    //defaultDepthBuffer_.reset(Texture2D::Create(device_.Get(), Texture2D::Builder(DXGI_FORMAT_D32_FLOAT, bufferSize.width, bufferSize.height).DepthStancilView()));

    textureManager_.reset(new TextureManagerImpl{ this });

    RenderSystemImpl::Initialize(this);
}
//ComPtr<ID3D11Device>		device_;
//ComPtr<ID3D11DeviceContext> deviceContext_;
//ComPtr<IDXGISwapChain>		swapChain_;
//std::unique_ptr<TextureManager>							textureManager_;
//std::unique_ptr<Kumazuma::Texture2D>					swapChainTexture_;
//std::unordered_map<std::wstring, ComPtr<ID3D11Buffer> > cbuffers_;
Kumazuma::GraphicsModuleImpl::GraphicsModuleImpl(GraphicsModuleImpl&& rhs) noexcept:
    device_{std::move(rhs.device_)},
    deviceContext_{std::move(rhs.deviceContext_)},
    //swapChain_{std::move(rhs.swapChain_)},
    textureManager_{std::move(rhs.textureManager_)},
    //swapChainTexture_{std::move(rhs.swapChainTexture_)},
    cbuffers_{std::move(rhs.cbuffers_)},
    //defaultDepthBuffer_{std::move(rhs.defaultDepthBuffer_)},
    shaders_{std::move(rhs.shaders_)},
    dxgiFactory_{std::move(rhs.dxgiFactory_)}
{
    static_cast<TextureManagerImpl*>(textureManager_.get())->SetGraphicsModule(this);
}


HRESULT Kumazuma::GraphicsModuleImpl::GetDevice(ID3D11Device** out)
{
    if (out == nullptr)
    {
        return E_POINTER;
    }
    ID3D11Device* device{ device_.Get() };
    *out = device;
    device->AddRef();
    return S_OK;
}

HRESULT Kumazuma::GraphicsModuleImpl::GetImmediateContext(ID3D11DeviceContext** out)
{
    if (out == nullptr)
    {
        return E_POINTER;
    }
    ID3D11DeviceContext* deviceContext{ deviceContext_.Get() };
    *out = deviceContext;
    deviceContext->AddRef();
    return S_OK;
}
//
//HRESULT Kumazuma::GraphicsModuleImpl::GetSwapChain(IDXGISwapChain** out)
//{
//    if (out == nullptr)
//    {
//        return E_POINTER;
//    }
//    IDXGISwapChain* swapChain{ swapChain_.Get() };
//    *out = swapChain;
//    swapChain->AddRef();
//    return S_OK;
//}

HRESULT Kumazuma::GraphicsModuleImpl::CreateCBuffer(wchar_t const* name, size_t bufferSize)
{
    auto findIt{ cbuffers_.find(name) };
    if (findIt != cbuffers_.end())
    {
        return E_INVALIDARG;
    }
    ComPtr<ID3D11Buffer> newBuffer;
    HRESULT hr;
    CD3D11_BUFFER_DESC bufferDesc{static_cast<UINT>(bufferSize), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE };
    hr = device_->CreateBuffer(&bufferDesc, nullptr, &newBuffer);
    if (FAILED(hr))
    {
        return E_FAIL;
    }
    cbuffers_.emplace(name, newBuffer);
    return S_OK;
}

HRESULT Kumazuma::GraphicsModuleImpl::GetCBuffer(wchar_t const* name, ID3D11Buffer** out)
{
    auto findIt{ cbuffers_.find(name) };
    if (findIt == cbuffers_.end())
    {
        return E_INVALIDARG;
    }
    ID3D11Buffer* buffer = findIt->second.Get();
    *out = buffer;
    buffer->AddRef();
    return S_OK;
}

HRESULT Kumazuma::GraphicsModuleImpl::LoadPixelShader(wchar_t const* id, wchar_t const* path, char const* entry)
{
    HRESULT hr{};
    std::unordered_map<std::wstring, ComPtr<ID3D11DeviceChild> >& pixelShaders =
        shaders_[__uuidof(ID3D11PixelShader)];
    auto shaderIt{ pixelShaders.find(id) };
    if (shaderIt != pixelShaders.end())
    {
        return E_INVALIDARG;
    }
    ComPtr<ID3DBlob> code;
    ComPtr<ID3DBlob> errMsg;
    ComPtr<ID3D11PixelShader> pixelShader;
    hr = D3DCompileFromFile(path, nullptr, nullptr, entry, "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, &code, &errMsg);
    if (FAILED(hr))
    {
        return E_FAIL;
    }
    device_->CreatePixelShader(code->GetBufferPointer(), code->GetBufferSize(), nullptr, &pixelShader);
    pixelShaders.emplace(id, pixelShader);
    return S_OK;
}

HRESULT Kumazuma::GraphicsModuleImpl::LoadPixelShaderFromBytes(wchar_t const* id, u8 const* ptr, u32 len)
{
    HRESULT hr{};
    std::unordered_map<std::wstring, ComPtr<ID3D11DeviceChild> >& pixelShaders =
        shaders_[__uuidof(ID3D11PixelShader)];
    auto shaderIt{ pixelShaders.find(id) };
    if (shaderIt != pixelShaders.end())
    {
        return E_INVALIDARG;
    }
    ComPtr<ID3DBlob> code;
    ComPtr<ID3DBlob> errMsg;
    ComPtr<ID3D11PixelShader> pixelShader;

    device_->CreatePixelShader(code->GetBufferPointer(), code->GetBufferSize(), nullptr, &pixelShader);
    pixelShaders.emplace(id, pixelShader);
    return S_OK;
}

HRESULT Kumazuma::GraphicsModuleImpl::GetPixelShader(wchar_t const* id, ID3D11PixelShader** out)
{
    std::unordered_map<std::wstring, ComPtr<ID3D11DeviceChild> >& pixelShaders =
        shaders_[__uuidof(ID3D11PixelShader)];

    auto shaderIt{ pixelShaders.find(id) };
    if (shaderIt == pixelShaders.end())
    {
        return E_INVALIDARG;
    }
    shaderIt->second->QueryInterface(out);
    return S_OK;
}

HRESULT Kumazuma::GraphicsModuleImpl::LoadComputeShader(wchar_t const* id, wchar_t const* path, char const* entry)
{
    HRESULT hr{};
    std::unordered_map<std::wstring, ComPtr<ID3D11DeviceChild> >& computeShaders =
        shaders_[__uuidof(ID3D11ComputeShader)];
    auto shaderIt{ computeShaders.find(id) };
    if (shaderIt != computeShaders.end())
    {
        return E_INVALIDARG;
    }
    ComPtr<ID3DBlob> code;
    ComPtr<ID3DBlob> errMsg;
    ComPtr<ID3D11ComputeShader> computeShader;
    hr = D3DCompileFromFile(path, nullptr, nullptr, entry, "cs_5_0", D3D10_SHADER_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3
        , 0, &code, &errMsg);
    if (FAILED(hr))
    {
        return E_FAIL;
    }
    device_->CreateComputeShader(code->GetBufferPointer(), code->GetBufferSize(), nullptr, &computeShader);
    computeShaders.emplace(id, computeShader);
    return S_OK;
}

HRESULT Kumazuma::GraphicsModuleImpl::LoadComputeShaderFromBytes(wchar_t const* id, u8 const* ptr, u32 len)
{
    HRESULT hr{};
    std::unordered_map<std::wstring, ComPtr<ID3D11DeviceChild> >& computeShaders =
        shaders_[__uuidof(ID3D11ComputeShader)];
    auto shaderIt{ computeShaders.find(id) };
    if (shaderIt != computeShaders.end())
    {
        return E_INVALIDARG;
    }
    ComPtr<ID3D11ComputeShader> computeShader;
    device_->CreateComputeShader(ptr, len, nullptr, &computeShader);
    computeShaders.emplace(id, computeShader);
    return S_OK;
}

HRESULT Kumazuma::GraphicsModuleImpl::GetComputeShader(wchar_t const* id, ID3D11ComputeShader** out)
{
    std::unordered_map<std::wstring, ComPtr<ID3D11DeviceChild> >& pixelShaders =
        shaders_[__uuidof(ID3D11ComputeShader)];

    auto shaderIt{ pixelShaders.find(id) };
    if (shaderIt == pixelShaders.end())
    {
        return E_INVALIDARG;
    }
    shaderIt->second->QueryInterface(out);
    return S_OK;
}


Kumazuma::TextureManager& Kumazuma::GraphicsModuleImpl::GetTextureManager()
{
    return *textureManager_;
}


IDXGIFactory* Kumazuma::GraphicsModuleImpl::GetDXGIFactory()
{
    return dxgiFactory_.Get();
}

//Kumazuma::Texture2D* Kumazuma::GraphicsModuleImpl::GetSwapChainTexture()
//{
//    return swapChainTexture_.get();
//}
//
//Kumazuma::Texture2D* Kumazuma::GraphicsModuleImpl::GetDefaultDepthBuffer()
//{
//    return defaultDepthBuffer_.get();;
//}
