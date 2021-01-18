#pragma once
#include"typedef.hpp"
namespace Kumazuma::Client
{
	class ManaPoints
	{
	public:
		ManaPoints(i32 max);
		ManaPoints(i32 max_, i32 current_);
		auto GetCurrent()const->i32;
		auto GetMaximum()const->i32;
		auto Increase(i32 amount)->void;
		auto Decrease(i32 amount)->void;
		auto SetMaximum(i32 amount, bool fillCurrent = true)->void;
		auto SetCurrent(i32 amount)->void;
		auto operator += (i32 amount)->ManaPoints&;
		auto operator -= (i32 amount)->ManaPoints&;
		auto operator + (i32 amount)const->ManaPoints;
		auto operator - (i32 amount)const->ManaPoints;

	private:
		i32 current_;
		i32 max_;
		/*
		
		*/
	};
}