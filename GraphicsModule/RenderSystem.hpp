#pragma once
#include "common.hpp"
namespace Kumazuma
{
	class GraphicsModule;
	class Material;
	class DLL_CLASS RenderSystem
	{
	public:
		virtual ~RenderSystem();
		virtual void AddMaterial(Material* material) = 0;
		virtual void RemoveMaterial(Material* material) = 0;
		virtual void Render() = 0;
		virtual HRESULT LoadPixelShader(wchar_t const* path, ID3D11PixelShader** out) = 0;
		virtual HRESULT LoadVertexShader(wchar_t const* path, ID3D11VertexShader** out) = 0;
		virtual HRESULT LoadComputeShader(wchar_t const* path, ID3D11ComputeShader** out) = 0;
	public:
		static RenderSystem* Create(GraphicsModule* gmodule);
	};
}