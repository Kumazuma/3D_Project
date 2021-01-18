#include "COMCharacterStateInfo.hpp"
_BEGIN_NS(Kumazuma::Client)
IMPLEMENT_COMPONENT_CLASS(Kumazuma::Client::COMCharacterStateInfo)
COMCharacterStateInfo::COMCharacterStateInfo(HitPoints const& hitPoints, ManaPoints const& manaPoints):
    Component{ TAG },
    hitPoints_{ hitPoints },
    manaPoints_{manaPoints}
{
}

auto COMCharacterStateInfo::GetManaPoints() const -> ManaPoints const&
{
    return manaPoints_;
}

auto COMCharacterStateInfo::GetHitPoints() const -> HitPoints const&
{
    return hitPoints_;
}

auto COMCharacterStateInfo::GetManaPoints() -> ManaPoints&
{
    return manaPoints_;
}

auto COMCharacterStateInfo::GetHitPoints() -> HitPoints&
{
    return hitPoints_;
}

auto COMCharacterStateInfo::Clone() const -> Game::Component* 
{
    return new COMCharacterStateInfo{ *this };
}

_END_NS
