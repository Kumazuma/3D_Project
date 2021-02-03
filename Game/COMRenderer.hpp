#pragma once
#include<game/component.hpp>
#include<GameRenderer.hpp>
namespace Kumazuma::Client
{
	class COMRenderer : public Game::Component
	{
	public:
		COMRenderer(std::shared_ptr<Graphics::GameRenderer> const& renderer);
		auto Clone()const->Game::Component*;
		auto GetRenderer()->std::shared_ptr<Graphics::GameRenderer>;
	private:
		std::shared_ptr<Graphics::GameRenderer> renderer_;
		DECLARE_COMPONENT_CLASS(Kumazuma::Client::COMRenderer)
	};
}