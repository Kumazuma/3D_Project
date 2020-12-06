#pragma once
#include<Windows.h>
namespace Kumazuma
{
	namespace WindowSystem
	{
		class Object
		{
		public:
			virtual ~Object() = default;
		};
		class Event
		{
		public:
			Event(Object& sender);
			virtual ~Event() = default;
			Object& GetSender() const { return *m_pSender; }
			void Skip() { m_isSkip = true; }
			void ClearSkip() { m_isSkip = false; }
			bool IsSkip()const { return m_isSkip; }
		private:
			Object* const m_pSender;
			bool m_isSkip;
		};
		inline Event::Event(Object& sender) :m_pSender{ &sender }, m_isSkip{ false }
		{

		}
		class EventTagBase
		{
		public:
			virtual ~EventTagBase() {};
		};

		class WinMsgEvent : public Event
		{
		public:
			WinMsgEvent(Object& sender, WPARAM wParam, LPARAM lParam) :Event{ sender }, m_wParam{ wParam }, m_lParam{ lParam }{}
			WPARAM GetWParam()const { return m_wParam; }
			LPARAM GetLParam()const { return m_lParam; }
			~WinMsgEvent() {}
		private:
			WPARAM m_wParam;
			LPARAM m_lParam;
		};
		class WinSizeEvent : public WinMsgEvent
		{
		public:
			WinSizeEvent(Object& sender, WPARAM wParam, LPARAM lParam) :WinMsgEvent{ sender , wParam ,lParam } {}
			int GetWidth()const { return LOWORD(GetLParam()); }
			int GetHeight()const { return HIWORD(GetLParam()); }
			bool IsMaximized()const { return GetWParam() == SIZE_MAXIMIZED; }
			bool IsMinimized() const { return GetWParam() == SIZE_MINIMIZED; }
			bool IsRestored()const { return GetWParam() == SIZE_RESTORED; }
			~WinSizeEvent() {}
		private:
		};
		class WinMsgIDBase : public EventTagBase
		{
		public:
			virtual ~WinMsgIDBase() = default;
			virtual Event* NewEvent(Object& sender, WPARAM wParam, LPARAM lParam) const = 0;
			bool operator==(const WinMsgIDBase& other)const;
		};
		inline bool WinMsgIDBase::operator==(const WinMsgIDBase& other) const
		{
			return &other == this;
		}
		template<typename EVENT>
		class WinMsgID : public WinMsgIDBase
		{
		public:
			WinMsgID() {}
			virtual Event* NewEvent(Object& sender, WPARAM wParam, LPARAM lParam) const override;
		};
		template<typename EVENT>
		Event* WinMsgID<EVENT>::NewEvent(Object& sender, WPARAM wParam, LPARAM lParam) const
		{
			return new EVENT{ sender, wParam , lParam };
		}
		class MessageHandlerBase
		{
		protected:
			MessageHandlerBase(const WinMsgIDBase& id) :m_msgID{ &id } {}
		public:
			virtual void Call(Event&) = 0;
			virtual const void* GetHandler() const = 0;
			virtual const void* GetFunction() const = 0;
			virtual ~MessageHandlerBase() {}
			const WinMsgIDBase& EventID()const { return *m_msgID; }
		private:
			const WinMsgIDBase* const m_msgID;
		};
		template<typename Class, typename Event>
		class DefMsgHandler :public MessageHandlerBase
		{
		public:
			typedef void(Class::* Method)(Event&);
			DefMsgHandler(const WinMsgIDBase& msgID, Class* pReceiver, Method m_pHandler);
			const void* GetHandler()const  override { return m_pHandler; }
			const void* GetFunction()const override { return reinterpret_cast<const void*>(&m_pMethod); }
			void Call(Kumazuma::WindowSystem::Event&) override;
		private:
			Class* m_pHandler;
			Method m_pMethod;
		};
		template<typename Class, typename Event>
		inline DefMsgHandler<Class, Event>::DefMsgHandler(const WinMsgIDBase& msgID, Class* pHandler, Method pMethod) :
			MessageHandlerBase{ msgID },
			m_pHandler{ pHandler },
			m_pMethod{ pMethod }
		{

		}
		template<typename Class, typename Event>
		inline void DefMsgHandler<Class, Event>::Call(Kumazuma::WindowSystem::Event& event)
		{
			return (m_pHandler->*m_pMethod)(static_cast<Event&>(event));
		}
		extern const WinMsgID<WinMsgEvent> EVT_Destroy;
		extern const WinMsgID<WinMsgEvent> EVT_Paint;
		extern const WinMsgID<WinMsgEvent> EVT_Timer;
		extern const WinMsgID<WinMsgEvent> EVT_KeyDown;
		extern const WinMsgID<WinMsgEvent> EVT_KeyUp;
		extern const WinMsgID<WinSizeEvent> EVT_Size;
	}
	

}