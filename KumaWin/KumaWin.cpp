// KumaWin.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "./include/kumawin/kumawin.hpp"
#include "./include/kumawin/messagehandler.hpp"
HINSTANCE Kumazuma::WindowSystem::Window::s_hInstance = nullptr;
const wchar_t* const MUMA_WIN_CLASS_NAME = L"KumaWinClass";
void Kumazuma::WindowSystem::Window::Initialize(HINSTANCE hInstance)
{
	if (s_hInstance == nullptr)
	{
		s_hInstance = hInstance;
		WNDCLASSEXW wndClassEx{};
		wndClassEx.cbSize = sizeof(WNDCLASSEXW);
		wndClassEx.cbWndExtra = sizeof(Window*);
		wndClassEx.lpszClassName = MUMA_WIN_CLASS_NAME;
		wndClassEx.hCursor = LoadCursorW(nullptr, IDC_ARROW);
		wndClassEx.hInstance = hInstance;
		wndClassEx.lpszMenuName = nullptr;
		wndClassEx.hIcon = LoadIconW(nullptr, IDI_APPLICATION);
		wndClassEx.lpfnWndProc = &Window::Procedure;
		wndClassEx.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
		RegisterClassExW(&wndClassEx);
	}
	
}

Kumazuma::WindowSystem::Window::Window():
	m_pInnerData{nullptr}
{
	
}

Kumazuma::WindowSystem::Window::Window(Builder&& builder) :
	m_pInnerData{ nullptr }
{
	HWND hWindow = CreateWindowExW(
		0,
		MUMA_WIN_CLASS_NAME,
		builder.m_title.c_str(),
		builder.m_style,
		builder.m_postion.first,
		builder.m_postion.second,
		builder.m_size.first,
		builder.m_size.second,
		nullptr,
		nullptr,
		s_hInstance,//hInstance
		nullptr
		);
	if (hWindow == nullptr)
	{
		return;
	}
	m_pInnerData = new InnerData{};
	LONG* tmp = (LONG*)&m_pInnerData;
	for (int i = 0; i < sizeof(void*) / sizeof(LONG); ++i)
	{
		SetWindowLongW(hWindow, i * sizeof(LONG), tmp[i]);
	}
	GetClientRect(hWindow, &m_pInnerData->size);
	m_pInnerData->hWindow = hWindow;
	m_pInnerData->self = this;
}
void Kumazuma::WindowSystem::Window::operator=(Window&& win) noexcept
{
	Window tmp{ std::move(win) };
	
	if (m_pInnerData != nullptr)
	{
		DestroyWindow(m_pInnerData->hWindow);
		delete m_pInnerData;
		m_pInnerData = nullptr;
	}
	new(this) Window{ std::move(tmp) };
}
Kumazuma::WindowSystem::Window::Window(Window&& window)noexcept :
	m_pInnerData{ window.m_pInnerData }
{
	window.m_pInnerData = nullptr;
	m_pInnerData->self = this;
}

Kumazuma::WindowSystem::Window::~Window()
{
	if (m_pInnerData != nullptr)
	{
		if (m_pInnerData->hWindow != nullptr)
		{
			for (int i = 0; i < sizeof(void*) / sizeof(LONG); ++i)
			{
				SetWindowLongW(m_pInnerData->hWindow, i * sizeof(LONG), 0);
			}
		}
		Disconnect();
		delete m_pInnerData;
		m_pInnerData = nullptr;
	}
}

HWND Kumazuma::WindowSystem::Window::GetHandle() const
{
	return m_pInnerData->hWindow;
}
RECT Kumazuma::WindowSystem::Window::GetRect() const
{
	if (m_pInnerData != nullptr)
	{
		return m_pInnerData->size;
	}
	return RECT();
}
void Kumazuma::WindowSystem::Window::SetTitle(const wchar_t* title)
{
	if (m_pInnerData != nullptr)
	{
		SetWindowTextW(m_pInnerData->hWindow, title);
	}
}
void Kumazuma::WindowSystem::Window::ValidateRect(const RECT& rc)
{
	::ValidateRect(m_pInnerData->hWindow, &rc);
}

POINT Kumazuma::WindowSystem::Window::ScreenToClient(const POINT& pt)
{
	POINT cpt = pt;
	::ScreenToClient(m_pInnerData->hWindow, &cpt);
	return cpt;
}

POINT Kumazuma::WindowSystem::Window::GetClientCursorPos() const
{
	POINT pt{};
	::GetCursorPos(&pt);
	::ScreenToClient(m_pInnerData->hWindow, &pt);
	return pt;
}

auto Kumazuma::WindowSystem::Window::GetInnerData(HWND hWindow)->InnerData*
{
	InnerData* ptr;
	LONG* pLONG = (LONG*) &ptr;
	for (int i = 0; i < sizeof(void*) / sizeof(LONG); ++i)
	{
		pLONG[i] = GetWindowLongW(hWindow, i * sizeof(LONG));
	}
	return ptr;
}

