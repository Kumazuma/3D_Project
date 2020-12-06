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
		public:
			static std::shared_ptr<InputManager> Instance() {
				if (s_instance == nullptr)
				{
					s_instance.reset(new InputManager{});
				}
				return s_instance;
			}
			bool IsPressing(EnumT id)
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
			bool IsDown(EnumT id)
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
			bool IsUp(EnumT id)
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
			void Update()
			{
				m_press.clear();
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
			void Bind(EnumT id, unsigned key)
			{
				m_mappedKeyData[id] = key;
			}

		};
		template<typename EnumT>
		std::shared_ptr<InputManager<EnumT>> InputManager<EnumT>::s_instance = nullptr;
	}
}