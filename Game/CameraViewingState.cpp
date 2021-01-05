#include "CameraViewingState.hpp"
#include <game/TransformComponent.hpp>
#include <game/object.hpp>
using namespace DirectX;
auto Kumazuma::Client::CameraViewingState::SetViewSpace( f32x44 const& value) -> void
{
    camera_->viewSpace_ = value;
}

auto Kumazuma::Client::CameraViewingState::GetViewSpace() const -> f32x44 const&
{
    return camera_->viewSpace_;
}

auto Kumazuma::Client::CameraViewingState::GetDistance() const -> f32
{
    return camera_->distance_;
}

auto Kumazuma::Client::CameraViewingState::SetDistance(f32 value) -> void
{
    camera_->distance_ = value;
}

auto Kumazuma::Client::CameraViewingState::GetTransformComponent() -> std::shared_ptr<Game::TransformComponent>
{
    auto object{ camera_->GetObj().lock() };
    if (object != nullptr)
    {
        auto transformCOM{ object->GetComponent<Game::TransformComponent>() };
        return transformCOM;
    }
    return nullptr;
}

auto Kumazuma::Client::CameraViewingState::GetPlayerTransform() const -> f32x44
{
    auto object{ camera_->playerObj_.lock() };
    f32x44 transform{};
    for (i32 i = 0; i < 4; ++i)
    {
        transform(i, i) = 1;
    }
    if (object != nullptr)
    {
        auto transformCOM{ object->GetComponent<Game::TransformComponent>() };
        if (transformCOM != nullptr)
        {
            transformCOM->GenerateTransformMatrixWithoutScale(&transform);
        }
    }
    return transform;
}

auto Kumazuma::Client::CameraViewingState::Update(CameraComponent* camera, f32 timeDelta) -> void
{
    camera_ = camera;
    OnUpdate(timeDelta);
}