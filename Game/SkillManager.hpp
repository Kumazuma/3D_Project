#pragma once
#include<string>
#include<memory>
#include"typedef.hpp"
#include<game/object.hpp>
#include<optional>
#include<map>
#include<memory>
namespace Kumazuma::Client
{
	using SkillID = u64;
	class SkillMeta;
	class SkillManager final
	{
	private:
		SkillManager();
	public:
		static auto Intialize()->void;
		static auto Instance()->std::shared_ptr<SkillManager>;
		auto SetMeta(SkillID id, SkillMeta const& meta)->void;
		auto GetMeta(SkillID id)->std::optional<SkillMeta>;
	private:
		std::map<SkillID, SkillMeta> table_;
	private:
		static std::shared_ptr<SkillManager> s_instance;
	};

}