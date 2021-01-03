#pragma once
#include<game/component.hpp>

#include"typedef.hpp"
namespace Kumazuma::Game
{
	class UpdateEvent;
}
namespace Kumazuma::Client
{
	class COMMoveController : public Game::Component
	{
	public:
		
		COMMoveController();

	protected:

	private:
		f32 m_speed;
		f32 m_mass;
	};
}