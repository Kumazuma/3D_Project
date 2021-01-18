#pragma once
#include<memory>
#include<game/object.hpp>
#include <DirectXMath.h>
#include"typedef.hpp"
namespace Kumazuma::Client
{
	auto SpawnRagnaros(f32x3 const& initPosition)->std::shared_ptr<Game::Object>;
}