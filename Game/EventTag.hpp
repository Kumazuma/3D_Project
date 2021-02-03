#pragma once
#include<game/event.hpp>
#include"DamageEvent.hpp"
#include"BuffEvent.hpp"
namespace Kumazuma::Client
{
	extern Game::EventTag<DamageEvent> const EVT_Damage;
	extern Game::EventTag<BuffEvent> const EVT_Buff;
}