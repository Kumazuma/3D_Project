#pragma once
#include<memory>
#include<game/object.hpp>
#include <DirectXMath.h>
#include<GameRenderer.hpp>
#include"typedef.hpp"
namespace Kumazuma::Client
{
	auto SpawnRagnaros(std::shared_ptr<Graphics::GameRenderer> const& renderer, DirectX::XMFLOAT3 const& initPosition)->std::shared_ptr<Game::Object>;
}