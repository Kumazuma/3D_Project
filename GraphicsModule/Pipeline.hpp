#pragma once
#include "common.hpp"
#include <d3d11.h>
namespace Kumazuma
{
	struct ShaderByteCode
	{
		void* ptr;
		unsigned size;
	};
	class Pipeline
	{
	public:
		Pipeline(
			ShaderByteCode const& vertexShader,
			ShaderByteCode const& pixelShader,
			ShaderByteCode const& domainShader,
			ShaderByteCode const& hullShader,
			ShaderByteCode const& geometryShader,
			D3D11_BLEND_DESC const& blendState,
			UINT sampleMask,
			D3D11_RASTERIZER_DESC const& rasterSizerState,
			D3D11_DEPTH_STENCIL_DESC const& depthStancilState,

			
		);
	};
}
