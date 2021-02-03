#pragma once
#include<common.hpp>
#include<d3dx9.h>
#include<string>
#include<TextureRenderTarget.h>
#include<memory>
#include<Renderer.h>
namespace Kumazuma::Graphics
{
	class RenderComponent;
	class GameRenderer;
	class RenderContext;
	class Effect;
	enum class RenderGroupID { 
		None,
		Environment,
		Deferred,
		Alpha,
		Wireframe,
		PostCombine,
		UI };
	struct DLL_CLASS _declspec(novtable) PostProcessing
	{
		virtual auto OnPostProcessing(RenderModule* renderModule, GameRenderer* out)->void = 0;
	};
	class DLL_CLASS GameRenderer : public IRendererBase
	{
	protected:
		GameRenderer() = default;
	public:
		static wchar_t const* const RENDER_TARGET_ALBEDO;
		static wchar_t const* const RENDER_TARGET_NORMAL;
		static wchar_t const* const RENDER_TARGET_DEPTH;
		static wchar_t const* const RENDER_TARGET_SPECULAR;

		virtual ~GameRenderer() = default;
	public:
		static auto Create(RenderModule* renderModule, GameRenderer** out)->HRESULT;
	public:
		virtual auto GetEffect(std::wstring const& effectName, ID3DXEffect** out)->HRESULT = 0;
		virtual auto SetSkyBox(IDirect3DBaseTexture9* texture)->void = 0;
		virtual auto AddRender(RenderGroupID groupID, std::shared_ptr<RenderComponent> const& renderCom)->void = 0;
		virtual auto AddUI(std::shared_ptr<RenderComponent> const& renderCom, u32 zOrder)->void = 0;
		virtual auto AddEffect(std::shared_ptr<Effect> const& effect)->void = 0;
		virtual auto AddPostprocessing(PostProcessing* postprocessing)->void = 0;
		virtual auto SetProjMatrix(DirectX::XMFLOAT4X4 const& mat)->void = 0;
		virtual auto SetViewMatrix(DirectX::XMFLOAT4X4 const& mat)->void = 0;
		virtual auto Render()->void = 0;
		virtual auto SetNearFar(f32 nearZ, f32 farZ)->void = 0;
		virtual auto AddLight(D3DLIGHT9 const& light)->void = 0;
	};
}