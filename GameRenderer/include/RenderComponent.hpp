#pragma once
#include<game/component.hpp>
#include<common.hpp>
class RenderModule;
namespace Kumazuma::Graphics
{
	class GameRenderer;
	class DLL_CLASS RenderComponent : public Game::Component
	{
	DECLARE_COMPONENT_CLASS(Kumazuma::Graphics::RenderComponent)
	protected:
		RenderComponent();
	public:
		virtual auto Render(RenderModule& renderModule, GameRenderer& render)->void = 0;
		virtual auto PrepareRender(RenderModule& renderModule, GameRenderer& render)->void;
	};
}