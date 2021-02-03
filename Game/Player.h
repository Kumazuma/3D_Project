#pragma once
#include<game/component.hpp>
#include<game/runtime.hpp>
#include<game/object.hpp>
#include<DirectXMath.h>
#include<GameRenderer.hpp>
namespace Kumazuma::Client
{
	auto SpawnPlayer(std::shared_ptr<Graphics::GameRenderer> const& renderer, DirectX::XMFLOAT3 const& initPosition)->std::shared_ptr<Game::Object>;
}