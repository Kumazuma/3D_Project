#include "pch.h"
#include "StandardMaterial.hpp"
#include "Mesh.hpp"
namespace Kumazuma
{
	StandardMaterial::~StandardMaterial()
	{

	}
	void StandardMaterial::RenderShadowMap()
	{

	}
	void StandardMaterial::Render(ID3D11DeviceContext* deviceContext)
	{
		auto& mesh{ subset_->GetMesh() };
		mesh.SetupIA(deviceContext);
		
	}
}
