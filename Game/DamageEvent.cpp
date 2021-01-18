#include "DamageEvent.hpp"
namespace Kumazuma::Client
{
	DamageEvent::DamageEvent(const Game::EventTagBase& tag, Damage const& damage) :
		Event{ tag },
		damage_{damage}
	{
	}

	auto DamageEvent::GetDamage() const -> Damage const&
	{
		return damage_;
	}

}
