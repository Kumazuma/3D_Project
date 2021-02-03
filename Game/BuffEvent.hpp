#pragma once
#include<game/event.hpp>
#include"typedef.hpp"
#include<string>
namespace Kumazuma::Client
{
	class BuffEvent:public Game::Event
	{
	public:
		BuffEvent(Game::EventTagBase const& tag, std::wstring const& skillID);
		auto GetSkillID()const->std::wstring const&;
	private:
		std::wstring skillID_;
	};
}