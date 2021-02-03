#pragma once
#include<game/object.hpp>
namespace Kumazuma::Client
{
	class Skill
	{
	public:
		virtual ~Skill() = default;
		virtual auto Activate(Game::Object& object)->void = 0;
	};
}