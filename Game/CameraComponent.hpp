#pragma once
#include<game/component.hpp>
#include<game/runtime.hpp>
#include"typedef.hpp"
#include<StringLiteral.h>
namespace Kumazuma
{
	namespace Client
	{
		struct CameraViewingState;
		class CameraComponent : public Game::Component
		{
			friend struct CameraViewingState;
		public:
			CameraComponent(std::shared_ptr<Game::Object const> player);
		public:
			auto Clone()const->Game::Component*;
			auto Update(Game::UpdateEvent const& event)->void;
			auto GetViewMatrix()const->f32x44 const&;
		private:
			f32x44 viewSpace_;
			f32 distance_;
			std::weak_ptr<Game::Object const> playerObj_;
			std::shared_ptr<CameraViewingState> currentState_;
			std::unordered_map<StringLiteral<char>, std::shared_ptr<CameraViewingState> > viewingStates_;

			DECLARE_COMPONENT_CLASS(Kumazuma::Client::CameraComponent)
		};
	}
}