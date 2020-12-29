#pragma once
#include<game/component.hpp>
#include<game/runtime.hpp>
#include"SkinnedXMeshObject.h"
namespace Kumazuma::Client
{
	class COMPlayerInput : public Game::Component
	{
	public:
		static const Game::ComponentTag<COMPlayerInput> TAG;
		COMPlayerInput();
	public:
		auto Clone()const->Game::Component*;
		auto Update(Game::UpdateEvent const& event)->void;
	private:
	};
}