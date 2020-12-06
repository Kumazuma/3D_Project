#pragma once
#include<string>
#include<unordered_map>
#include"json.hpp"
#include"UnicodeHelper.h"
namespace Kumazuma
{
	namespace Client
	{
		class Enviroment
		{
		public:
			static constexpr wchar_t BASE_DIR[] = L"BASE_DIR";
			static auto Initialize()->void;
			static auto Release()->void;
			template<typename T>
			static auto GetValue(std::wstring const& key)->T;
			template<>
			static auto GetValue<std::wstring>(std::wstring const& key)->std::wstring;
		protected:
			static auto _GetValue(std::wstring const& key)->std::unordered_map<std::wstring, nlohmann::json>::const_iterator;
		private:
			static std::unordered_map<std::wstring, nlohmann::json> s_table;
		};
		template<typename T>
		inline auto Enviroment::GetValue(std::wstring const& key) -> T
		{
			auto s = _GetValue(key);
			return static_cast<T>(s->second);
		}
		template<>
		inline auto Enviroment::GetValue<std::wstring>(std::wstring const& key) -> std::wstring
		{
			auto s = _GetValue(key);
			return ConvertUTF8ToWide(s->second);
		}
	}
}