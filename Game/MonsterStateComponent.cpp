#include "pch.h"
#include "COMMonsterState.hpp"
#include <game/TransformComponent.hpp>
#include "IMonsterState.hpp"
#include "Collider.hpp"
IMPLEMENT_COM_TAG(Kumazuma::MS2::COMMonsterState);
Kumazuma::MS2::COMMonsterState::COMMonsterState(MonsterStateId firstStateId, std::map<MonsterStateId, std::shared_ptr<IMonsterState> >&& states):
    Component(COMMonsterState::TAG),
    m_states{std::move(states)}
{
    m_pCurrentState = m_states[firstStateId];
    m_currentStateId = firstStateId;
}

Kumazuma::MS2::COMMonsterState::COMMonsterState():
    Component(COMMonsterState::TAG)
{
}

void Kumazuma::MS2::COMMonsterState::Initialize(MonsterStateId firstStateId, std::map<MonsterStateId, std::shared_ptr<IMonsterState>>&& states)
{
    m_states.swap(states);
    m_pCurrentState = m_states[firstStateId];
    m_currentStateId = firstStateId;
    Bind(Game::EVT_UPDATE, &COMMonsterState::Update);
}

void Kumazuma::MS2::COMMonsterState::Update(const Kumazuma::Game::UpdateEvent& event)
{
    if (m_pCurrentState != nullptr)
    {
        std::shared_ptr<Kumazuma::Game::Object> pObj{ GetObj().lock() };
        auto tmp{ m_pCurrentState };
        tmp->Update(this, event.GetDelta(), pObj);
        const auto pTransform = pObj->GetComponent(Game::TransformComponent::TAG);
        pObj->GetComponent(MS2::COMCollider::TAG)->SetPos(pTransform->GetPosition());
    }
}

bool Kumazuma::MS2::COMMonsterState::SetState(MonsterStateId stateId)
{
    const auto it = m_states.find(stateId);
    if (it != m_states.end())
    {
        m_pCurrentState = it->second;
        m_currentStateId = stateId;
        return true;
    }
    return false;
}

Kumazuma::Game::Component* Kumazuma::MS2::COMMonsterState::Clone() const
{
    std::map<MonsterStateId, std::shared_ptr<IMonsterState> > states;
    for (auto& it : m_states)
    {
        std::shared_ptr<IMonsterState> pNewState{ it.second->Clone() };
        states.emplace(it.first, std::move(pNewState));
    }
    return new COMMonsterState{ m_currentStateId, std::move( states ) };
}