#include "pch.h"
#include "./include/game/component.hpp"
using namespace Kumazuma::Game;
const ComponentTag<Component> Kumazuma::Game::COM_ANY{ nullptr };
Component::Component(const ComTagBase& tag) :
    m_pTag{ &tag },
    m_parent{}
{

}
std::weak_ptr<Object> Component::GetObj()
{
    return m_parent;
}
std::weak_ptr<const Object> Component::GetObj() const
{
    return m_parent;
}

void Kumazuma::Game::Component::UnbindAll()
{
    m_handledEvent.clear();
    m_handlers.clear();
}

void Kumazuma::Game::Component::_Bind(const EventTagBase* tag, EventHandleMthod handler)
{
    m_handledEvent.insert(tag);
    m_handlers[tag] = handler;
}

void Component::SetObject(std::weak_ptr<Object> obj)
{
    m_parent = std::move(obj);
}

void Component::HandleEvent(Event* pEvent)
{
    auto it = m_handlers.find(&pEvent->GetTag());
    if (it != m_handlers.end())
    {
        (this->*it->second)(*pEvent);
    }
}
