#pragma once
#include<game/object.hpp>
#include<memory>
namespace Kumazuma::Graphics
{
	class GameRenderer;
}
namespace Kumazuma::Client
{
	auto SpawnFinale(std::shared_ptr<Graphics::GameRenderer>const& renderer)->std::shared_ptr<Game::Object>;
}