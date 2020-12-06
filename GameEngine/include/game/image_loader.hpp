#pragma once
#include<memory>
#include<map>
#include<kumazuma/comwrapper.hpp>
#include<string>
#include <wincodec.h>
namespace Kumazuma
{
	namespace Game
	{
		template<typename _RendererT, typename _ImageT = typename _RendererT::Image>
		class ImageResourceManager
		{
		public:
			using Image = _ImageT;

			ImageResourceManager();
			ImageResourceManager(const ImageResourceManager&) = delete;
			ImageResourceManager(ImageResourceManager&&)noexcept;
			~ImageResourceManager();
			//컬러키가 설정되면 이미지 파일의 알파값은 무시한다.
			std::shared_ptr<Image> Load(_RendererT& renderer, const std::wstring& name, unsigned colorkey);
			//알파값을 포함하여 이미지를 읽는다.
			std::shared_ptr<Image> Load(_RendererT& renderer, const std::wstring& name);
			//기존의 로드된 이미지를 가져온다.
			std::shared_ptr<Image> Get(const std::wstring& name);
			void Clear();
		private:
			ComWrapper<IWICImagingFactory> m_wicFactory;
			std::map<std::wstring, std::shared_ptr<Image> > m_images;
		};
		template<typename _RendererT, typename _ImageT>
		inline ImageResourceManager< _RendererT, _ImageT>::ImageResourceManager() :
			m_wicFactory{}
		{
			HRESULT hr = 0;
			hr = ::CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_ALL, IID_IWICImagingFactory, (void**)&m_wicFactory);
			assert(hr == S_OK);
		}
		template<typename _RendererT, typename _ImageT>
		inline ImageResourceManager< _RendererT, _ImageT>::ImageResourceManager(ImageResourceManager&& other) noexcept :
			m_images{ std::move(other.m_images) },
			m_wicFactory{ std::move(other.m_wicFactory) }
		{
			other.m_wicFactory = nullptr;
		}
		template<typename _RendererT, typename _ImageT>
		inline ImageResourceManager< _RendererT, _ImageT>::~ImageResourceManager()
		{
			Clear();
			m_wicFactory.~ComWrapper();
		}
		template<typename _RendererT, typename _ImageT>
		inline std::shared_ptr<_ImageT> ImageResourceManager< _RendererT, _ImageT>::Get(const std::wstring& name)
		{
			return m_images[name];
		}
		template<typename _RendererT, typename _ImageT>
		inline std::shared_ptr<_ImageT> ImageResourceManager< _RendererT, _ImageT>::Load(_RendererT& renderer, const std::wstring& name, unsigned colorkey)
		{
			auto node = m_images.find(name);
			if (node == m_images.end())
			{
				HRESULT hr = 0;
				ComWrapper<IWICBitmapDecoder> decoder;
				ComWrapper<IWICBitmapFrameDecode> frameDecoder;
				ComWrapper<IWICFormatConverter> fmtConverter;
				hr = m_wicFactory->CreateDecoderFromFilename(name.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
				assert(hr == S_OK);
				hr = decoder->GetFrame(0, &frameDecoder);
				assert(hr == S_OK);
				hr = m_wicFactory->CreateFormatConverter(&fmtConverter);
				assert(hr == S_OK);
				
				hr = fmtConverter->Initialize(frameDecoder, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeCustom);
				assert(hr == S_OK);
				node = m_images.emplace(name, std::make_shared<Image>(Image::FromWICDecoder(renderer, fmtConverter, colorkey))).first;
			}
			return node->second;
		}
		template<typename _RendererT, typename _ImageT>
		inline std::shared_ptr<_ImageT> ImageResourceManager< _RendererT, _ImageT>::Load(_RendererT& renderer, const std::wstring& name)
		{
			auto node = m_images.find(name);
			if (node == m_images.end())
			{
				HRESULT hr = 0;
				ComWrapper<IWICBitmapDecoder> decoder;
				ComWrapper<IWICBitmapFrameDecode> frameDecoder;
				ComWrapper<IWICFormatConverter> fmtConverter;
				hr = m_wicFactory->CreateDecoderFromFilename(name.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
				assert(hr == S_OK);
				hr = decoder->GetFrame(0, &frameDecoder);
				assert(hr == S_OK);
				hr = m_wicFactory->CreateFormatConverter(&fmtConverter);
				assert(hr == S_OK);
				hr = fmtConverter->Initialize(frameDecoder, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeCustom);
				assert(hr == S_OK);
				node = m_images.emplace(name, std::make_shared<Image>(Image::FromWICDecoder(renderer, fmtConverter))).first;
			}
			return node->second;
		}
		template<typename _RendererT, typename _ImageT>
		inline void ImageResourceManager< _RendererT, _ImageT>::Clear()
		{
			m_images.clear();
		}
	}
}
#pragma comment(lib, "WindowsCodecs.lib")