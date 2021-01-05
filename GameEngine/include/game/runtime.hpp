#pragma once
#include<memory>
#include<map>
#include<list>
#include<queue>
#include "game.hpp"
#include "component.hpp"
#include "comtag.hpp"
#include "module.hpp"
#include "event.hpp"
#include "list.hpp"
#include <unordered_map>
#include <shared_mutex>
#include <tbb/concurrent_queue.h>
namespace Kumazuma
{
	namespace ThreadPool
	{
		class Manager;
	}
	namespace Game
	{
		class ComTagBase;
		class EventTagBase;
		class Event;
		class EventProcessor;
		struct EventQueueItem
		{
			std::shared_ptr<Event> event;
			std::weak_ptr<Component> ptr;
		};
		class Runtime
		{
			friend class ObjectFactory;
			friend class Object;
			friend class EventProcessor;
			
		protected:
			Runtime();
			Runtime(const Runtime&) = delete;
		public:
			static std::shared_ptr<Runtime> Instance();
			static void Initialize();
			static void Release();
		public:
			~Runtime();
			//std::shared_ptr<Object> CreateObject();
			template<typename _ComT, typename _Evt>
			void Broadcast(const ComponentTag<_ComT>& tag, _Evt&& event);
			template<typename _Evt, typename _ComT, typename ...Arg>
			void Broadcast(const ComponentTag<_ComT>& tag, Arg&&... args);
			template<typename _ComT, typename _Evt>
			void Broadcast(const Object& obj, const ComponentTag<_ComT>& tag, _Evt&& event);
			template<typename _Evt, typename _ComT, typename ...Arg>
			void Broadcast(const Object& obj, const ComponentTag<_ComT>& tag, Arg&&... args);
			template<typename _COM, typename ...Args>
			auto CreateComponent(Args&& ...args)->std::shared_ptr<_COM>;
			auto Update(float t)->void;
			auto DispatchEvent()->void;
		private:
			static auto OnDeleteComponent(Component* com)->void;
		private:
			auto DoBroadcast(ComTagBase const& comTag, Event* event, Object const* const pObj = nullptr)->void;
			auto AddComponent(ComTagBase const*, Component*)->std::shared_ptr<Component>;
			auto GetTableLockRef(ComTagBase const* tag)->std::shared_mutex&;
			auto GC()->void;

		private:
			std::vector<std::unique_ptr<Module> > m_modules;
			std::shared_ptr<ThreadPool::Manager> m_threadPoolMgr;
			std::mutex m_lock;
			std::unordered_map<ComTagBase const*, std::shared_mutex> m_tableLocks;
			std::unordered_map<ComTagBase const*, std::unordered_set<Component*> > m_tagComponentTable;
			tbb::concurrent_queue<EventQueueItem> m_eventQueue;
		private:
			static std::shared_ptr<Runtime> s_managerInner;

		};
		template<typename _ComT, typename _Evt>
		inline void Runtime::Broadcast(const ComponentTag<_ComT>& tag, _Evt&& event)
		{
			static_assert(std::is_base_of<Component, _ComT>::value == true, "the tag is not valid.");
			DoBroadcast(tag, new _Evt{ std::move(event) });
		}
		template<typename _Evt, typename _ComT, typename ...Arg>
		inline void Runtime::Broadcast(const ComponentTag<_ComT>& tag, Arg&& ...args)
		{
			static_assert(std::is_base_of<Component, _ComT>::value == true, "the tag is not valid.");
			DoBroadcast(tag, new _Evt{ std::forward<Arg>(args)... });
		}
		template<typename _ComT, typename _Evt>
		inline void Runtime::Broadcast(const Object& obj, const ComponentTag<_ComT>& tag, _Evt&& event)
		{
			static_assert(std::is_base_of<Component, _ComT>::value == true, "the tag is not valid.");
			DoBroadcast(tag, new _Evt{ std::move(event) }, &obj);
		}
		template<typename _Evt, typename _ComT, typename ...Arg>
		inline void Runtime::Broadcast(const Object& obj, const ComponentTag<_ComT>& tag, Arg&&... args)
		{
			static_assert(std::is_base_of<Component, _ComT>::value == true, "the tag is not valid.");
			DoBroadcast(tag, new _Evt{ std::forward<Arg>(args)... }, &obj);
		}
		template<typename _COM, typename ...Args>
		inline auto Runtime::CreateComponent(Args && ...args) -> std::shared_ptr<_COM>
		{
			return std::static_pointer_cast<_COM>(AddComponent(_COM::TAG, new _COM{ std::forward<Args>(args)... }));
		}
		class UpdateEvent : public Event
		{
		public:
			explicit UpdateEvent(float delta);
			~UpdateEvent() {}
			float GetDelta()const { return m_delta; }
		private:
			float m_delta;
		};
		extern const EventTag<UpdateEvent> EVT_UPDATE;
	}
}
