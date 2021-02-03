#pragma once
#include"include/GameRenderer.hpp"
#include<wrl.h>
#include<unordered_map>
#include<map>

#include<list.hpp>
#include<spinlock.hpp>
#include<typedef.hpp>
#include<SkyBoxObject.h>
using namespace Microsoft::WRL;
using namespace DirectX;

namespace Kumazuma::Graphics
{
	
	class GameRendererImpl : public GameRenderer
	{
	public:
		GameRendererImpl(RenderModule* renderModule);
		GameRendererImpl(GameRendererImpl&& renderer) noexcept;
		auto GetEffect(std::wstring const& effectName, ID3DXEffect** out)->HRESULT;
		auto SetSkyBox(IDirect3DBaseTexture9* texture)->void ;
		auto AddRender(RenderGroupID groupID, std::shared_ptr<RenderComponent> const& renderCom)->void;
		auto AddUI(std::shared_ptr<RenderComponent> const& renderCom, u32 zOrder)->void;
		auto AddEffect(std::shared_ptr<Effect> const& effect)->void;
		auto AddPostprocessing(PostProcessing* postprocessing)->void;
		auto SetProjMatrix(DirectX::XMFLOAT4X4 const& mat)->void;
		auto SetViewMatrix(DirectX::XMFLOAT4X4 const& mat)->void;
		auto Render()->void;

		auto GetRenderModuleRef()->RenderModule &;
		auto GetDevice(IDirect3DDevice9** out)->HRESULT;
		auto GetProjMatrix(DirectX::XMFLOAT4X4* pOut)->HRESULT;
		auto GetViewMatrix(DirectX::XMFLOAT4X4* pOut)->HRESULT;
		auto GetFrustum(Frustum* const pOutFrustum)->HRESULT;
		auto SetNearFar(f32 nearZ, f32 farZ)->void;
		auto AddLight(D3DLIGHT9 const& light)->void;

	private:
		auto ClearRenderTargets(RenderModule* renderModule)->void;
		auto DeferredRender(RenderModule* renderModule)->void;
		auto DeferredLighting(RenderModule* pRenderModule) -> void;
		auto DeferredCombine(RenderModule* pRenderModule)->void;
		auto RenderEffect(RenderModule* pRenderModule)->void;
		auto RenderWireframe(RenderModule* pRenderModule)->void;
		auto RenderPostCombine()->void;
		auto RenderUI()->void;
		auto CreateEffectFromRC(IDirect3DDevice9* device, wchar_t const* resourceName, wchar_t const* resourceType, ID3DXEffect** effect)->HRESULT;
	private:
		SpinLock locker_;
		Frustum frustum_;
		RenderModule* renderModule_;
		std::unique_ptr<SkyBoxObject> skybox_;
		std::unordered_map<std::wstring, ComPtr<ID3DXEffect> > effectTable_;
		std::unordered_map <RenderGroupID, LinkedList< std::shared_ptr<RenderComponent> > > renderGroups_;
		std::map < i32, std::list<std::shared_ptr<RenderComponent> > > UIs_;
		std::list<std::shared_ptr<Effect> > effects_;
		std::list<D3DLIGHT9> lights_;
		ComPtr<ID3DXEffect> combineShader_;
		ComPtr<ID3DXEffect> lightingShader_;
		ComPtr<IDirect3DVertexBuffer9> vertexBuffer_;
		ComPtr<IDirect3DIndexBuffer9>  indexBuffer_;
		ComPtr<IDirect3DBaseTexture9> skyboxTexture_;
		ComPtr<ID3DXSprite> sprite_;
		f32x44 projMatrix_;
		f32x44 viewMatrix_;
		f32 minZ_;
		f32 maxZ_;
	};
}