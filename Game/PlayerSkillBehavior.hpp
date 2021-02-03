#pragma once
#include "PlayerBehavior.hpp"
#include "Skill.hpp"
#include "FlameEffect.hpp"
#include <list>
namespace Kumazuma::Client
{
	class PlayerNormalAttackBehavior : public PlayerBehavior
	{
	protected:
		auto Update(f32 timedelta)->void;
		auto Reset(f32 timedelta)->void;
	private:
		std::unordered_set<Game::Object*> preventAttack_;
	};
	class PlayerSkill1Behavior : public PlayerBehavior
	{
	protected:
		auto Update(f32 timedelta)->void;
		auto Reset(f32 timedelta)->void;
	private:
	};
	class PlayerSkill2Behavior : public PlayerBehavior
	{
	public:
		PlayerSkill2Behavior();
	protected:
		auto Update(f32 timedelta)->void;
		auto Reset(f32 timedelta)->void;
	private:
		std::shared_ptr<FlameEffect> effect_;
		std::list< std::shared_ptr<FlameEffect> > effects_;
		f32 accumTime;
		std::unordered_set<Game::Object*> preventAttack_;
		Collider fireCollider_;
	};
	class PlayerSkill3Behavior : public PlayerBehavior
	{
	protected:
	private:
	};
}