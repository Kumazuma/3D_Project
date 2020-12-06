#pragma once
#include "component.hpp"
#include "graphics.hpp"
#include "object.hpp"
#include "animation.hpp"
#include "ComponentFactory.hpp"
namespace Kumazuma
{
	namespace Game
	{
		extern const EventTag<Event> EVT_OutOfView;
		template<typename Renderer>
		class RenderComponent : public Component
		{
		public:
			const static ComponentTag<RenderComponent> TAG;
			struct IDrawObject;
			
		public:
			RenderComponent();
			RenderComponent(RenderComponent&&)noexcept;
			RenderComponent(const RenderComponent&);
			RenderComponent(IDrawObject* pDrawObject);
			~RenderComponent();
			void operator = (RenderComponent&& other)noexcept;
			static RenderComponent Circle(float radius);
			static RenderComponent Rectangle(float width, float height);
			virtual Component* Clone() const override;
			void SetInViewport(bool value) { m_isInViewport = value; }
			bool IsInViewport()const { return m_isInViewport; }
			IDrawObject& GetDrawObject()const { return *m_pDrawObject; }
		protected:
			RenderComponent(IDrawObject* pDrawObject, const ComTagBase& tagBase);
		private:
			bool m_isInViewport;
			IDrawObject* m_pDrawObject;
		};
		template<typename Renderer>
		class ComponentFactory<RenderComponent<Renderer>> :public IComponentFactory
		{
		public:
			using IDrawObject = typename RenderComponent<Renderer>::IDrawObject;
			ComponentFactory(IDrawObject* pDraw):m_pDrawObject{ pDraw }
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
				return new RenderComponent<Renderer>(m_pDrawObject->Clone());
			}
			const ComTagBase& GetTag()const override
			{
				return RenderComponent<Renderer>::TAG;
			}
		private:
			IDrawObject* m_pDrawObject;
		};

		template<typename Renderer>
		struct RenderComponent<Renderer>::IDrawObject
		{
			typedef typename Renderer::DrawContext DrawContext;
			virtual void Render(DrawContext&, const Object& obj) = 0;
			virtual void PostUpdate(float timeDelta) {}
			virtual IDrawObject* Clone() const = 0;
			virtual ~IDrawObject() = default;
		};
		
		template<typename Renderer>
		class SimpleRectangle : public RenderComponent<Renderer>::IDrawObject
		{
		public:
			typedef typename RenderComponent<Renderer>::IDrawObject IDrawObject;
			typedef typename Renderer::DrawContext DrawContext;
			SimpleRectangle(float width, float height) :
				m_width{ width },
				m_height{ height }
			{

			}
			void Render(DrawContext& context, const Kumazuma::Game::Object& obj) override
			{
				const auto center = obj.GetTransform();
				const _2D::Vector2 tmp{ m_width / 2, m_height / 2 };
				const _2D::Vector2 leftTop = center - tmp;
				const _2D::Vector2 rightBottom = center + tmp;
				context.DrawRectangle(leftTop, rightBottom);
			}
			IDrawObject* Clone() const override
			{
				return new SimpleRectangle{ m_width, m_height };
			}
		private:
			float m_width;
			float m_height;
		};
		template<typename Renderer>
		class SimpleCircle: public RenderComponent<Renderer>::IDrawObject
		{
			float m_radius;
		public:
			typedef typename RenderComponent<Renderer>::IDrawObject IDrawObject;
			typedef typename Renderer::DrawContext DrawContext;
			SimpleCircle(float radius) :m_radius{ radius }
			{

			}
			void Render(typename Renderer::DrawContext& context, const Object& obj)
			{
				const auto center = obj.GetTransform();
				const _2D::Vector2 tmp{ m_radius / 2, m_radius / 2 };
				const _2D::Vector2 leftTop = center - tmp;
				const _2D::Vector2 rightBottom = center + tmp;
				context.DrawEllipse(leftTop, rightBottom);
			}
			IDrawObject* Clone() const
			{
				return new SimpleCircle{ m_radius };
			}
		};
	}
}
#include "render_com_impl.hpp"
