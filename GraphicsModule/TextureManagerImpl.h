#pragma once
#include "TextureManager.hpp"
#include <unordered_map>
#include <wrl.h>
#include <memory>
#include <wincodec.h>
#include <mutex>
using namespace Microsoft::WRL;
namespace Kumazuma
{
	class GraphicsModuleImpl;
	class TextureManagerImpl : public TextureManager
	{
	public:
		TextureManagerImpl(GraphicsModuleImpl* device);
		virtual Texture2D* Load(wchar_t const* path) override;
		void SetGraphicsModule(GraphicsModuleImpl* module_);
		void GC();
	private:
		std::mutex mutex_;
		GraphicsModuleImpl* module_;
		ComPtr<IWICImagingFactory> factory_;
		std::unordered_map<std::wstring, ComPtr<ID3D11Texture2D> > table_;
	};
}