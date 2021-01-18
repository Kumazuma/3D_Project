#include "HitPoints.hpp"
namespace Kumazuma::Client
{
	HitPoints::HitPoints(i32 max):
		max_{ max },
		current_{ max }
	{
	}
	HitPoints::HitPoints(i32 max_, i32 current_):
		max_{max_},
		current_{current_}
	{
	}
	auto HitPoints::GetCurrent() const -> i32
	{
		return current_;
	}
	auto HitPoints::GetMaximum() const -> i32
	{
		return max_;
	}
	auto HitPoints::Heal(i32 amount) -> void
	{
		i32 current{ current_ + amount };
		//¿À¹öÈú ¹æÁö
		if (current > max_)
		{
			current = max_;
		}
		current_ = current;
	}
	auto HitPoints::Damage(i32 amount) -> void
	{
		current_ -= amount;
	}
	auto HitPoints::SetMaximum(i32 amount, bool fillCurrent) -> void
	{
		max_ = amount;
		if (fillCurrent || max_ < current_)
		{
			current_ = max_;
		}
	}
	auto HitPoints::SetCurrent(i32 amount) -> void
	{
		current_ = amount;
		if (max_ < current_)
		{
			current_ = max_;
		}
	}
}

