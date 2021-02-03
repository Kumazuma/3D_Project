#include "pch.h"
#include "SimplePlaneImpl.hpp"
#include <SpinLock.hpp>
#include <mutex>
static SpinLock g_locker;
struct Vertex
{
	f32x3 position;
	f32x2 texcoord;
};
std::weak_ptr<Kumazuma::Graphics::SimplePlaneShare> Kumazuma::Graphics::SimplePlaneImpl::s_shareState_;

Kumazuma::Graphics::SimplePlaneShare::SimplePlaneShare(IDirect3DDevice9* device)
{
	fvf_ = D3DFVF_XYZ | D3DFVF_TEX1;

	HRESULT hr{};
	hr = device->CreateVertexBuffer(
		sizeof(Vertex) * 4,
		D3DUSAGE_WRITEONLY,
		D3DFVF_XYZ | D3DFVF_TEX1,
		D3DPOOL_MANAGED,
		&vertexBuffer_,
		nullptr);
	if (FAILED(hr))
	{
		throw std::exception("vertex buffer error");
	}
	hr = device->CreateIndexBuffer(
		sizeof(u16) * 6,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&indexBuffer_,
		nullptr
	);
	if (FAILED(hr))
	{
		throw std::exception("index buffer error");
	}
	Vertex* vertexMappPtr{};
	u16* indexMappedPtr{};
	vertexBuffer_->Lock(0, 0, reinterpret_cast<void**>(&vertexMappPtr), 0);
	indexBuffer_->Lock(0, 0, reinterpret_cast<void**>(&indexMappedPtr), 0);
	vertexMappPtr[0].position = f32x3{ 0.5f, 0.5f, 0.f };
	vertexMappPtr[1].position = f32x3{ 0.5f, -0.5f, 0.f };
	vertexMappPtr[2].position = f32x3{ -0.5f, 0.5f, 0.f };
	vertexMappPtr[3].position = f32x3{ -0.5f, -0.5f, 0.f };
	vertexMappPtr[0].texcoord = f32x2{ 1.0f, 0.0f };
	vertexMappPtr[1].texcoord = f32x2{ 1.0f, 1.0f };
	vertexMappPtr[2].texcoord = f32x2{ 0.0f, 0.0f };
	vertexMappPtr[3].texcoord = f32x2{ 0.0f, 1.0f };
	indexMappedPtr[0] = 0;
	indexMappedPtr[1] = 1;
	indexMappedPtr[2] = 3;
	indexMappedPtr[3] = 0;
	indexMappedPtr[4] = 3;
	indexMappedPtr[5] = 2;
	indexBuffer_->Unlock();
	vertexBuffer_->Unlock();
}

auto Kumazuma::Graphics::SimplePlaneShare::Render(IDirect3DDevice9* device) -> void
{
	device->SetFVF(fvf_);
	device->SetFVF(fvf_);
	device->SetStreamSource(0, vertexBuffer_.Get(), 0, sizeof(Vertex));
	device->SetIndices(indexBuffer_.Get());
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

Kumazuma::Graphics::SimplePlaneImpl::SimplePlaneImpl(IDirect3DDevice9* device)
{
	shareState_ = s_shareState_.lock();
	if (shareState_ == nullptr)
	{
		std::lock_guard<decltype(g_locker)> guard{ g_locker };
		shareState_ = s_shareState_.lock();
		if (shareState_ == nullptr)
		{
			shareState_ = std::make_shared<SimplePlaneShare>(device);
			s_shareState_ = shareState_;
		}
	}
}

auto Kumazuma::Graphics::SimplePlaneImpl::Render(IDirect3DDevice9* device) -> void
{
	shareState_->Render(device);
}
