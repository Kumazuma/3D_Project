#pragma once
#include<game/component.hpp>
#include"typedef.hpp"
#include"typedef.hpp"
namespace Kumazuma::Client
{
	class BuffEvent;
	class COMSkills : public Game::Component
	{
	public:
		COMSkills();
		auto Clone()const->Game::Component*;
		auto UpdateSkillInfo(f32 t)->void;
		auto OnSkillBuff(BuffEvent const& evt)->void;
		auto GetAttackSpeed()const->f32;
	private:
		f32 buffDuration_;
		f32 attackSpeed_;
	DECLARE_COMPONENT_CLASS(Kumazuma::Client::COMSkills)
	};
}
