#pragma once
#include<RenderComponent.hpp>
#include<game/runtime.hpp>
#include<GameRenderer.hpp>
namespace Kumazuma::Client
{
	class COMWeaponRender : public Graphics::RenderComponent
	{
	public:
		COMWeaponRender(
			std::shared_ptr<Graphics::GameRenderer> const& renderer, 
			std::wstring const& frameName,
			std::shared_ptr<RenderObject> const& mesh
			);
		auto Update(Game::UpdateEvent const& evt)->void;
		auto Render(RenderModule& renderModule, Graphics::GameRenderer& render)->void;
		auto Clone()const->Game::Component*;
		auto SetVisible(bool flag)->void;
		auto IsVisible()const->bool;
	private:
		std::shared_ptr<Graphics::GameRenderer> renderer_;
		std::shared_ptr<RenderObject> mesh_;
		std::wstring frameName_;
		bool visible_;
	};
}