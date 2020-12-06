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
			//�⺻ ������.
			Window();
			Window(Builder&& builder);
			Window(Window&& window) noexcept;
			//�Ҹ���
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
			//��������ڴ� ����!
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
			//event��� ���� üũ
			static_assert(std::is_base_of<Event, EventTagType>::value == true, "�±��� �̺�Ʈ Ÿ���� �̺�Ʈ Ŭ������ ���ų� �ڽ� Ÿ���̾�� �մϴ�");
			static_assert(std::is_base_of<MethodEventType, EventTagType>::value == true, "�޼ҵ��� �Ű������� �±��� �̺�Ʈ Ÿ�԰� ���ų� �θ� Ÿ���̾�� �մϴ�");
			DefMsgHandler<Class, MethodEventType>* entity{ new DefMsgHandler<Class,MethodEventType>(tag, handler,method) };
			Connect(entity);
		}
		template<typename EventTagType, typename Class>
		inline void Window::Disconnect(const WinMsgID<EventTagType>& eventTagID, Class* handler)
		{
			static_assert(std::is_base_of<Event, EventTagType>::value == true, "�±��� �̺�Ʈ Ÿ���� �̺�Ʈ Ŭ������ ���ų� �ڽ� Ÿ���̾�� �մϴ�");
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