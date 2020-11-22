#include "pch.h"
#include<wincodec.h>
#include "RenderModule.h"
#include <vector>
#include "TerrainObject.h"
#include <d3dx9.h>
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "d3dx9.lib")
auto RenderModule::Create(HWND hWindow, u32 width, u32 height, RenderModule** pOut) -> HRESULT
{
	RenderModule* pObj{ new RenderModule{} };
	HRESULT hr{ pObj->Initialize(hWindow, width, height) };
	if (FAILED(hr))
	{
		delete pObj;
		pObj = nullptr;
	}
	*pOut = pObj;
	return hr;
}

RenderModule::RenderModule()
{
}

auto RenderModule::Initialize(HWND hWindow, u32 width, u32 height) -> HRESULT
{
	HRESULT hr{E_FAIL};
	try
	{
		m_width = width;
		m_height = height;
		hr = ::CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_ALL,
			IID_IWICImagingFactory,
			reinterpret_cast<void**>(&m_pImageFactory));
		if (FAILED(hr))
			throw hr;
		
		
		hr = Direct3DCreate9Ex(D3D_SDK_VERSION, &m_pSDK);
		if (FAILED(hr))
			throw hr;

		D3DCAPS9 DeviceCaps{};
		DWORD iFlag{ D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED };
		D3DPRESENT_PARAMETERS d3dPP{};
		hr = m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps);
		if (FAILED(hr))
			throw hr;

		d3dPP.BackBufferWidth = width;
		d3dPP.BackBufferHeight = height;
		d3dPP.BackBufferFormat = D3DFMT_A8R8G8B8;
		d3dPP.BackBufferCount = 1;

		d3dPP.MultiSampleType = D3DMULTISAMPLE_NONE;
		d3dPP.MultiSampleQuality = 0;

		d3dPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dPP.hDeviceWindow = (HWND)hWindow;
		d3dPP.Windowed = true;

		d3dPP.EnableAutoDepthStencil = true;
		d3dPP.AutoDepthStencilFormat = D3DFMT_D24S8;
		d3dPP.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		d3dPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		
		hr = m_pSDK->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, (HWND)hWindow, iFlag, &d3dPP, nullptr, &m_pDevice);
		if (FAILED(hr))
			throw hr;
		hr = m_pDevice->CreateTexture(64, 64, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pDefaultTexture, NULL);
		switch (hr)
		{
		case D3DERR_INVALIDCALL:
			"D3DERR_INVALIDCALL"; __debugbreak();
			break;
		case D3DERR_OUTOFVIDEOMEMORY:
			"D3DERR_OUTOFVIDEOMEMORY"; __debugbreak();

			break;
		case E_OUTOFMEMORY:
			"E_OUTOFMEMORY"; __debugbreak();
			break;
		}

		D3DLOCKED_RECT		LockRect{};
		
		m_pDefaultTexture->LockRect(0, &LockRect, NULL, 0);
		{
			u32* pStart = reinterpret_cast<u32*>(LockRect.pBits);
			for (int i = 0; i < 64; ++i)
			{
				u32 * const pEnd = pStart + LockRect.Pitch/sizeof(u32);
				for (u32* it = pStart; it != pEnd; ++it)
				{
					*it = D3DCOLOR_COLORVALUE(0.5f, 0.5f, 0.5f, 1.0f);
				}
				pStart = pEnd;
			}
		}
		//*((_ulong*)LockRect.pBits) = D3DCOLOR_COLORVALUE(0.5f, 0.5f, 0.5f, 1.f);
		m_pDefaultTexture->UnlockRect(0);
		return S_OK;
	}
	catch (HRESULT hr)
	{
		return hr;
	}
}

auto RenderModule::GetDevice(IDirect3DDevice9Ex** pOut) -> HRESULT
{
	try
	{
		if (pOut == nullptr)
			throw E_POINTER;
		*pOut = m_pDevice.Get();
		m_pDevice->AddRef();
		return S_OK;
	}
	catch (HRESULT hr)
	{
		return hr;
	}
}

