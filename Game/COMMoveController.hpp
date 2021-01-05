#pragma once
#include<game/component.hpp>

#include"typedef.hpp"

namespace Kumazuma::Game
{
	class UpdateEvent;
}
namespace Kumazuma::Client
{
	class PhisicsManager;
	class COMMoveController : public Game::Component
	{
	public:
		static Game::ComponentTag<COMMoveController> const TAG;
		COMMoveController(f32 speed);

		auto Move(f32 timeDelta, DirectX::XMVECTOR dir)->void;
		auto Jump(f32 timeDelta)->void;
		auto Clone()const->Game::Component*;
	protected:

	private:
		f32 speed_;
	};
}