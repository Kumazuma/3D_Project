#include "CameraComponent.hpp"
#include <Windows.h>
#include <game/object.hpp>
#include <game/TransformComponent.hpp>
#include "LookForwardState.hpp"
#include "FreeViewingState.hpp"
#include "IdleViewingState.hpp"
#include "framework.h"
using namespace Kumazuma::Client;
using namespace Kumazuma::Game;
using namespace DirectX;
const ComponentTag< CameraComponent> CameraComponent::TAG{"CameraComponent"};
constexpr StringLiteral<char> STATE_CHASING_PLAYER_LOOK{"STATE_CHASING_PLAYER_LOOK"};
constexpr StringLiteral<char> STATE_FREE_VIEWING{ "STATE_FREE_VIEWING" };
constexpr StringLiteral<char> STATE_IDLE{"STATE_IDLE"};

Kumazuma::Client::CameraComponent::CameraComponent(std::shared_ptr<Game::Object const> player):
    Component{ TAG },
    distance_{ 40.f },
    playerObj_{ player },
    viewSpace_{}
{
    Bind(EVT_UPDATE, &CameraComponent::Update);
    //currentState_
    //m_pCurrentState = std::shared_ptr<CameraViewingState>(new LookForwardState{});
    viewingStates_.emplace(STATE_IDLE, new IdleViewingState{});
    viewingStates_.emplace(STATE_FREE_VIEWING, new FreeViewingState{});
    viewingStates_.emplace(STATE_CHASING_PLAYER_LOOK, new LookForwardState{});
    currentState_ = viewingStates_[STATE_IDLE];
}

auto Kumazuma::Client::CameraComponent::Clone() const -> Game::Component*
{
    return nullptr;
}

auto Kumazuma::Client::CameraComponent::Update(Game::UpdateEvent const& event) -> void
{
    auto obj{ GetObj().lock() };
    auto state{ currentState_ };
    state->Update(this, event.GetDelta());
    auto ptr{ InputManager::Instance() };
    auto const lButtonPressing{ ptr->IsPressing(PLAYER_INPUT::MOUSE_LBUTTON) };
    auto const rButtonPressing{ ptr->IsPressing(PLAYER_INPUT::MOUSE_RBUTTON) };
    if (lButtonPressing )
    {
        state = viewingStates_[STATE_FREE_VIEWING];
    }
    else if (rButtonPressing)
    {
        state = viewingStates_[STATE_CHASING_PLAYER_LOOK];
    }
    else
    {
        state = viewingStates_[STATE_IDLE];
    }
    
    if (state != currentState_)
    {
        currentState_ = state;
        currentState_->Reset(event.GetDelta());
    }
}

auto Kumazuma::Client::CameraComponent::GetViewMatrix() const -> f32x44 const&
{
    return viewSpace_;
}
