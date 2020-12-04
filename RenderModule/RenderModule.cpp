#include "pch.h"
#include<wincodec.h>
#include"RenderEntity.h"
#include "RenderModule.h"
#include <vector>
#include "TerrainObject.h"
#include <d3dx9.h>
#include <DxErr.h>
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "DxErr.lib")
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
		m_hwnd = hWindow;
		hr = ::CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_ALL,
			IID_IWICImagingFactory,
			reinterpret_cast<void**>(&m_pImageFactory));
		if (FAILED(hr))
			throw hr;
		
		
		*(&m_pSDK) = Direct3DCreate9(D3D_SDK_VERSION);
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
		d3dPP.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		m_d3dpp = d3dPP;
		hr = m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, (HWND)hWindow, iFlag, &d3dPP,  &m_pDevice);
		if (FAILED(hr))
			throw hr;
		CreateSimpleColorTexture(64, 64, { 0.5f, 0.5f, 0.5f, 1.f }, &m_pDefaultTexture);
		CreateSimpleColorTexture(64, 64, { 1.0f, 0.f, 0.f, 1.f }, &m_pRedTexture);
		CreateSimpleColorTexture(64, 64, { 0.f, 1.0f, 0.f, 1.f }, &m_pGreenTexture);
		CreateSimpleColorTexture(64, 64, { 0.f, 0.f, 1.f, 1.f }, &m_pBlueTexture);
		m_pDevice->GetSwapChain(0, &m_defaultSwapChain);
		
		return S_OK;
	}
	catch (HRESULT hr)
	{
		return hr;
	}
}

auto RenderModule::ClearEntityTable() -> void
{
	for (auto& it : m_renderEntities)
	{
		it.second.clear();
	}
}

auto RenderModule::GetDevice(IDirect3DDevice9** pOut) -> HRESULT
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
	D3DXIMAGE_INFO info;

	D3DXGetImageInfoFromFileW(szFilePath, &info);
	hr = D3DXCreateTextureFromFileExW(
		m_pDevice.Get(),
		szFilePath,
		info.Width,
		info.Height,
		info.MipLevels,
		0,
		info.Format,
		D3DPOOL_MANAGED,
		D3DX_FILTER_LINEAR,
		D3DX_FILTER_LINEAR,
		0,
		&info,
		nullptr,
		&pTexture
	);
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
	HRESULT hr{};
	D3DMATRIX viewTransform{};
	XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&viewTransform), mView);
	hr = m_pDevice->SetTransform(D3DTS_VIEW, &viewTransform);
}
auto RenderModule::SetProj(float angle, float aspect, float nearZ, float farZ)->void
{
	aspect =
		static_cast<float>(GetWidth()) /
		static_cast<float>(GetHeight());

	XMMATRIX mProj{ XMMatrixPerspectiveFovLH(XMConvertToRadians(angle), aspect, nearZ, farZ) };
	HRESULT hr{};

	hr = m_pDevice->SetTransform(D3DTS_PROJECTION, reinterpret_cast<D3DMATRIX*>(&mProj));
	assert(SUCCEEDED(hr));

}
auto RenderModule::SetViewProjMatrix(DirectX::XMFLOAT4X4 const& viewMatrix, DirectX::XMFLOAT4X4 const& projMatrix)->void
{

}

auto RenderModule::CreateSimpleColorTexture(u32 width, u32 height, const DirectX::XMFLOAT4& color, IDirect3DTexture9** pOut) -> HRESULT
{
	HRESULT hr{};
	if (pOut == nullptr)
	{
		return E_POINTER;
	}
	COMPtr<IDirect3DTexture9> pTexture;
	hr = m_pDevice->CreateTexture(width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTexture, NULL);
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
	if (FAILED(hr))
	{
		return hr;
	}
	D3DLOCKED_RECT		LockRect{};
	D3DCOLOR d3dColor = D3DCOLOR_COLORVALUE(color.x, color.y, color.z, color.w);
	pTexture->LockRect(0, &LockRect, NULL, 0);
	{
		u32* pStart = reinterpret_cast<u32*>(LockRect.pBits);
		for (u32 i = 0; i < height; ++i)
		{
			u32* const pEnd = pStart + LockRect.Pitch / sizeof(u32);
			for (u32* it = pStart; it != pEnd; ++it)
			{
				*it = d3dColor;
			}
			pStart = pEnd;
		}
	}
	//*((_ulong*)LockRect.pBits) = D3DCOLOR_COLORVALUE(0.5f, 0.5f, 0.5f, 1.f);
	pTexture->UnlockRect(0);
	*pOut = pTexture.Get();
	pTexture->AddRef();
	return S_OK;
}

