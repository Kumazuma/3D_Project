#pragma once

using namespace System;
class RenderModule;
class NativeMaptoolRenderer;
namespace MaptoolRenderer {
	ref class Camera;
	interface struct IRenderable;
	interface struct IRenderEntity;
	enum class RenderGroupID {
		None,
		Environment,
		Deferred,
		Alpha,
		Effect,
		Wireframe,
		UI
	};
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
		static property GraphicsDevice^ Instance{auto get()->GraphicsDevice^;}
		
		auto Render(Control^ control, IEnumerable<IRenderable^>^ renderable, Camera^ camera)->void;
	internal:
		auto AddRenderObject(RenderGroupID group, IRenderEntity^ entity)->void;
	internal:
		property RenderModule* RenderModuleHandle{auto get()->RenderModule*;}
		property NativeMaptoolRenderer* RendererHandle {auto get()->NativeMaptoolRenderer*;}
	private:
		Dictionary<RenderGroupID, IList<IRenderEntity^>^> renderGroups_;
	private:
		RenderModule* renderModulePtr_;
		NativeMaptoolRenderer* rendererPtr_;
		static GraphicsDevice^ s_instance;
	};
}
