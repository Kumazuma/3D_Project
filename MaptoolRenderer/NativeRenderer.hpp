#pragma once
#pragma unmanaged
#include <RenderModule/Renderer.h>
namespace MaptoolRenderer
{
	class NativeRenderer : public IRendererBase
	{
	public:
		NativeRenderer(RenderModule* renderModule);
		auto GetRenderModuleRef()->RenderModule &;
		auto GetDevice(IDirect3DDevice9 * *out)->HRESULT;
		auto SetProjMatrix(DirectX::XMFLOAT4X4 const& mat)->void;
		auto SetViewMatrix(DirectX::XMFLOAT4X4 const& mat)->void;
		auto GetProjMatrix(DirectX::XMFLOAT4X4 * pOut)->HRESULT;
		auto GetViewMatrix(DirectX::XMFLOAT4X4 * pOut)->HRESULT;
		auto GetFrustum(Frustum* const pOutFrustum)->HRESULT;
	private:
		RenderModule* renderModule_;
		DirectX::XMFLOAT4X4* projSpaceMatrix_;
		DirectX::XMFLOAT4X4* viewSpaceMatrix_;
	};
}
#pragma managed