#pragma once
#include"CameraComponent.hpp"
#include"typedef.hpp"
#include<game/TransformComponent.hpp>
namespace Kumazuma::Client
{
	struct CameraViewingState
	{
	public:
		virtual ~CameraViewingState() = default;
		virtual auto Reset(f32 timeDelta)->void = 0;
		auto Update(CameraComponent* camera, f32 timeDelta)->void;
	protected:
		virtual auto OnUpdate( f32 timeDelta)->void = 0;
	protected:
		auto SetViewSpace(f32x44 const& value)->void;
		auto GetViewSpace()const->f32x44 const&;
		auto GetDistance()const->f32;
		auto SetDistance(f32 value)->void;
		auto GetTransformComponent()->std::shared_ptr<Game::TransformComponent>;
		auto GetPlayerTransform()const->f32x44;
	private:
		CameraComponent* camera_;
	};
	
}