#pragma once
#include<game/component.hpp>
#include<game/runtime.hpp>
namespace Kumazuma::Client
{
	class COMRagnarosDeadController : public Game::Component
	{
	public:
		COMRagnarosDeadController();
		auto Clone()const->Game::Component*;
		auto Update(Game::UpdateEvent const& evt)->void;
	protected:

	DECLARE_COMPONENT_CLASS(Kumazuma::Client::COMRagnarosDeadController)
	};
}