#include "pch.h"
#include "GraphicsModuleImpl.hpp"
#include "TextureManagerImpl.h"
#include "TextureManager.hpp"
#include "Texture2DBuilder.hpp"
Kumazuma::GraphicsModuleImpl::GraphicsModuleImpl(HWND hWindow, Size2D<u32> const& bufferSize, bool fullScreen)
{
    UINT creationFlags{ D3D11_CREATE_DEVICE_BGRA_SUPPORT };
    D3D_FEATURE_LEVEL featureLevel{ D3D_FEATURE_LEVEL_11_0 };
    DXGI_SWAP_CHAIN_DESC swapChainDesc{};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = bufferSize.width;
    swapChainDesc.BufferDesc.Height = bufferSize.height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.OutputWindow = hWindow;
    swapChainDesc.Windowed = !fullScreen;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags = 0;
#if defined(_DEBUG)
    // If the project is in a debug build, enable the debug layer.
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        &featureLevel,
        1,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &swapChain_,
        &device_,
        nullptr,
        &deviceContext_
    );
    swapChainTexture_.reset(new SwapChainTexture2D(device_.Get(), swapChain_.Get()));

    textureManager_.reset(new TextureManagerImpl{ this });
    defaultDepthBuffer_.reset(Texture2D::Create(device_.Get(), Texture2D::Builder(DXGI_FORMAT_D32_FLOAT, bufferSize.width, bufferSize.height).DepthStancilView()));
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
    swapChain_{std::move(rhs.swapChain_)},
    textureManager_{std::move(rhs.textureManager_)},
    swapChainTexture_{std::move(rhs.swapChainTexture_)},
    cbuffers_{std::move(rhs.cbuffers_)},
    defaultDepthBuffer_{std::move(rhs.defaultDepthBuffer_)}
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

HRESULT Kumazuma::GraphicsModuleImpl::GetSwapChain(IDXGISwapChain** out)
{
    if (out == nullptr)
    {
        return E_POINTER;
    }
    IDXGISwapChain* swapChain{ swapChain_.Get() };
    *out = swapChain;
    swapChain->AddRef();
    return S_OK;
}

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

HRESULT Kumazuma::GraphicsModuleImpl::LoadPixelShader(wchar_t const* path, ID3D11PixelShader** out)
{
    return E_NOTIMPL;
}

HRESULT Kumazuma::GraphicsModuleImpl::LoadVertexShader(wchar_t const* path, ID3D11VertexShader** out)
{
    return E_NOTIMPL;
}

HRESULT Kumazuma::GraphicsModuleImpl::LoadComputeShader(wchar_t const* path, ID3D11ComputeShader** out)
{
    return E_NOTIMPL;
}

Kumazuma::TextureManager& Kumazuma::GraphicsModuleImpl::GetTextureManager()
{
    return *textureManager_;
}

Kumazuma::Texture2D* Kumazuma::GraphicsModuleImpl::GetSwapChainTexture()
{
    return swapChainTexture_.get();
}

Kumazuma::Texture2D* Kumazuma::GraphicsModuleImpl::GetDefaultDepthBuffer()
{
    return defaultDepthBuffer_.get();;
}
