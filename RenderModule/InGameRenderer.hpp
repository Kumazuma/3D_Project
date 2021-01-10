#pragma once
#include "typedef.hpp"
#include "Common.h"
#include "Renderer.h"
#include "list.hpp"
#include "Frustum.h"
#include <StringLiteral.h>
#include <map>
class TextureRenderTarget;
class DLL_CLASS InGameRenderer : public IRenderer
{
	InGameRenderer(RenderModule* pRenderModule, u32 width, u32 height);
	InGameRenderer(InGameRenderer&& rhs)noexcept;
public:
	using Kind = RenderModule::Kind;
	static auto Create(RenderModule* pRenderModule, u32 width, u32 height, InGameRenderer** pOut)->HRESULT;
	auto Render(RenderModule* pRenderModule)->void override;
	auto AddEntity(RenderModule::Kind kind, std::shared_ptr<RenderEntity>const& entity)->void override;
	auto SetProjMatrix(DirectX::XMFLOAT4X4 const& mat)->void override;
	auto SetNearFar(f32 nearZ, f32 farZ)->void ;
	auto SetViewMatrix(DirectX::XMFLOAT4X4 const& mat)->void override;
	auto GetEffect(ID3DXEffect** pOut)->HRESULT override;
	auto GetProjMatrix(DirectX::XMFLOAT4X4* pOut)->HRESULT override;
	auto GetViewMatrix(DirectX::XMFLOAT4X4* pOut)->HRESULT override;
	auto GetFrustum(Frustum* const pOutFrustum)->HRESULT override;
	auto Delete()->void override;
	auto AddLight(std::wstring const& name, D3DLIGHT9 const& light)->void;
protected:
	auto DefferedRender(RenderModule* pRenderModule)->void;
	auto Lighting(RenderModule* pRenderModule)->void;
	auto Combine(RenderModule* pRenderModule)->void;
	auto ClearEntityTable()->void;
private:
	Frustum m_frustum;
	f32 m_nearZ;
	f32 m_farZ;
	COMPtr<ID3DXEffect> m_effect;
	COMPtr<ID3DXEffect> m_lightingEffect;
	COMPtr<ID3DXEffect> m_combineEffect;

	COMPtr<ID3DXSprite> m_sprite;

	std::unordered_map<std::wstring, D3DLIGHT9> m_lights;
	std::unordered_map<StringLiteral<char>, std::unique_ptr<TextureRenderTarget> > m_renderTargets;
	std::unordered_map < RenderModule::Kind, std::list<std::shared_ptr<RenderEntity> > > m_renderEntities;
	DirectX::XMFLOAT4X4 m_projMatrix;
	DirectX::XMFLOAT4X4 m_viewMatrix;
	DirectX::XMFLOAT4X4 m_viewProjMatrix;

	COMPtr<IDirect3DVertexBuffer9> m_pVertexBuffer;
	COMPtr<IDirect3DIndexBuffer9>  m_pIndexBuffer;
	static constexpr DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;
};