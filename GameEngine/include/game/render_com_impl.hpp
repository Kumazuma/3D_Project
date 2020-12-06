#pragma once
#include "render_com.hpp"
namespace Kumazuma
{
	namespace Game
	{
		
		template<typename Renderer>
		const ComponentTag<RenderComponent<Renderer>> RenderComponent<Renderer>::TAG{ "COM_RENDER" };
		template<typename Renderer>
		inline RenderComponent<Renderer>::RenderComponent() :
			Component{ RenderComponent::TAG },
			m_pDrawObject{ nullptr },
			m_isInViewport{ false }
		{
		}
		template<typename Renderer>
		inline RenderComponent<Renderer>::RenderComponent(RenderComponent&& other) noexcept :
			Component{ std::move(other) },
			m_pDrawObject{ other.m_pDrawObject },
			m_isInViewport{ other.m_isInViewport }
		{
			other.m_pDrawObject = nullptr;
		}
		template<typename Renderer>
		inline RenderComponent<Renderer>::RenderComponent(const RenderComponent& other):
			RenderComponent{other.m_pDrawObject->Clone(), other.GetTag()}
		{
		}
		template<typename Renderer>
		inline RenderComponent<Renderer>::RenderComponent(IDrawObject* pDrawObject) :
			RenderComponent{}
		{
			m_pDrawObject = pDrawObject;
		}
		template<typename Renderer>
		inline RenderComponent<Renderer>::~RenderComponent()
		{
			if (m_pDrawObject != nullptr)
			{
				delete m_pDrawObject;
			}
		}
		template<typename Renderer>
		inline void RenderComponent<Renderer>::operator=(RenderComponent&& other) noexcept
		{
			RenderComponent tmp{ std::move(other) };
			this->~RenderComponent();
			new(this) RenderComponent{ std::move(tmp) };
		}
		template<typename Renderer>
		inline RenderComponent<Renderer> RenderComponent<Renderer>::Circle(float radius)
		{
			return RenderComponent{ new SimpleCircle<Renderer>{radius} };
		}
		template<typename Renderer>
		inline RenderComponent<Renderer> RenderComponent<Renderer>::Rectangle(float width, float height)
		{
			return RenderComponent{ new SimpleRectangle<Renderer>{width , height} };
		}
		template<typename Renderer>
		inline Component* RenderComponent<Renderer>::Clone() const 
		{
			return new RenderComponent{ m_pDrawObject->Clone() };
		}
		template<typename Renderer>
		inline RenderComponent<Renderer>::RenderComponent(IDrawObject* pDrawObject, const ComTagBase& tagBase):
			Component{ tagBase }, m_pDrawObject{ pDrawObject }, m_isInViewport{ false }
		{
		}
	}
}