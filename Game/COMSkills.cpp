#include "COMSkills.hpp"
#include "EventTag.hpp"
IMPLEMENT_COMPONENT_CLASS(Kumazuma::Client::COMSkills)


Kumazuma::Client::COMSkills::COMSkills():
    Component{ TAG },
    attackSpeed_{1.f}
{
    Bind(EVT_Buff, &COMSkills::OnSkillBuff);
}

auto Kumazuma::Client::COMSkills::Clone() const -> Game::Component*
{
    return nullptr;
}

auto Kumazuma::Client::COMSkills::UpdateSkillInfo(f32 t) -> void
{
    if (buffDuration_ > 0.f)
    {
        buffDuration_ -= t;
        if (buffDuration_ <= 0.f)
        {
            buffDuration_ = 0.f;
            attackSpeed_ = 1.f;
        }
    }
}

auto Kumazuma::Client::COMSkills::OnSkillBuff(BuffEvent const& evt) -> void
{
    auto const& skillID{ evt.GetSkillID() };
    if (buffDuration_ <= 0.f)
    {
        buffDuration_ = 5.f;
        attackSpeed_ = 2.0f;
    }
}

auto Kumazuma::Client::COMSkills::GetAttackSpeed() const -> f32
{
    return attackSpeed_;
}
