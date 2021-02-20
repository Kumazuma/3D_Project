#pragma once
#include "common.hpp"
#include <d3d11.h>
#include<DirectXMath.h>
namespace Kumazuma
{
	class GraphicsModule;
	class Material;
	class SwapChain;
	enum class MeshType:int;
	class DLL_CLASS RenderSystem
	{
	public:
		static RenderSystem* Create(GraphicsModule* gmodule, SwapChain* swapChain);
		virtual ~RenderSystem();
		virtual void AddMaterial(Material* material) = 0;
		virtual void RemoveMaterial(Material* material) = 0;
		virtual void SettupVertexShader(MeshType type, ID3D11DeviceContext* context, DirectX::XMFLOAT4X4* worldSpace) = 0;
		virtual void Render(GraphicsModule* gmodule, DirectX::XMFLOAT4X4 const* view, DirectX::XMFLOAT4X4 const* proj) = 0;
	};
}