#pragma once
#include"game.hpp"
#include "common.hpp"
namespace Kumazuma
{
	namespace Game
	{
		class DLL_CLASS EventTagBase
		{
		public:
			virtual ~EventTagBase() = default;
			const char* GetString()const { return m_eventTag; }
			bool operator==(const EventTagBase& other)const { return this == &other; }
			bool operator!=(const EventTagBase& other)const { return this != &other; }
		protected:
			EventTagBase(const char* const eventTag) :m_eventTag{ eventTag } {}
			EventTagBase(const EventTagBase&) = delete;
			EventTagBase(EventTagBase&&) = delete;
			
		private:
			const char* const m_eventTag;
		};
		template<typename _EventT>
		class EventTag : public EventTagBase
		{
		public:
			EventTag(const char* const eventTag) :EventTagBase{ eventTag } {}
		};

		class Event
		{
		public:
			virtual ~Event() = default;
			const EventTagBase& GetTag()const { return *m_pEventTag; }
			Event(const EventTagBase& tag) :m_pEventTag{ &tag } {}
		protected:
		private:
			const EventTagBase* m_pEventTag;
		};
		
	}
}