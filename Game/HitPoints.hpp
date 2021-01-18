#pragma once
#include "typedef.hpp"
namespace Kumazuma::Client
{
	class HitPoints
	{
	public:
		HitPoints(i32 max);
		HitPoints(i32 max_, i32 current_);
		auto GetCurrent()const->i32;
		auto GetMaximum()const->i32;
		auto Heal(i32 amount)->void;
		auto Damage(i32 amount)->void;
		auto SetMaximum(i32 amount, bool fillCurrent = true)->void;
		auto SetCurrent(i32 amount)->void;
	private:
		i32 current_;
		i32 max_;
	};

}