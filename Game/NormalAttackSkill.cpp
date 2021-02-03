#include "NormalAttackSkill.hpp"

auto Kumazuma::Client::NormalAttackSkill::Create(Game::Object& obj) -> Skill*
{
    return new NormalAttackSkill{};
}

auto Kumazuma::Client::NormalAttackSkill::Activate(Game::Object& obj) -> void
{

}
