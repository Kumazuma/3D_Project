#include"ManaPoints.hpp"
_BEGIN_NS(Kumazuma::Client)
ManaPoints::ManaPoints(i32 max):
	max_{ max },
	current_{ max }
{
}

ManaPoints::ManaPoints(i32 max_, i32 current_):
	max_{ max_ },
	current_{ current_ }
{
}

auto ManaPoints::GetCurrent() const -> i32
{
	return current_;
}

auto ManaPoints::GetMaximum() const -> i32
{
	return max_;
}

auto ManaPoints::Increase(i32 amount) -> void
{
	i32 current{ current_ + amount };
	if (current > max_)
	{
		current = max_;
	}
	current_ = current;
}

auto ManaPoints::Decrease(i32 amount) -> void
{
	i32 current{ current_ - amount };
	if (current < 0)
	{
		current = 0;
	}
	current_ = current;
}

auto ManaPoints::SetMaximum(i32 amount, bool fillCurrent) -> void
{
	max_ = amount;
	if (fillCurrent || max_ < current_)
	{
		current_ = max_;
	}
}
auto ManaPoints::SetCurrent(i32 amount) -> void
{
	current_ = amount;
	if (max_ < current_)
	{
		current_ = max_;
	}
}

auto Kumazuma::Client::ManaPoints::operator+=(i32 amount) -> ManaPoints&
{
	Increase(amount);
	return *this;
}

auto Kumazuma::Client::ManaPoints::operator-=(i32 amount) -> ManaPoints&
{
	Decrease(amount);
	return *this;
}

auto Kumazuma::Client::ManaPoints::operator+(i32 amount) const -> ManaPoints
{
	ManaPoints copy{ *this };
	copy += amount;
	return copy;
}

auto Kumazuma::Client::ManaPoints::operator-(i32 amount) const -> ManaPoints
{
	ManaPoints copy{ *this };
	copy -= amount;
	return copy;
}

_END_NS