void Kumazuma::WindowSystem::Window::Connect(MessageHandlerBase* handler)
{
	if (m_pInnerData != nullptr)
	{
		m_pInnerData->handlers.insert(handler);
	}
}
void Kumazuma::WindowSystem::Window::DoDisconnect(const WinMsgIDBase& eventIdTag, const void* handler)
{
	if (m_pInnerData != nullptr)
	{
		auto& table = m_pInnerData->handlers;
		auto it = table.begin();
		while (it != table.end())
		{
			auto* field = *it;
			if (field->GetHandler() == handler &&
				field->EventID() == eventIdTag)
			{
				it = table.erase(it);
				delete field;
			}
			else
			{
				++it;
			}
		}
	}
	
}
void Kumazuma::WindowSystem::Window::Disconnect()
{
	if (m_pInnerData != nullptr)
	{
		for (auto it : m_pInnerData->handlers)
		{
			delete it;
		}
		m_pInnerData->handlers.clear();
	}
}
void Kumazuma::WindowSystem::Window::Show()
{
	if (m_pInnerData != nullptr)
	{
		ShowWindow(m_pInnerData->hWindow, SW_SHOW);
	}
}
void Kumazuma::WindowSystem::Window::Close()
{
	if (m_pInnerData != nullptr)
	{
		DestroyWindow(m_pInnerData->hWindow);
	}
}
LRESULT Kumazuma::WindowSystem::Window::Procedure(const HWND hWnd,const UINT uMsg,const WPARAM wParam,const LPARAM lParam)
{
	if (uMsg != WM_CREATE)
	{
		InnerData* pInnerData = GetInnerData(hWnd);
		const WinMsgIDBase* id = nullptr;
		switch (uMsg)
		{
		case WM_PAINT:
			id = &Kumazuma::WindowSystem::EVT_Paint;
			break;
		case WM_DESTROY:
			id = &Kumazuma::WindowSystem::EVT_Destroy;
			break;
		case WM_TIMER:
			id = &Kumazuma::WindowSystem::EVT_Timer;
			break;
		case WM_KEYDOWN:
			id = &Kumazuma::WindowSystem::EVT_KeyDown;
			break;
		case WM_KEYUP:
			id = &Kumazuma::WindowSystem::EVT_KeyUp;
			break;
		case WM_SIZE:
			id = &Kumazuma::WindowSystem::EVT_Size;
			if (pInnerData != nullptr)
			{
				GetClientRect(hWnd, &pInnerData->size);
			}
			break;
		}
		if (pInnerData != nullptr && id != nullptr)
		{
			bool processed{ false };
			Event* event = id->NewEvent(*pInnerData->self, wParam, lParam);
			for (auto handler : pInnerData->handlers)
			{
				if (handler->EventID() == *id)
				{
					handler->Call(*event);
					processed = true;
				}
			}
			if (uMsg == WM_DESTROY)
			{
				pInnerData->hWindow = NULL;
			}
			if (event != nullptr)
			{
				delete event;

			}
			if (processed)
			{
				return 0;
			}
		}
		return DefWindowProcW(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

Kumazuma::WindowSystem::Window::Builder::Builder(const Builder& other):
	m_postion{other.m_postion},
	m_size{other.m_size},
	m_title{other.m_title},
	m_style{other.m_style}
{

}

Kumazuma::WindowSystem::Window::Builder::Builder()
{
	m_size = { CW_USEDEFAULT, CW_USEDEFAULT };
	m_postion = { CW_USEDEFAULT, CW_USEDEFAULT };
	m_style = WS_OVERLAPPEDWINDOW;
	//Move to,
	//Line To
	//Ellipse
	//Rectangle
}

Kumazuma::WindowSystem::Window::Builder::Builder(Builder&& other) noexcept:
	m_postion{ other.m_postion },
	m_size{ other.m_size },
	m_title{std::move(other.m_title) },
	m_style{ std::move(other.m_style) }
{
}

auto Kumazuma::WindowSystem::Window::Builder::title(const std::wstring& title)&&->Self
{
	m_title = title;
	return std::move(*this);
}
auto Kumazuma::WindowSystem::Window::Builder::title(const std::wstring& title) & ->Self&
{
	m_title = title;
	return *this;
}
auto Kumazuma::WindowSystem::Window::Builder::position(int x, int y) && ->Self
{
	m_postion = { x, y };
	return std::move(*this);
}
auto Kumazuma::WindowSystem::Window::Builder::position(int x, int y) & ->Self&
{
	m_postion = { x, y };
	return *this;
}
auto Kumazuma::WindowSystem::Window::Builder::size(int width, int height) && ->Self
{
	m_size = { width, height };
	return std::move(*this);
}
auto Kumazuma::WindowSystem::Window::Builder::style(DWORD value)&& ->Self
{
	m_style = value;
	return std::move(*this);
}
auto Kumazuma::WindowSystem::Window::Builder::style(DWORD value) & ->Self&
{
	m_style = value;
	return *this;
}
auto  Kumazuma::WindowSystem::Window::Builder::size(int width, int height) & ->Self&
{
	m_size = { width, height };
	return *this;
}