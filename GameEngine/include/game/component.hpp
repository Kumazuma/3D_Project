#pragma once

#include<set>
#include<map>
#include<vector>
#include<type_traits>

#include"game.hpp"
#include<memory>
#include "comtag.hpp"
#include"event.hpp"
#include<unordered_map>
#include<unordered_set>
namespace Kumazuma
{
	namespace Game
	{

		class Component : public std::enable_shared_from_this<Component>
		{
			friend class Runtime;
			friend class Module;
			friend class Object;
			using EventHandleMthod = void(Component::*)(const Event&);
		public:
			const ComTagBase& GetTag()const { return *m_pTag; }
			virtual Component* Clone() const = 0;
			virtual ~Component() = default;
			virtual void OnLoaded() {}
			std::weak_ptr<const Object> GetObj()const;
			auto HandleEvent(Event& event)->void;
		protected:
			Component(const ComTagBase& tag);
			std::weak_ptr<Object> GetObj();
			
			bool IsHandled(const EventTagBase& eventTag)const { return m_handledEvent.find(&eventTag) != m_handledEvent.end(); }
			template<typename EventT, typename DerivedType, typename MethodArg>
			void Bind(const EventTag<EventT>& handleEventTag, void(DerivedType::* mehod)(MethodArg&) );
			void UnbindAll();
		private:
			void _Bind(const EventTagBase*, EventHandleMthod handler);
			void SetObject(std::weak_ptr<Object> obj);
			void HandleEvent(Event*);
		private:
			std::unordered_set<const EventTagBase*> m_handledEvent;
			std::unordered_map<const EventTagBase*, EventHandleMthod> m_handlers;
			const ComTagBase* m_pTag;
			std::weak_ptr<Object> m_parent;
		};

		template<typename EventT, typename DerivedType, typename MethodArg>
		inline void Component::Bind(const EventTag<EventT>& handleEventTag, void(DerivedType::* method)(MethodArg&))
		{
			static_assert(std::is_base_of<Component, DerivedType>::value == true, "DrivedType is not derived from component");
			static_assert(std::is_base_of<EventT, MethodArg>::value == true, "MethodArg is not based on EventT");
			static_assert(std::is_base_of<Event, EventT>::value == true, "");
			_Bind(&handleEventTag, (EventHandleMthod)method);
		}
		extern const ComponentTag<Component> COM_ANY;
	}
}