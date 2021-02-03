#include "SkillMeta.hpp"

Kumazuma::Client::SkillMeta::SkillMeta(SkillType type, std::wstring const& name, std::wstring const& description):
    type_{type},
    name_{name},
    description_{description}
{

}


auto Kumazuma::Client::SkillMeta::GetName() const -> std::wstring const&
{
    return name_;
}

auto Kumazuma::Client::SkillMeta::GetDescription() const -> std::wstring const&
{
    return description_;
}

auto Kumazuma::Client::SkillMeta::GetType() const -> SkillType
{
    return type_;
}

Kumazuma::Client::ActiveSkillMeta::ActiveSkillMeta(
    std::wstring const& name,
    std::wstring const& description,
    f32 cooltime,
    std::function<Skill* (Game::Object&)> const& factory) :
    SkillMeta{ SkillType::Active, name, description },
    cooltime_{ cooltime },
    factory_{factory}
{
}

auto Kumazuma::Client::ActiveSkillMeta::GetCoolTime() const -> f32
{
    return cooltime_;
}

auto Kumazuma::Client::ActiveSkillMeta::CreateSkill(Game::Object& obj) const -> Skill*
{
    return factory_(obj);
}
