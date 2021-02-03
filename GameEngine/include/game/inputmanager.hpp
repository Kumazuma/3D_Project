#pragma once
#include<vector>
#include<array>
#include<map>
#include<set>
#include<Windows.h>
#include<string>
#include<memory>
#include<unordered_set>
#include<unordered_map>
namespace Kumazuma
{
	namespace Game
	{
		template<typename EnumT>
		class InputManager
		{
			InputManager() {}
			InputManager(const InputManager&) = delete;
			InputManager(InputManager&&) = delete;
			static std::shared_ptr<InputManager> s_instance;
			std::unordered_set<unsigned> m_press;
			std::unordered_set<unsigned> m_down;
			std::unordered_set<unsigned> m_up;
			std::unordered_map<EnumT, unsigned> m_mappedKeyData;
			POINT m_cursorPosition = { 0,0 };
			POINT m_prevPosition = { 0,0 };
			HWND m_hwnd = NULL;
			HCURSOR m_hOldCursor = NULL;
			bool m_locked = false;
			
		public:
			static std::shared_ptr<InputManager> Instance();
			std::unordered_map<EnumT, unsigned> GetKeyMapping()const;
			

			bool IsPressing(EnumT id);
			bool IsDown(EnumT id);
			bool IsUp(EnumT id);

			void Update();
			void Bind(EnumT id, unsigned key);
			POINT GetMousePos()const;
			POINT GetPrevPos()const;
			bool IsLock();
			void LockCursor(HWND hWnd);
			void UnlockCursor();
		};
		template<typename EnumT>
		std::shared_ptr<InputManager<EnumT>> InputManager<EnumT>::s_instance = nullptr;
		template<typename EnumT>
		inline std::shared_ptr<InputManager<EnumT> > InputManager<EnumT>::Instance() {
			if (s_instance == nullptr)
			{
				s_instance.reset(new InputManager{});
			}
			return s_instance;
		}
		template<typename EnumT>
		inline std::unordered_map<EnumT, unsigned> InputManager<EnumT>::GetKeyMapping() const
		{
			return m_mappedKeyData;
		}
		template<typename EnumT>
		inline bool InputManager<EnumT>::IsPressing(EnumT id)
		{
			auto res = m_mappedKeyData.find(id);
			if (res == m_mappedKeyData.end())
			{
				return false;
			}
			unsigned key = res->second;
			if (m_press.find(key) == m_press.end())
			{
				return false;
			}
			return true;
		}
		template<typename EnumT>
		inline bool InputManager<EnumT>::IsDown(EnumT id)
		{
			auto res = m_mappedKeyData.find(id);
			if (res == m_mappedKeyData.end())
			{
				return false;
			}
			unsigned key = res->second;
			if (m_press.find(key) == m_press.end())
			{
				return false;
			}
			auto isdown = m_down.find(key);
			if (isdown != m_down.end())
			{
				return false;
			}
			m_down.insert(key);
			return true;
		}
		template<typename EnumT>
		inline bool InputManager<EnumT>::IsUp(EnumT id)
		{
			auto res = m_mappedKeyData.find(id);
			if (res == m_mappedKeyData.end())
			{
				return false;
			}
			unsigned key = res->second;
			//지금 키가 눌려 있으면 일단 false다
			if (m_press.find(key) != m_press.end())
			{
				return false;
			}

			auto ispress = m_up.find(key);
			if (ispress != m_up.end())
			{
				m_up.erase(ispress);
				return false;
			}
			m_up.insert(key);
			return true;
		}
		template<typename EnumT>
		inline void InputManager<EnumT>::Update()
		{
			m_press.clear();
			if (GetActiveWindow() != NULL)
			{
				m_prevPosition = m_cursorPosition;
				GetCursorPos(&m_cursorPosition);
				if (m_locked && m_hwnd != NULL)
				{
					//ShowCursor(FALSE);
					if (GetActiveWindow() == m_hwnd)
					{
						POINT clientCursorPt{ m_cursorPosition };
						ScreenToClient(m_hwnd, &clientCursorPt);
						RECT rc{};
						GetClientRect(m_hwnd, &rc);
						LONG const width{ rc.right - rc.left };
						LONG const height{ rc.bottom - rc.top };

						POINT pt{
							(rc.right - rc.left) / 2,
							(rc.bottom - rc.top) / 2
						};
						m_prevPosition = pt;
						ClientToScreen(m_hwnd, &pt);
						SetCursorPos(pt.x, pt.y);
					}
				}
				else
				{
					//ShowCursor(TRUE);

				}
				for (auto& it : m_mappedKeyData)
				{
					if (GetAsyncKeyState(it.second) & 0x8000)
					{
						m_press.insert(it.second);
						m_up.erase(it.second);
					}
					else
					{
						m_down.erase(it.second);
					}
				}
			}
		}
		template<typename EnumT>
		inline void InputManager<EnumT>::Bind(EnumT id, unsigned key)
		{
			m_mappedKeyData[id] = key;
		}
		template<typename EnumT>
		inline POINT InputManager<EnumT>::GetMousePos() const
		{
			return m_cursorPosition;
		}
		template<typename EnumT>
		inline POINT InputManager<EnumT>::GetPrevPos() const
		{
			return m_prevPosition;
		}
		template<typename EnumT>
		inline bool InputManager<EnumT>::IsLock()
		{
			return m_locked;
		}
		template<typename EnumT>
		inline void InputManager<EnumT>::LockCursor(HWND hWnd)
		{
			if (m_locked == false)
			{
				ShowCursor(FALSE);
				ShowCursor(FALSE);
			}
			m_locked = true;
			m_hwnd = hWnd;
		
			m_hOldCursor = GetCursor();
			SetCursor(NULL);
			if (m_locked && m_hwnd != NULL)
			{
				if (GetActiveWindow() != NULL)
				{
					RECT rc{};
					GetClientRect(m_hwnd, &rc);
					POINT pt{
						(rc.right - rc.left) / 2,
						(rc.bottom - rc.top) / 2
					};
					ClientToScreen(m_hwnd, &pt);
					SetCursorPos(pt.x, pt.y);
					m_cursorPosition = pt;
				}
			}
			//GetCursorPos(&m_cursorPosition);

		}
		template<typename EnumT>
		inline void InputManager<EnumT>::UnlockCursor()
		{
			if (m_locked == true)
			{
				ShowCursor(TRUE);
			}
			m_locked = false;
			SetCursor(m_hOldCursor);
			m_hwnd = NULL;
		}
}
}