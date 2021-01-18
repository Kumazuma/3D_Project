#pragma once
#include"typedef.hpp"
#include<string>
namespace Kumazuma::Client
{
	enum class DamageType
	{
		Normal,
		Fire,
		Ice,
		Arcane,
		Holy,
		Dark
	};
	struct Damage
	{
	public:
		Damage(
			std::wstring const& attacker,
			std::wstring const& skillName,
			DamageType damageType,
			i32 amount);
		std::wstring attacker;
		std::wstring skillName;
		DamageType damageType;
		i32 amount;
	};
}