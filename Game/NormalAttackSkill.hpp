#pragma once
#include"SkillMeta.hpp"
#include"Skill.hpp"
namespace Kumazuma::Client
{
	class NormalAttackSkill : public Skill
	{
	public:
		static auto Create(Game::Object& obj)->Skill*;
	public:
		auto Activate(Game::Object& obj)->void;

	};
}