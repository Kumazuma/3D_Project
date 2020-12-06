#pragma once
#include "component.hpp"
#include "comtag.hpp"
#include "module.hpp"
#include "event.hpp"
#include "transform.hpp"
#include "ComponentFactory.hpp"
namespace Kumazuma
{
	namespace Game
	{
		using RectF = _2D::RectF;
		class CollisionEvent : public Event
		{
		public:
			CollisionEvent(const EventTag<CollisionEvent>& tag, std::weak_ptr<const Object>  other) :Event{ tag }, m_other { std::move(other) } {}
			std::weak_ptr<const Object> GetOther()const { return m_other; }
		private:
			std::weak_ptr<const Object> m_other;
		};
		struct ref_less
		{
		public:
			bool operator ()(const ObjectTag& arg1, const ObjectTag& arg2) const
			{
				return &arg1 < &arg2;
			}
		};
		
		
		extern const EventTag<CollisionEvent> EVT_COLLISION;
	}
}