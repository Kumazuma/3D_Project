#pragma once
#include "Material.hpp"
#include "Subset.hpp"
namespace Kumazuma
{
	class DLL_CLASS StandardMaterial : public Material
	{
		struct Impl;
	public:
		StandardMaterial( ) = delete;
		~StandardMaterial();
		void RenderShadowMap();
		void Render(ID3D11DeviceContext* deviceContext) override;
	private:
		Subset const* subset_;
		DirectX::XMFLOAT4X4* worldMatrixPtr_;
	};
}