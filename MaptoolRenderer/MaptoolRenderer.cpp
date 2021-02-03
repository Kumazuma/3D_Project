#include "pch.h"
#include "MaptoolRenderer.h"
#include "IRenderable.hpp"
#include <RenderModule/RenderModule.h>
#include <msclr\lock.h>
MaptoolRenderer::GraphicsDevice::GraphicsDevice(Control^ control, int width, int height)
{
	HWND hwnd{reinterpret_cast<HWND>( control->Handle.ToPointer()) };
	RenderModule* res{};
	RenderModule::Create(hwnd, width, height, &res);
	this->renderModulePtr_ = res;
}
MaptoolRenderer::GraphicsDevice::!GraphicsDevice()
{
	Dispose();
}

MaptoolRenderer::GraphicsDevice::~GraphicsDevice()
{
	if (s_instance != nullptr)
	{
		msclr::lock r(GraphicsDevice::typeid);
		if (s_instance != nullptr)
		{
			s_instance = nullptr;
		}
	}
	if (renderModulePtr_ != nullptr)
	{
		delete renderModulePtr_;
		renderModulePtr_ = nullptr;
	}
	if (rendererPtr_ != nullptr)
	{
		delete rendererPtr_;
		rendererPtr_ = nullptr;
	}
}

auto MaptoolRenderer::GraphicsDevice::Initalize(Control^ control, int width, int height) -> void
{
	if (s_instance != nullptr)
	{
		msclr::lock r(GraphicsDevice::typeid);
		if (s_instance != nullptr)
		{
			s_instance = gcnew GraphicsDevice(control, width, height);
		}
	}
}

auto MaptoolRenderer::GraphicsDevice::Instance::get() -> GraphicsDevice^
{
	return s_instance;
}
auto MaptoolRenderer::GraphicsDevice::RenderModuleHandle::get() -> RenderModule*
{
	return renderModulePtr_;
}
auto MaptoolRenderer::GraphicsDevice::RendererHandle::get() -> NativeMaptoolRenderer*
{
	return rendererPtr_;
}

auto MaptoolRenderer::GraphicsDevice::Render(Control^ control, IEnumerable<IRenderable^>^ renderables, Camera^ camera) -> void
{
	for each (IRenderable ^ renderable in renderables)
	{
		renderable->PrepereRender(this);
	}
}

auto MaptoolRenderer::GraphicsDevice::AddRenderObject(RenderGroupID group, IRenderEntity^ entity) -> void
{
	if (!this->renderGroups_.ContainsKey(group))
	{
		this->renderGroups_.Add(group, gcnew List<IRenderEntity^>{});
	}
	auto^ list{ this->renderGroups_[group] };
	list->Add(entity);
}
