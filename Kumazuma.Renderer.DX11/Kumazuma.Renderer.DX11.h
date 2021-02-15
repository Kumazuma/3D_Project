#pragma once

using namespace System;
namespace DirectX
{
	struct XMFLOAT4X4;
}
namespace Kumazuma::Renderer::DX11 {
	using namespace Collections::Generic;
	using namespace System::Windows::Forms;
	public ref class GraphicsDevice
	{
	protected:
		GraphicsDevice(Control^ control, int width, int height);
	public:
		static auto Initalize(Control^ control, int width, int height)->void;
		!GraphicsDevice();
		~GraphicsDevice();
		static property GraphicsDevice^ Instance {auto get()->GraphicsDevice^; }
		auto Render(Control^ control, IEnumerable<IRenderable^>^ renderable, Camera^ camera)->void;
	internal:
		auto AddRenderObject(RenderGroupID group, IRenderEntity^ entity)->void;
		auto GetEffect(String^ key, ID3DXEffect** effect)->void;
	private:
		auto ClearRenderTarget()->void;
		auto RenderDeferred(DirectX::XMFLOAT4X4 const* viewSpacePtr, DirectX::XMFLOAT4X4 const* projSpacePtr, DirectX::XMFLOAT4X4 const* viewProj)->void;
		auto DeferredLighting(DirectX::XMFLOAT4X4 const* viewSpacePtr, DirectX::XMFLOAT4X4 const* projSpacePtr, DirectX::XMFLOAT4X4 const* viewProj)->void;
		auto DeferredCombine()->void;
		auto DrawDebug()->void;
	internal:
		property RenderModule* RenderModuleHandle {auto get()->RenderModule*; }
		NativeRenderer* renderer_;
	private:
		Dictionary<RenderGroupID, IList<IRenderEntity^>^> renderGroups_;
	private:
		RenderModule* renderModulePtr_;
		IDirect3DVertexBuffer9* vertexBuffer_;
		IDirect3DIndexBuffer9* indexBuffer_;
		ID3DXSprite* sprite_;
		IDictionary<String^, size_t>^ effects_;
		ID3DXEffect* lightingEffect_;
		ID3DXEffect* combineEffect_;
		static GraphicsDevice^ s_instance;
	};
}
