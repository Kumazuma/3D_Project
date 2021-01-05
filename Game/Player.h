#pragma once
#include<game/component.hpp>
#include<game/runtime.hpp>
#include<game/object.hpp>
#include<DirectXMath.h>
namespace Kumazuma::Client
{
	auto SpawnPlayer(DirectX::XMFLOAT3 const& initPosition)->std::shared_ptr<Game::Object>;
}