#pragma once
#include<game/event.hpp>
#include"DamageEvent.hpp"
namespace Kumazuma::Client
{
	extern Game::EventTag<DamageEvent> const EVT_Damage;
}