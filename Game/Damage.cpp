#include "Damage.hpp"

Kumazuma::Client::Damage::Damage(
	std::wstring const& attacker,
	std::wstring const& skillName,
	DamageType damageType,
	i32 amount):
	attacker{attacker},
	skillName{skillName},
	damageType{damageType},
	amount{amount}
{
}
