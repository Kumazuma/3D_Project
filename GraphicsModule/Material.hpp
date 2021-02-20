#pragma once
#include "common.hpp"
#include <d3d11.h>
namespace Kumazuma
{
	enum class MaterialShadingClass
	{
		None,
		Environment,
		DeferredShading,
		ForwardShading,
		END
	};
	class RenderSystem;
	class DLL_CLASS Material
	{
	public:
		Material(MaterialShadingClass shadingClass);
		virtual ~Material();
		virtual void RenderShadowMap() = 0;
		virtual void Render(RenderSystem* renderSystem, ID3D11DeviceContext* ) = 0;
		MaterialShadingClass GetShadingClass()const;
	private:
		MaterialShadingClass shadingClass_;

	};
}