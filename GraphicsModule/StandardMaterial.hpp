#pragma once
#include "Material.hpp"
#include "MeshCollection.hpp"

namespace Kumazuma
{
	class GraphicsModule;
	
	class DLL_CLASS StandardMaterial : public Material
	{
		struct Impl;
	public:
		StandardMaterial(GraphicsModule* device, Mesh const* mesh_) ;
		~StandardMaterial();
		void SetWorldMatrixPtr(DirectX::XMFLOAT4X4* ptr);
		void RenderShadowMap();
		void Render(RenderSystem* renderSystem, ID3D11DeviceContext* deviceContext) override;
	private:
		Mesh const* mesh_;
		GraphicsModule* gmodule_;
		DirectX::XMFLOAT4X4* worldMatrixPtr_;
		ID3D11PixelShader* pixelShader_;
		ID3D11SamplerState* samplerState_;
		ID3D11Buffer* materialCBuffer_;
	};
}