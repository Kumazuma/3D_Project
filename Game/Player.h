#pragma once
#include<game/component.hpp>
#include<game/runtime.hpp>
#include<game/object.hpp>
namespace Kumazuma::Client
{
	auto SpawnPlayer()->std::shared_ptr<Game::Object>;
}