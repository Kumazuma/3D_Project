#include "BuffEvent.hpp"

Kumazuma::Client::BuffEvent::BuffEvent(Game::EventTagBase const& tag, std::wstring const& skillID) :
    Event(tag) ,
    skillID_{ skillID }
{

}

auto Kumazuma::Client::BuffEvent::GetSkillID() const -> std::wstring const&
{
    return skillID_;
}
