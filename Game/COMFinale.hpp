#pragma once
#include<game/component.hpp>
#include<game/runtime.hpp>
#include<RenderComponent.hpp>
#include "typedef.hpp"
namespace Kumazuma::Graphics
{
	class GameRenderer;
	class SimplePlane;
}
namespace Kumazuma::Client
{
	class COMFinale :public Game::Component
	{
	public:
		COMFinale(std::shared_ptr<Graphics::GameRenderer> const& renderer);
		auto Update(Game::UpdateEvent const& evt)->void;
		auto Clone()const->Game::Component*;
	private:
	DECLARE_COMPONENT_CLASS(Kumazuma::Client::COMFinale)
	};
	class COMFinaleRender : public Graphics::RenderComponent
	{
	public:
		COMFinaleRender(std::shared_ptr<Graphics::GameRenderer> const& renderer);
		auto Update(Game::UpdateEvent const& evt)->void;
		auto Render(RenderModule& renderModule, Graphics::GameRenderer& render)->void;
		auto Clone()const->Game::Component*;
	private:
		std::unique_ptr<Graphics::SimplePlane> simplePlane_;
		std::shared_ptr<Graphics::GameRenderer> renderer_;
		f32 timeAccum_;
	};
}