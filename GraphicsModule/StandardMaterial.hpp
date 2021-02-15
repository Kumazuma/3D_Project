#pragma once
#include "Material.hpp"
#include "Subset.hpp"
namespace Kumazuma
{
	class GraphicsModule;
	class DLL_CLASS StandardMaterial : public Material
	{
		struct Impl;
	public:
		StandardMaterial(GraphicsModule* device, Subset const* subset) ;
		~StandardMaterial();
		void SetWorldMatrixPtr(DirectX::XMFLOAT4X4* ptr);
		void RenderShadowMap();
		void Render(ID3D11DeviceContext* deviceContext) override;
	private:
		Subset const* subset_;
		GraphicsModule* gmodule_;
		DirectX::XMFLOAT4X4* worldMatrixPtr_;
		ID3D11PixelShader* pixelShader_;
		ID3D11SamplerState* samplerState_;
	};
}