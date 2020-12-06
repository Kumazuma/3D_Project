#pragma once
#include<Windows.h>
#include<string>
#include<set>
#include"messagehandler.hpp"
#include<type_traits>
namespace Kumazuma
{
	namespace WindowSystem
	{
		class MessageHandlerBase;
		class Window : public Object
		{
			struct InnerData;
		public:
			static void Initialize(HINSTANCE hInstance);
		public:
			class Builder;
			//기본 생성자.
			Window();
			Window(Builder&& builder);
			Window(Window&& window) noexcept;
			//소멸자
			~Window();
			void operator=(Window&& win) noexcept;
			bool IsValid()const { return m_pInnerData != nullptr; }
			HWND GetHandle()const;
			RECT GetRect()const;
			void ValidateRect() { ValidateRect(this->GetRect()); }
			void ValidateRect(const RECT& rect);
			POINT ScreenToClient(const POINT& pt);
			POINT GetClientCursorPos() const;
			void SetTitle(const wchar_t* title);
			template<typename EventTagType, typename Class, typename MethodEventType>
			void Connect(const WinMsgID<EventTagType>& tag, Class* handler, void(Class::* method)(MethodEventType&));
			template<typename Event, typename Class>
			void Disconnect(const WinMsgID<Event>&, Class*);
			void Disconnect();
			void Show();
			void Close();
		private:
			Window(const Window&) = delete;
			static InnerData* GetInnerData(HWND hWindow);
			void Connect(MessageHandlerBase* handler);
			void DoDisconnect(const WinMsgIDBase& eventIdTag, const void* handler);
		private:
			//복사생성자는 사형!
			InnerData* m_pInnerData;
		private:
			static LRESULT CALLBACK Procedure(HWND, UINT, WPARAM, LPARAM);
			static HINSTANCE s_hInstance;
			struct InnerData
			{
				HWND hWindow;
				Window* self;
				std::set<MessageHandlerBase*> handlers;
				RECT size;
			};
		};
		template<typename EventTagType, typename Class, typename MethodEventType>
		inline void Window::Connect(const WinMsgID<EventTagType>& tag, Class* handler, void(Class::* method)(MethodEventType&))
		{
			//event상속 관계 체크
			static_assert(std::is_base_of<Event, EventTagType>::value == true, "태그의 이벤트 타입은 이벤트 클래스와 같거나 자식 타임이어야 합니다");
			static_assert(std::is_base_of<MethodEventType, EventTagType>::value == true, "메소드의 매개변수는 태그의 이벤트 타입과 같거나 부모 타입이어야 합니다");
			DefMsgHandler<Class, MethodEventType>* entity{ new DefMsgHandler<Class,MethodEventType>(tag, handler,method) };
			Connect(entity);
		}
		template<typename EventTagType, typename Class>
		inline void Window::Disconnect(const WinMsgID<EventTagType>& eventTagID, Class* handler)
		{
			static_assert(std::is_base_of<Event, EventTagType>::value == true, "태그의 이벤트 타입은 이벤트 클래스와 같거나 자식 타임이어야 합니다");
			DoDisconnect(eventTagID, (const void*)handler);
		}
		class Window::Builder final
		{
			friend class Window;
		public:
			typedef Window::Builder Self;
			Builder();
			Builder(Builder&&) noexcept;
			Self title(const std::wstring& title)&&;
			Self& title(const std::wstring& title)&;
			Self position(int x, int y)&&;
			Self& position(int x, int y)&;
			Self size(int width, int height)&&;
			Self& size(int width, int height)&;
			Self style(DWORD value)&&;
			Self& style(DWORD value)&;
		private:
			Builder(const Builder&);
		private:
			DWORD m_style;
			std::pair<int, int> m_size;
			std::pair<int, int> m_postion;
			std::wstring m_title;
		};
	}
	
}