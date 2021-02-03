#pragma once
#include"RenderModule.h"
#include<d3dx9.h>
#include<DirectXMath.h>
struct RenderEntity;
class Frustum;
class RenderModule;

struct DLL_CLASS __declspec(novtable) IRenderer
{
	virtual auto Render(RenderModule* pRenderModule)->void = 0;
	virtual auto AddEntity(RenderModule::Kind kind, std::shared_ptr<RenderEntity>const& entity)->void = 0;
	virtual auto SetProjMatrix(DirectX::XMFLOAT4X4 const& mat)->void = 0;
	virtual auto SetViewMatrix(DirectX::XMFLOAT4X4 const& mat)->void = 0;
	virtual auto GetProjMatrix(DirectX::XMFLOAT4X4* pOut)->HRESULT = 0;
	virtual auto GetViewMatrix(DirectX::XMFLOAT4X4* pOut)->HRESULT = 0;
	virtual auto GetEffect(ID3DXEffect** pOut)->HRESULT = 0;
	virtual auto GetFrustum(Frustum* const pOutFrustum)->HRESULT = 0;
	virtual auto Delete()->void = 0;
	virtual ~IRenderer() = default;
};

struct DLL_CLASS __declspec(novtable) IRendererBase
{
	virtual ~IRendererBase() = default;
	virtual auto GetRenderModuleRef()->RenderModule& = 0;
	virtual auto GetDevice(IDirect3DDevice9** out)->HRESULT = 0;
	virtual auto SetProjMatrix(DirectX::XMFLOAT4X4 const& mat)->void = 0;
	virtual auto SetViewMatrix(DirectX::XMFLOAT4X4 const& mat)->void = 0;
	virtual auto GetProjMatrix(DirectX::XMFLOAT4X4* pOut)->HRESULT = 0;
	virtual auto GetViewMatrix(DirectX::XMFLOAT4X4* pOut)->HRESULT = 0;
	virtual auto GetFrustum(Frustum* const pOutFrustum)->HRESULT = 0;
};