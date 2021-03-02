#include "pch.h"
#include "TextureManagerImpl.h"
#include <filesystem>
#include "CachedTextureImpl.hpp"
#include "GraphicsModuleImpl.hpp"
namespace Kumazuma
{
	TextureManagerImpl::TextureManagerImpl(GraphicsModuleImpl* device)
	{
		HRESULT hr{};
		module_ = device;
		hr = ::CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_ALL, IID_IWICImagingFactory,(void**) &factory_);
	}
	Texture2D* Kumazuma::TextureManagerImpl::Load(wchar_t const* path)
	{

		HRESULT hr{};
		ComPtr<IWICBitmapDecoder> decoder;
		ComPtr<IWICBitmapFrameDecode> frameDecoder;
		ComPtr<IWICFormatConverter> fmtConverter;
		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DeviceContext> deviceContext;
		ComPtr<ID3D11Texture2D> textureDX11;
		module_->GetDevice(&device);
		module_->GetImmediateContext(&deviceContext);
		std::filesystem::path fullPath{ std::filesystem::absolute(path) };
		std::wstring fullPathString{ fullPath.generic_wstring() };
		{
			
			auto findIt = loadingPaths.find(fullPathString);
			if (findIt != loadingPaths.end())
			{
				auto it = table_.find(fullPathString);
				while (it == table_.end())
				{
					auto findIt = loadingPaths.find(fullPathString);
					if (findIt == loadingPaths.end())
					{
						return nullptr;
					}
				}
				return new CachedTexture2DImpl{ device.Get(), it->second,fullPathString };
			}
			std::lock_guard<std::mutex> guard{ mutex_ };
			loadingPaths.emplace(path);
		}

		hr = factory_->CreateDecoderFromFilename(fullPathString.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
		if (FAILED(hr))
		{
			std::lock_guard<std::mutex> guard{ mutex_ };
			loadingPaths.erase(path);
			return nullptr;
		}
		assert(hr == S_OK);
		hr = decoder->GetFrame(0, &frameDecoder);
		assert(hr == S_OK);
		hr = factory_->CreateFormatConverter(&fmtConverter);
		assert(hr == S_OK);
		hr = fmtConverter->Initialize(frameDecoder.Get(), GUID_WICPixelFormat32bppPRGBA, WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeCustom);
		UINT width{};
		UINT height{};
		fmtConverter->GetSize(&width, &height);
		std::vector<uint8_t> imageData(static_cast<size_t>(width) * static_cast<size_t>(height) * 4, 0);
		hr = fmtConverter->CopyPixels(nullptr, width * 4, static_cast<UINT>(imageData.size()), imageData.data());
		CD3D11_TEXTURE2D_DESC texture2DDesc =
			CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R8G8B8A8_UNORM, width, height, 1, 0, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET |D3D11_BIND_UNORDERED_ACCESS );
		texture2DDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		device->CreateTexture2D(&texture2DDesc, nullptr, &textureDX11);

		std::lock_guard<GraphicsModule> graphicsLock{ *module_ };
		deviceContext->UpdateSubresource(textureDX11.Get(), 0, nullptr, imageData.data(), static_cast<UINT>(imageData.size() / height), 0);
		CachedTexture2DImpl texture{ device.Get(), textureDX11,fullPathString };
		ComPtr<ID3D11ShaderResourceView> srv;
		((Texture2D&)texture).GetView<ID3D11ShaderResourceView>(&srv);
		deviceContext->GenerateMips(srv.Get());
		
		//예약된 자리를 채운다.
		std::lock_guard<std::mutex> guard{ mutex_ };
		table_[path]= textureDX11;
		return new CachedTexture2DImpl{ texture };
	}

	void TextureManagerImpl::SetGraphicsModule(GraphicsModuleImpl* module)
	{
		module_ = module;
	}

	void Kumazuma::TextureManagerImpl::GC()
	{
	}
}

