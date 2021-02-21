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
		RenderSystemImpl(GraphicsModule* gModule, SwapChain* swapChain);
		static void Initialize(GraphicsModule* gModule);
		virtual void AddMaterial(Material* material) override;
		virtual void RemoveMaterial(Material* material) override;
		virtual void Render(GraphicsModule* gmodule, DirectX::XMFLOAT4X4 const* view, DirectX::XMFLOAT4X4 const* proj) override;
		virtual void SettupVertexShader(MeshType type, ID3D11DeviceContext* context, DirectX::XMFLOAT4X4* worldSpace) override;
		void SetGraphicsModule(GraphicsModule* module);
	private:
		void InitializeRenderState(ID3D11Device* device);
		void RenderShadowMap(ID3D11Device* device, ID3D11DeviceContext* context);
		void RenderDeferred(ID3D11Device* device, ID3D11DeviceContext* context);
		void DeferredLighting(ID3D11Device* device, ID3D11DeviceContext* context);
		void DeferredCombine(ID3D11Device* device, ID3D11DeviceContext* context);
		void RenderForward(ID3D11Device* device, ID3D11DeviceContext* context);
		void RenderUI(ID3D11Device* device, ID3D11DeviceContext* context);
	private:
		SwapChain* swapChain_;
		std::array< LinkedList<Material*, 32>, NumOf<MaterialShadingClass>() > materials_;
		std::unique_ptr<Texture2D> albedoGBuffer_;
		std::unique_ptr<Texture2D> normalGBuffer_;
		std::unique_ptr<Texture2D> depthGBuffer_;
		std::unique_ptr<Texture2D> sharpnessGBuffer_;
		//
		std::unique_ptr<Texture2D> lightSpecularMap_;
		std::unique_ptr<Texture2D> lightAmbientMap_;

		//
		DirectX::XMFLOAT4X4 viewSpaceMatrix_;
		DirectX::XMFLOAT4X4 projSpaceMatrix_;
		DirectX::XMFLOAT4X4 viewProjMatrix_;
		//for deferred state
		ComPtr<ID3D11DepthStencilState> depthState_;
		ComPtr<ID3D11BlendState>		blendState_;
		ComPtr<ID3D11RasterizerState>	rasterizerCullBackState_;
		ComPtr<ID3D11RasterizerState>	rasterizerCullNoneState_;
		//
		ComPtr<ID3D11InputLayout>		staticMeshInputLayout_;
		ComPtr<ID3D11VertexShader>		staticMeshVertexShader_;
		//
		ComPtr<ID3D11Buffer>			vsGlobalCBuffer_;
		ComPtr<ID3D11Buffer>			vsLocalCBuffer_;
		//
		ComPtr<ID3D11ComputeShader>		directinalLightingCShader_;
		ComPtr<ID3D11Buffer>			csLightCBuffer_;
		ComPtr<ID3D11Buffer>			csGlobalCBuffer_;
		ComPtr<ID3D11ComputeShader>		combineCShader_;

	};
}