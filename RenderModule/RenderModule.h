#pragma once
#include<d3d9.h>
#include"COMPtr.hpp"
#include "Common.h"
#include "RenderObject.h"
#include<string>
#include<wincodec.h>
#include "Frustum.h"
#include <unordered_map>
#include <memory>
#include <list>
#include "RenderTarget.hpp"
struct IWICImagingFactory;
enum class DefaultColorTexture {
	RED,
	GREEN,
	BLUE,
};
class Frustum;
struct RenderEntity;
class DLL_CLASS RenderModule
{
protected:
	RenderModule(RenderModule const&) = delete;
public:
	enum class Kind{NONE, ENVIRONMENT, TERRAIN, NONALPHA, SKINNED,  ALPHA, NAVIMASH, UI};
	static auto Create(HWND hWindow, u32 width, u32 height, bool fullScreen, RenderModule** pOut)->HRESULT;

public:
	
	auto GetDevice(IDirect3DDevice9** pOut)->HRESULT;
	auto CreateRenderTarget(std::wstring const& id, u32 width, u32 height, D3DFORMAT format)->HRESULT;
	auto GetRenderTarget(std::wstring const& id, IRenderTarget** out)->HRESULT;
	auto GetDefaultRenderTarget(IRenderTarget** out)->HRESULT;
	auto CreateTerrain(wchar_t const* szHeightMapPath, size_t len, f32 interval, f32 maxHeight, RenderObject** pOut)->HRESULT;
	auto CreateTexture(wchar_t const* szFilePath, IDirect3DTexture9** pOut)->HRESULT;
	auto CreateCubeTexture(wchar_t const* szFilePath, IDirect3DCubeTexture9** pOut)->HRESULT;
	auto GetDefaultTexture(IDirect3DTexture9** pTexture)->HRESULT;
	auto GetWidth()const->u32 { return m_width; }
	auto GetHeight()const->u32 { return m_height; }

	/*
	auto SetCamera(DirectX::XMFLOAT3 const& cameraPosition, DirectX::XMFLOAT3 const& at, DirectX::XMFLOAT3 const& up)->void;
	auto SetCamera(DirectX::XMFLOAT3 const* pCameraPos, DirectX::XMFLOAT3 const* pRotation)->void;
	auto SetProj(float angle, float aspect, float nearZ, float farZ)->void;
	auto SetViewProjMatrix(DirectX::XMFLOAT4X4 const& viewMatrix, DirectX::XMFLOAT4X4 const& projMatrix)->void;
	*/
	auto CreateSimpleColorTexture(u32 width, u32 height, const DirectX::XMFLOAT4& color, IDirect3DTexture9** pOut)->HRESULT;
	auto GetSimpleColorTexture(DefaultColorTexture kind, IDirect3DTexture9** pOut)->HRESULT;
	/*
	auto GetFrustum()const->Frustum const&;
	auto PrepareFrustum()->void;
	auto Render(float r, float g, float b, float a, HWND hWnd)->void;
	auto AddRenderEntity(Kind kind, std::shared_ptr<RenderEntity>const& entity)->void;
	*/
	auto ConvertProjToWorld(
		DirectX::XMFLOAT3 const& cameraPos,
		DirectX::XMFLOAT3 const& cameraRotation,
		float angle,
		float aspect,
		float nearZ,
		float farZ,
		DirectX::XMFLOAT3 const& pos
		)->DirectX::XMFLOAT3;
	auto Reset()->bool;
	
	auto GetDefaultSwapChain(IDirect3DSwapChain9** ppSwapChain)->void;
	static auto GenerateViewMatrix(DirectX::XMFLOAT3 const& cameraPosition, DirectX::XMFLOAT3 const& at, DirectX::XMFLOAT3 const& up, DirectX::XMFLOAT4X4* pOut)->void;
	static auto GenerateViewMatrix(DirectX::XMFLOAT3 const& cameraPosition, DirectX::XMFLOAT3 const& rotation, DirectX::XMFLOAT4X4* pOut)->void;
	static auto GenerateProjPerspective(f32 angle, f32 aspect, f32 nearZ, f32 farZ, DirectX::XMFLOAT4X4* pOut)->void;
	static auto GenerateProjOtho(f32 width, f32 height, f32 nearZ, f32 farZ, DirectX::XMFLOAT4X4* pOut)->void;
protected:
	RenderModule();
	auto BeginRender(float r, float g, float b, float a)->void;
	auto EndRender(HWND hWnd)->void;
	auto Initialize(HWND hWindow, u32 width, u32 height, bool fullScreen)->HRESULT;
	auto ClearEntityTable()->void;
private:
	std::unordered_map < Kind, std::list<std::shared_ptr<RenderEntity> > > m_renderEntities;


	std::unordered_map<std::wstring, D3DSURFACE_DESC> surfaceDescTable;
	D3DPRESENT_PARAMETERS m_d3dpp;
	COMPtr<IDirect3DDevice9> m_pDevice;
	COMPtr<IDirect3D9> m_pSDK;
	COMPtr<IDirect3DTexture9> m_pDefaultTexture;
	COMPtr<IDirect3DTexture9> m_pRedTexture;
	COMPtr<IDirect3DTexture9> m_pGreenTexture;
	COMPtr<IDirect3DTexture9> m_pBlueTexture;
	COMPtr<IWICImagingFactory> m_pImageFactory;
	COMPtr<IDirect3DSwapChain9> m_defaultSwapChain;
	COMPtr<IRenderTarget> m_defaultRenderTarget;
	std::unordered_map<std::wstring, COMPtr<IRenderTarget> > m_renderTargets;
	Frustum m_frustum;
	u32 m_width;
	u32 m_height;
	HWND m_hwnd;
};