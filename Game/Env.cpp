#include "Env.hpp"
#include "UnicodeHelper.h"
#include <fstream>
std::unordered_map<std::wstring, nlohmann::json> Kumazuma::Client::Enviroment::s_table = {};
auto Kumazuma::Client::Enviroment::Initialize() -> void
{
	s_table.clear();
	std::ifstream fstream;
	fstream.open(L"./env.json");
	if (!fstream.is_open())
	{
		return;
	}
	nlohmann::json file{ nlohmann::json::parse(fstream) };
	for (auto& it : file.items())
	{
		std::wstring key{ ConvertUTF8ToWide(it.key()) };
		s_table.emplace(std::move(key), it.value() );
	}
}

auto Kumazuma::Client::Enviroment::Release() -> void
{
	s_table.clear();
}

auto Kumazuma::Client::Enviroment::_GetValue(std::wstring const& key) ->std::unordered_map<std::wstring, nlohmann::json>::const_iterator
{
	return s_table.find(key);
}
