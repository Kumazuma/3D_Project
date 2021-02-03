#pragma once
#include<DirectXMath.h>
#include<common.hpp>
class RenderModule;
namespace Kumazuma::Graphics
{
	class GameRenderer;
	class DLL_CLASS _declspec(novtable) Effect
	{
	public:
		virtual ~Effect() = default;
		virtual auto Render(RenderModule& renderModule, GameRenderer& render)->void = 0;
		virtual auto GetCenter(DirectX::XMFLOAT3 * out)const->void = 0;
	};
}