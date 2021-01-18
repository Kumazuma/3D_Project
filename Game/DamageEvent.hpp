#pragma once
#include<game/event.hpp>
#include"typedef.hpp"
#include"Damage.hpp"
namespace Kumazuma::Client
{
	class DamageEvent : public Game::Event
	{
	public:
		DamageEvent(const Game::EventTagBase& tag, Damage const& damage);
		auto GetDamage()const->Damage const&;
	private:
		Damage damage_;
	};
}