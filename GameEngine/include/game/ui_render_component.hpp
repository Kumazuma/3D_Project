#pragma once
#include "render_com.hpp"
namespace Kumazuma
{
	namespace Game
	{
		template<typename RendererT>
		class UIRenderComponent : public RenderComponent<RendererT>
		{
		public:
			const static ComponentTag<UIRenderComponent> COM_UI_RENDER;
			using IDrawObject = typename RenderComponent<RendererT>::IDrawObject;
			UIRenderComponent();
			UIRenderComponent(UIRenderComponent&&)noexcept;
			UIRenderComponent(IDrawObject* pDrawObject);
			~UIRenderComponent();
			void operator = (UIRenderComponent&& other)noexcept;
			virtual Component* Clone() const override;
		};
		template<typename Renderer>
		class ComponentFactory<UIRenderComponent<Renderer>> :public IComponentFactory
		{
		public:
			using IDrawObject = typename UIRenderComponent<Renderer>::IDrawObject;
			ComponentFactory(IDrawObject* pDraw) :m_pDrawObject{ pDraw }
			{

			}
			~ComponentFactory()
			{
				if (m_pDrawObject != nullptr)
				{
					delete m_pDrawObject;
					m_pDrawObject = nullptr;
				}
			}
			Component* Create()const override
			{
				return new UIRenderComponent<Renderer>(m_pDrawObject->Clone());
			}
			const ComTagBase& GetTag()const override
			{
				return UIRenderComponent<Renderer>::COM_UI_RENDER;
			}
		private:
			IDrawObject* m_pDrawObject;
		};
	}
}
#include "ui_render_component_impl.hpp"