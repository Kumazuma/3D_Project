#pragma once
#include<string>
#include"typedef.hpp"
#include<functional>
#include<game/object.hpp>
namespace Kumazuma::Client
{
	enum class SkillType {
		Passive,
		Active,
	};
	class Skill;
	class SkillMeta
	{
	protected:
		SkillMeta(SkillType type, std::wstring const& name, std::wstring const& description);
	public:
		virtual ~SkillMeta() = default;
		auto GetName()const->std::wstring const&;
		auto GetDescription()const->std::wstring const&;
		auto GetType()const->SkillType;
	private:
		std::wstring name_;
		std::wstring description_;
		SkillType type_;
	};
	class ActiveSkillMeta : public SkillMeta
	{
	public:
		ActiveSkillMeta(std::wstring const& name, std::wstring const& description, f32 cooltime, std::function<Skill* (Game::Object&)> const& factory);
	public:
		auto GetCoolTime()const->f32;
		auto CreateSkill(Game::Object& obj)const->Skill*;
	private:
		f32 cooltime_;
		std::function<Skill* (Game::Object&)> factory_;
	};

}