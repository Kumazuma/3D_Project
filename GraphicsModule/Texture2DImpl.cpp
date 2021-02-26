#include "pch.h"
#include "Texture2DImpl.hpp"
namespace Kumazuma
{
    Texture2DImpl::Texture2DImpl()
    {
    }

    Texture2DImpl::Texture2DImpl(ID3D11Device* device, D3D11_TEXTURE2D_DESC const& desc)
    {
        HRESULT hr{};
        size_ = Size2D<u32>{ desc.Width, desc.Height };

        hr = device->CreateTexture2D(&desc, nullptr, &texture_);
        if (FAILED(hr)) throw hr;
        InitializeViews(device, desc);
    }

    void Texture2DImpl::InitializeViews(ID3D11Device* device, D3D11_TEXTURE2D_DESC const& desc)
    {
        format_ = desc.Format;
        HRESULT hr;
        if (desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
        {
            ComPtr<ID3D11ShaderResourceView> srv;

            D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{ };
            shaderResourceViewDesc.Format = desc.Format;
            if (desc.ArraySize == 1)
            {
                shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
                shaderResourceViewDesc.Texture2D.MipLevels = (desc.MiscFlags & D3D11_RESOURCE_MISC_GENERATE_MIPS) ? -1 : desc.MipLevels;
            }
            else
            {
                shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
                shaderResourceViewDesc.Texture2DArray.MostDetailedMip = 0;
                shaderResourceViewDesc.Texture2DArray.MipLevels = (desc.MiscFlags & D3D11_RESOURCE_MISC_GENERATE_MIPS) ? -1 : desc.MipLevels;
                shaderResourceViewDesc.Texture2DArray.ArraySize = desc.ArraySize;
                shaderResourceViewDesc.Texture2DArray.FirstArraySlice = 0;
            }
            hr = device->CreateShaderResourceView(texture_.Get(), &shaderResourceViewDesc, &srv);
            if (FAILED(hr)) throw hr;

            ComPtr<ID3D11View> view;
            srv.As(&view);
            views_.emplace(__uuidof(ID3D11ShaderResourceView), view);
        }
        if (desc.BindFlags & D3D11_BIND_RENDER_TARGET)
        {
            ComPtr<ID3D11RenderTargetView> rtv;
            D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
            renderTargetViewDesc.Format = desc.Format;
            if (desc.ArraySize == 1)
            {
                renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
                renderTargetViewDesc.Texture2D.MipSlice = 0;
            }
            else
            {
                renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
                renderTargetViewDesc.Texture2DArray.ArraySize = desc.ArraySize;
                renderTargetViewDesc.Texture2DArray.FirstArraySlice = 0;
                renderTargetViewDesc.Texture2DArray.MipSlice = 0;
            }
            hr = device->CreateRenderTargetView(texture_.Get(), &renderTargetViewDesc, &rtv);
            if (FAILED(hr)) throw hr;

            ComPtr<ID3D11View> view;
            rtv.As(&view);
            views_.emplace(__uuidof(ID3D11RenderTargetView), view);
        }
        if (desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
        {
            ComPtr<ID3D11UnorderedAccessView> uav;
            D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
            uavDesc.Format = desc.Format;
            if (desc.ArraySize == 1)
            {
                uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
                uavDesc.Texture2D.MipSlice = 0;
            }
            else
            {
                uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
                uavDesc.Texture2DArray.ArraySize = desc.ArraySize;
                uavDesc.Texture2DArray.FirstArraySlice = 0;
                uavDesc.Texture2DArray.MipSlice = 0;
            }
            hr = device->CreateUnorderedAccessView(texture_.Get(), &uavDesc, &uav);
            if (FAILED(hr)) throw hr;
            ComPtr<ID3D11View> view;
            uav.As(&view);
            views_.emplace(__uuidof(ID3D11UnorderedAccessView), view);
        }
        if (desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
        {
            ComPtr<ID3D11DepthStencilView> depView;
            D3D11_DEPTH_STENCIL_VIEW_DESC desc{};
            desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
            desc.Texture2D.MipSlice = 0;
            desc.Format = desc.Format;
            hr = device->CreateDepthStencilView(texture_.Get(), &desc, &depView);
            if (FAILED(hr)) throw hr;
            ComPtr<ID3D11View> view;
            depView.As(&view);
            views_.emplace(__uuidof(ID3D11DepthStencilView), view);
        }
    }

    HRESULT Texture2DImpl::Delete()
    {
        if (this != nullptr)
        {
            delete this;
        }
        return S_OK;
    }

    HRESULT Texture2DImpl::GetResource(ID3D11Texture2D** out)
    {
        if (out == nullptr)
        {
            return E_POINTER;
        }
        ID3D11Texture2D* tex = this->texture_.Get();
        *out = tex;
        tex->AddRef();
        return S_OK;
    }

    HRESULT Texture2DImpl::GetSize(Size2D<u32>* out)
    {
        if (out == nullptr)
        {
            return E_POINTER;
        }
        *out = size_;
        return S_OK;
    }

    HRESULT Texture2DImpl::GetView(GUID guid, ID3D11View** out)
    {
        if (out == nullptr)
        {
            return E_POINTER;
        }
        auto findIt = views_.find(guid);
        if (findIt == views_.end())
        {
            return E_INVALIDARG;
        }
        ID3D11View* view = findIt->second.Get();
        *out = view;
        view->AddRef();
        return S_OK;
    }

    DXGI_FORMAT Texture2DImpl::GetFormat()
    {
        return DXGI_FORMAT();
    }

    Texture2D* Texture2DImpl::CreateGenerateMipmap(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
    {
        return nullptr;
    }

}
