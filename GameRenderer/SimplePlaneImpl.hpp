#pragma once
#include"include/SimplePlane.hpp"
#include<wrl.h>
namespace Kumazuma::Graphics
{
	class SimplePlaneShare
	{
	public:
		SimplePlaneShare(IDirect3DDevice9* device);
        auto Render(IDirect3DDevice9* device)->void;
        Microsoft::WRL::ComPtr<IDirect3DIndexBuffer9> indexBuffer_;
        Microsoft::WRL::ComPtr<IDirect3DVertexBuffer9> vertexBuffer_;
        DWORD fvf_;
	};
	class SimplePlaneImpl : public SimplePlane
	{
	public:
		SimplePlaneImpl(IDirect3DDevice9* device);
		auto Render(IDirect3DDevice9* device)->void;

	private:
		static std::weak_ptr<SimplePlaneShare> s_shareState_;
	private:
		std::shared_ptr<SimplePlaneShare> shareState_;
	};
}