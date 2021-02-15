#pragma once
#include<array>
#include<memory>
#include<wrl.h>
#include"RenderSystem.hpp"
#include"Material.hpp"
#include"Texture2D.hpp"
#include"list.hpp"
using namespace Microsoft::WRL;
namespace Kumazuma
{
	template<typename T>
	unsigned constexpr NumOf()
	{
		return static_cast<unsigned>(T::END);
	}
	template<typename T>
	unsigned constexpr NumOf(T val)
	{
		return static_cast<unsigned>(val);
	}
	class RenderSystemImpl : public RenderSystem
	{
	public:
		RenderSystemImpl(GraphicsModule* gmodule);
		virtual void AddMaterial(Material* material) override;
		virtual void RemoveMaterial(Material* material) override;
		virtual void Render() override;

	private:
		void InitializeRenderState();
		void RenderShadowMap();
		void RenderDeferred();
		void RenderForward();
		void RenderUI();
	private:
		GraphicsModule* gmodule_;
		std::array< LinkedList<Material*, 32>, NumOf<MaterialShadingClass>() > materials_;
		std::unique_ptr<Texture2D> albedoGBuffer_;
		std::unique_ptr<Texture2D> normalGBuffer_;
		std::unique_ptr<Texture2D> depthGBuffer_;
		std::unique_ptr<Texture2D> sharpnessGBuffer_;
		//for deferred state
		ComPtr<ID3D11DepthStencilState> depthState_;
		ComPtr<ID3D11BlendState>		blendState_;
		ComPtr<ID3D11RasterizerState>	rasterizerCullBackState_;
		ComPtr<ID3D11RasterizerState>	rasterizerCullNoneState_;
	};
}