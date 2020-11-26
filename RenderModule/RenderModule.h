#pragma once
#include<d3d9.h>
#include"COMPtr.hpp"
#include "Common.h"
#include "RenderObject.h"
#include<string>
#include<wincodec.h>
#include "Frustum.h"
struct IWICImagingFactory;
enum class DefaultColorTexture {
	RED,
	GREEN,
	BLUE,
};
class Frustum;
class DLL_CLASS RenderModule
{
public:
	static auto Create(HWND hWindow, u32 width, u32 height, RenderModule** pOut)->HRESULT;
public:
	auto GetDevice(IDirect3DDevice9** pOut)->HRESULT;
	auto CreateTerrain(wchar_t const* szHeightMapPath, size_t len, f32 interval, f32 maxHeight, RenderObject** pOut)->HRESULT;
	auto CreateTexture(wchar_t const* szFilePath, IDirect3DTexture9** pOut)->HRESULT;
	auto CreateCubeTexture(wchar_t const* szFilePath, IDirect3DCubeTexture9** pOut)->HRESULT;
	auto GetDefaultTexture(IDirect3DTexture9** pTexture)->HRESULT;
	auto GetWidth()const->u32 { return m_width; }
	auto GetHeight()const->u32 { return m_height; }
	auto SetCamera(DirectX::XMFLOAT3 const& cameraPosition, DirectX::XMFLOAT3 const& at, DirectX::XMFLOAT3 const& up)->void;
	auto SetCamera(DirectX::XMFLOAT3 const* pCameraPos, DirectX::XMFLOAT3 const* pRotation)->void;
	auto SetProj(float angle, float aspect, float nearZ, float farZ)->void;
	auto SetViewProjMatrix(DirectX::XMFLOAT4X4 const& viewMatrix, DirectX::XMFLOAT4X4 const& projMatrix)->void;
	auto CreateSimpleColorTexture(u32 width, u32 height, const DirectX::XMFLOAT4& color, IDirect3DTexture9** pOut)->HRESULT;
	auto GetSimpleColorTexture(DefaultColorTexture kind, IDirect3DTexture9** pOut)->HRESULT;
	auto GetFrustum()const->Frustum const&;
	auto BeginRender(float r, float g, float b, float a)->void;
	auto EndRender()->void;
	auto Present(HWND hWnd = nullptr)->void;
protected:
	RenderModule();
	auto Initialize(HWND hWindow, u32 width, u32 height)->HRESULT;

private:
	COMPtr<IDirect3DDevice9> m_pDevice;
	COMPtr<IDirect3D9> m_pSDK;
	COMPtr<IDirect3DTexture9> m_pDefaultTexture;
	COMPtr<IDirect3DTexture9> m_pRedTexture;
	COMPtr<IDirect3DTexture9> m_pGreenTexture;
	COMPtr<IDirect3DTexture9> m_pBlueTexture;
	COMPtr<IWICImagingFactory> m_pImageFactory;
	Frustum m_frustum;
	u32 m_width;
	u32 m_height;
};