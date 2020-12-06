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
namespace Kumazuma
{
	namespace Game
	{
		class ComTagBase;
		class EventTagBase;
		class Event;
		class Runtime
		{
			friend class ObjectFactory;
			friend class Object;
			struct EventQueueItem
			{
				Event* event;
				Kumazuma::LinkedList<std::weak_ptr<Component>> handledCompnent;
			};
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
			//template<typename _Com>
			//void AddComponent(std::shared_ptr<Object>& obj, _Com&& component);
			//template<typename _Com, typename ...Arg>
			//void AddComponent(std::shared_ptr<Object>& obj, Arg&& ...args);
			template<typename _Com>
			std::vector<std::shared_ptr<const _Com>> GetComponents(const ComponentTag<_Com>& tag) const;
			template<typename _Com>
			const _Com& GetComponent(const ComponentTag<_Com>& tag)const;

			template<typename _Com, typename Fn>
			void ForEach(const ComponentTag<_Com>& tag, Fn fn) const;
			template<typename _Com, typename Class, typename Fn, typename...Args>
			void ForEach(const ComponentTag<_Com>& tag, Class* self, Fn fn, Args&&...args) const;
			template<typename _ModuleT>
			void AddModule(_ModuleT&&);
			void Update(float);
			void GC();
		protected:
			size_t GetObjUid(const Object& obj);
			std::shared_ptr<Object> CreateObject(const ObjectFactory& factory);
			Runtime();
			Runtime(const Runtime&) = delete;
			void DoBroadcast(const ComTagBase& comTag, Event* event, size_t objId = 0);
		private:
			std::vector<std::unique_ptr<Module> > m_modules;
			std::unordered_map<size_t, Kumazuma::LinkedList<std::shared_ptr<Component> > > m_objectNComs;
			std::unordered_map<const ComTagBase*, Kumazuma::LinkedList<std::weak_ptr<Component>, 64 > > m_tagNComs;
			Kumazuma::LinkedList<Object, 64> m_objectPool;
			//해당 업데이트 함수에서 돌릴 준비가 된 컴포넌트들
			Kumazuma::LinkedList<Component*, 64> m_readyComponentQueue;
			Kumazuma::LinkedList<size_t, 64> m_readyToDestoryObjectQueue;
			size_t m_lastGameObjectIndex;
			Kumazuma::LinkedList<EventQueueItem> m_queue;
		private:
			static void DeleteObject(Object* ptr);
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
			DoBroadcast(tag, new _Evt{ std::move(event) },GetObjUid(obj));
		}
		template<typename _Evt, typename _ComT, typename ...Arg>
		inline void Runtime::Broadcast(const Object& obj, const ComponentTag<_ComT>& tag, Arg&&... args)
		{
			static_assert(std::is_base_of<Component, _ComT>::value == true, "the tag is not valid.");
			DoBroadcast(tag, new _Evt{ std::forward<Arg>(args)... }, GetObjUid(obj));
		}
		template<typename _Com>
		inline const _Com& Runtime::GetComponent(const ComponentTag<_Com>& tag)const
		{
			const typename decltype(m_tagNComs)::const_iterator node = m_tagNComs.find(&tag);
			if (node != m_tagNComs.end())
			{
				if (node->second.empty() == false)
				{
					const _Com& c = static_cast<const _Com&>(*node->second.begin()->lock());
					return c;
				}
			}
			return *(const _Com*) nullptr;
		}
		template<typename _ComT>
		inline std::vector<std::shared_ptr<const _ComT>> Runtime::GetComponents(const ComponentTag<_ComT>& tag) const
		{
			const typename decltype(m_tagNComs)::const_iterator node = m_tagNComs.find(&tag);
			if (node != m_tagNComs.end())
			{
				std::vector<std::shared_ptr<const _ComT>> res;
				res.reserve(node->second.size());
				for (auto it : node->second)
				{
					auto ref = it.lock();
					if (ref != nullptr)
					{
						res.emplace_back(std::static_pointer_cast<const _ComT>(ref));
					}
					
				}
				return res;
			}
			return std::vector<std::shared_ptr<const _ComT>>{};
		}
		template<typename _ComT, typename Fn>
		inline  void Runtime::ForEach(const ComponentTag<_ComT>& tag, Fn fn) const
		{
			const typename decltype(m_tagNComs)::const_iterator node = m_tagNComs.find(&tag);
			if (node != m_tagNComs.end())
			{
				for (auto it : node->second)
				{
					auto ref = it.lock();
					if (ref != nullptr)
					{
						fn(static_cast<const _ComT&>(*ref));
					}
				}
			}
		}
		template<typename _Com, typename Class, typename Fn, typename ...Args>
		inline void Runtime::ForEach(const ComponentTag<_Com>& tag, Class* self, Fn fn, Args&& ...args) const
		{
			const typename decltype(m_tagNComs)::const_iterator node = m_tagNComs.find(&tag);
			if (node != m_tagNComs.end())
			{
				for (auto it : node->second)
				{
					auto ref = it.lock();
					if (ref != nullptr)
					{
						(self->*fn)(static_cast<const _Com&>(*ref), args...);
					}
																				
				}
			}
		}
		template<typename _ModuleT>
		inline void Runtime::AddModule(_ModuleT&& module)
		{
			m_modules.push_back(std::unique_ptr<Module>{new _ModuleT{ std::move(module) }});
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