auto RenderModule::CreateTerrain(wchar_t const* szHeightMapPath, size_t len,  f32 interval, f32 maxHeight, RenderObject** pOut) -> HRESULT
{
	HRESULT hr{};
	try
	{
		if (pOut == nullptr)
			throw E_POINTER;
		COMPtr<IWICBitmapDecoder> pDecoder;
		COMPtr<IWICBitmapFrameDecode> pFrameDecoder;
		COMPtr<IWICFormatConverter> pFmtConverter;
		hr = m_pImageFactory->CreateDecoderFromFilename(szHeightMapPath, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder);
		if (FAILED(hr))
			throw hr;
		hr = pDecoder->GetFrame(0, &pFrameDecoder);
		if (FAILED(hr))
			throw hr;
		hr = m_pImageFactory->CreateFormatConverter(&pFmtConverter);
		if (FAILED(hr))
			throw hr;
		
		hr = pFmtConverter->Initialize(
			pFrameDecoder.Get(),
			GUID_WICPixelFormat32bppRGBA,
			WICBitmapDitherTypeNone,
			nullptr,
			0.f,
			WICBitmapPaletteTypeCustom
		);
		if (FAILED(hr))
			throw hr;
		UINT width{};
		UINT height{};
		pFmtConverter->GetSize(&width, &height);
		std::vector<u8> imageData(static_cast<size_t>(width) * static_cast<size_t>(height) * 4, 0);
		hr = pFmtConverter->CopyPixels(
			nullptr,
			width * 4,
			static_cast<UINT>(imageData.size()),
			imageData.data());
		if (FAILED(hr))
			throw hr;
		TerrainObject* pObj{};
		TerrainObject::Create(
			this,
			static_cast<u32>(width),
			static_cast<u32>(height),
			interval,
			maxHeight,
			reinterpret_cast<u8 const*>(imageData.data()),
			&pObj);
		if (pObj != nullptr)
		{
			*pOut = pObj;
		}
		return S_OK;
	}
	catch (HRESULT hr)
	{
		return hr;
	}
}

auto RenderModule::CreateTexture(wchar_t const* szFilePath, IDirect3DTexture9** pOut) -> HRESULT
{
	if (pOut == nullptr)
	{
		return E_POINTER;
	}
	IDirect3DTexture9* pTexture;
	HRESULT hr{};
	hr = D3DXCreateTextureFromFileW(m_pDevice.Get(), szFilePath, &pTexture);
	if (SUCCEEDED(hr))
	{
		*pOut = pTexture;
	}
	return hr;
}

auto RenderModule::CreateCubeTexture(wchar_t const* szFilePath, IDirect3DCubeTexture9** pOut) -> HRESULT
{
	if (pOut == nullptr)
	{
		return E_POINTER;
	}
	IDirect3DCubeTexture9* pTexture;
	HRESULT hr{};
	hr = D3DXCreateCubeTextureFromFileW(m_pDevice.Get(), szFilePath, &pTexture);
	if (SUCCEEDED(hr))
	{
		*pOut = pTexture;
	}
	return hr;
}

auto RenderModule::GetDefaultTexture(IDirect3DTexture9** pTexture) -> HRESULT
{
	try
	{
		if (pTexture == nullptr)
			throw E_POINTER;
		*pTexture = m_pDefaultTexture.Get();
		m_pDefaultTexture->AddRef();
	}
	catch (HRESULT hr)
	{
		return hr;
	}
	return S_OK;
}
using namespace DirectX;
auto RenderModule::SetCamera(DirectX::XMFLOAT3 const& cameraPosition, DirectX::XMFLOAT3 const& at, DirectX::XMFLOAT3 const& up)->void
{
	XMMATRIX mView{ XMMatrixLookAtLH(
		XMLoadFloat3(&cameraPosition),
		XMLoadFloat3(&at),
		XMLoadFloat3(&up)
	) };
	m_pDevice->SetTransform(D3DTS_VIEW, reinterpret_cast<D3DMATRIX*>(&mView));
}
auto RenderModule::SetCamera(DirectX::XMFLOAT3 const* pCameraPos, DirectX::XMFLOAT3 const* pRotation) -> void
{
	XMVECTOR vPosition{ XMLoadFloat3(pCameraPos) };
	XMVECTOR vForward{XMVectorSet(0.f, 0.f, 1.f, 0.f)};
	XMVECTOR vUp{ XMVectorSet(0.f, 1.f, 0.f, 0.f) };
	XMMATRIX mWorld{ XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(pRotation)) };
	vForward = XMVector3TransformNormal(vForward, mWorld);
	vUp = XMVector3TransformCoord(vUp, mWorld);

	XMMATRIX mView{ XMMatrixLookAtLH(
		vPosition,
		vPosition + vForward,
		vUp
	) };
	m_pDevice->SetTransform(D3DTS_VIEW, reinterpret_cast<D3DMATRIX*>(&mView));
}
auto RenderModule::SetProj(float angle, float aspect, float nearZ, float farZ)->void
{
	aspect =
		static_cast<float>(GetWidth()) /
		static_cast<float>(GetHeight());

	XMMATRIX mProj{ XMMatrixPerspectiveFovLH(XMConvertToRadians(angle), aspect, nearZ, farZ) };

	m_pDevice->SetTransform(D3DTS_PROJECTION, reinterpret_cast<D3DMATRIX*>(&mProj));

}
auto RenderModule::SetViewProjMatrix(DirectX::XMFLOAT4X4 const& viewMatrix, DirectX::XMFLOAT4X4 const& projMatrix)->void
{

}