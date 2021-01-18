#pragma once
#include<game/object.hpp>
#include"COMPlayerInput.hpp"
#include"typedef.hpp"
namespace Kumazuma::Client
{
	enum class PlayerBehaviorID:u32;
	class PlayerBehavior
	{
	public:
		virtual ~PlayerBehavior() = default;
		auto Update(Game::Object* object, COMPlayerInput* component, f32 timedelta)->void;
		auto Reset(Game::Object* object, COMPlayerInput* component, f32 timedelta)->void;

		auto SetState(PlayerBehaviorID id)->void;
		auto GetState()const->PlayerBehaviorID;
		auto GetObjectRef()->Game::Object&;
		auto GetComponentRef()->COMPlayerInput&;
	protected:
		virtual auto Update(f32 timedelta)->void = 0;
		virtual auto Reset(f32 timedelta)->void = 0;
	private:
		Game::Object*	object_;
		COMPlayerInput*	com_;
	};
}