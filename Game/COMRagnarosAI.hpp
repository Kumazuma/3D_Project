#pragma once
#include<game/component.hpp>
namespace Kumazuma::Game
{
	class UpdateEvent;
}
namespace Kumazuma::Client
{
	class COMRagnarosAI : public Game::Component
	{
	public:
		static const Game::ComponentTag<COMRagnarosAI> TAG;
		COMRagnarosAI();
	public:
		auto Clone()const->Game::Component*;
		auto Update(Game::UpdateEvent const& event)->void;
	private:
	};
}