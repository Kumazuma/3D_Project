#pragma once
#include<game/component.hpp>
#include<game/runtime.hpp>
namespace Kumazuma
{
	namespace Client
	{
		class CameraComponent : public Game::Component
		{
		public:
			static const Game::ComponentTag<CameraComponent> TAG;
			CameraComponent();
		public:
			auto Clone()const->Game::Component*;
			auto Update(Game::UpdateEvent const& event)->void;
		};
	}
}