auto RenderModule::GetSimpleColorTexture(DefaultColorTexture kind, IDirect3DTexture9** pOut) -> HRESULT
{
	IDirect3DTexture9* pRef{};
	if (pOut == nullptr)
	{
		return E_POINTER;
	}
	switch (kind)
	{
	case DefaultColorTexture::RED:
		pRef = m_pRedTexture.Get();
		break;
	case DefaultColorTexture::GREEN:
		pRef = m_pGreenTexture.Get();
		break;
	case DefaultColorTexture::BLUE:
		pRef = m_pBlueTexture.Get();
		break;
	default:
		return E_FAIL;
	}
	pRef->AddRef();
	*pOut = pRef;
	return S_OK;
}

auto RenderModule::GetFrustum() const -> Frustum const&
{
	return m_frustum;
}

auto RenderModule::PrepareFrustum() -> void
{
	HRESULT hr{};
	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProj;
	hr = m_pDevice->GetTransform(D3DTS_VIEW, reinterpret_cast<D3DMATRIX*>(&mView));
	assert(SUCCEEDED(hr));
	hr = m_pDevice->GetTransform(D3DTS_PROJECTION, reinterpret_cast<D3DMATRIX*>(&mProj));
	assert(SUCCEEDED(hr));

	m_frustum.MakeFrustum(mView, mProj);
}

auto RenderModule::Render(float r, float g, float b, float a, IDirect3DSwapChain9* pSwapChain) -> void
{
	if (!Renderable())
	{
		ClearEntityTable();
		return;
	}
	COMPtr<IDirect3DSurface9> backbuffer;
	pSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	m_pDevice->SetRenderTarget(0, backbuffer.Get());
	BeginRender(r, g, b, a);
	for (auto& it : m_renderEntities[Kind::ENVIRONMENT])
	{
		it->Render(this);
	}
	for (auto& it : m_renderEntities[Kind::NONALPHA])
	{
		it->Render(this);
	}
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	for (auto& it : m_renderEntities[Kind::ALPHA])
	{
		it->Render(this);
	}
	for (auto& it : m_renderEntities[Kind::NAVIMASH])
	{
		it->Render(this);
	}
	for (auto& it : m_renderEntities[Kind::UI])
	{
		it->Render(this);
	}
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	ClearEntityTable();
	EndRender(pSwapChain);
}

auto RenderModule::AddRenderEntity(Kind kind, std::shared_ptr<RenderEntity> const& entity) -> void
{
	m_renderEntities[kind].push_back(entity);
}

auto RenderModule::ConvertProjToWorld(DirectX::XMFLOAT3 const& cameraPos, DirectX::XMFLOAT3 const& cameraRotation, float angle, float aspect, float nearZ, float farZ, DirectX::XMFLOAT3 const& pos) -> DirectX::XMFLOAT3
{
	XMVECTOR vPosition{ XMLoadFloat3(&cameraPos) };
	XMVECTOR vForward{ XMVectorSet(0.f, 0.f, 1.f, 0.f) };
	XMVECTOR vUp{ XMVectorSet(0.f, 1.f, 0.f, 0.f) };
	XMMATRIX mWorld{ XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&cameraRotation)) };
	vForward = XMVector3TransformNormal(vForward, mWorld);
	vUp = XMVector3TransformCoord(vUp, mWorld);

	XMMATRIX mView{ XMMatrixLookAtLH(
		vPosition,
		vPosition + vForward,
		vUp
	) };
	aspect =
		static_cast<float>(GetWidth()) /
		static_cast<float>(GetHeight());
	XMMATRIX mProj{ XMMatrixPerspectiveFovLH(XMConvertToRadians(angle), aspect, nearZ, farZ) };
	XMMATRIX mProjInverse{ XMMatrixInverse(nullptr, mProj) };
	XMMATRIX mViewInverse{ XMMatrixInverse(nullptr, mView) };

	XMVECTOR vPos{ XMLoadFloat3(&pos) };
	vPos = XMVector3TransformCoord(vPos, mProjInverse);
	vPos = XMVector3TransformCoord(vPos, mViewInverse);
	XMFLOAT3 res{};
	XMStoreFloat3(&res, vPos);
	return res;
}


auto RenderModule::BeginRender(float r, float g, float b, float a) -> void
{
	if (!Renderable())
	{
		return;
	}
	HRESULT hr{};
	hr = m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_COLORVALUE(r,g,b,a), 1.f, 0);
	assert(SUCCEEDED(hr));
	hr = m_pDevice->BeginScene();
	assert(SUCCEEDED(hr));
}

auto RenderModule::EndRender(IDirect3DSwapChain9* pSwapChain) -> void
{
	HRESULT hr;
	if (pSwapChain == nullptr)
	{
		pSwapChain = m_defaultSwapChain.Get();
	}
	m_pDevice->EndScene();
	pSwapChain->Present(nullptr, nullptr, nullptr, nullptr, 0);
}

auto RenderModule::Renderable() -> bool
{
	HRESULT hr = m_pDevice->TestCooperativeLevel();
	if (hr == D3DERR_DEVICENOTRESET)
	{
		hr = m_pDevice->Reset(&m_d3dpp);
	}
	return hr == S_OK;
}
