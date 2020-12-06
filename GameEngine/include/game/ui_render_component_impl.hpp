#pragma once
#include "ui_render_component.hpp"
namespace Kumazuma
{
	namespace Game
	{

		template<typename Renderer>
		const ComponentTag<UIRenderComponent<Renderer>> UIRenderComponent<Renderer>::COM_UI_RENDER{ "COM_UI_RENDER" };
		template<typename Renderer>
		inline UIRenderComponent<Renderer>::UIRenderComponent() :
			RenderComponent<Renderer>{ nullptr, UIRenderComponent::COM_UI_RENDER  }
		{

		}
		template<typename Renderer>
		inline UIRenderComponent<Renderer>::UIRenderComponent(UIRenderComponent&& other) noexcept :
			RenderComponent<Renderer>{ std::move(other) }
		{
		}
		template<typename Renderer>
		inline UIRenderComponent<Renderer>::UIRenderComponent(IDrawObject* pDrawObject) :
			RenderComponent<Renderer>{ pDrawObject,UIRenderComponent::COM_UI_RENDER }
		{
		}
		template<typename Renderer>
		inline UIRenderComponent<Renderer>::~UIRenderComponent()
		{
		}
		template<typename Renderer>
		inline void UIRenderComponent<Renderer>::operator=(UIRenderComponent&& other) noexcept
		{
			UIRenderComponent tmp{ std::move(other) };
			this->~UIRenderComponent();
			new(this) UIRenderComponent{ std::move(tmp) };
		}
		template<typename Renderer>
		inline Component* UIRenderComponent<Renderer>::Clone() const
		{
			return new UIRenderComponent{ this->GetDrawObject().Clone() };
		}
	}
}