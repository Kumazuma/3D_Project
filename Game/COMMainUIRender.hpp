#pragma once
#include<RenderComponent.hpp>
#include"HitPoints.hpp"
#include<d3d9.h>
namespace Kumazuma::Game
{
	class UpdateEvent;
}
namespace Kumazuma::Graphics
{
	class GameRenderer;
	class SimplePlane;
}
namespace Kumazuma::Client
{
	
	class COMMainUIRender :public Graphics::RenderComponent
	{
	public:
		COMMainUIRender(std::shared_ptr<Graphics::GameRenderer> const& renderer, std::shared_ptr<Game::Object> const& targetObject);
		auto Update(Game::UpdateEvent const& evt)->void;
		auto PrepareRender(RenderModule& renderModule, Graphics::GameRenderer& render)->void override;
		auto Render(RenderModule& renderModule, Graphics::GameRenderer& render)->void;
		auto Clone()const->Game::Component*;
	private:
		HitPoints hitPoints_;
		std::unique_ptr<Graphics::SimplePlane> plane_;
		std::shared_ptr<Graphics::GameRenderer> renderer_;
		std::weak_ptr<Game::Object> targetObject_;
		COMPtr<IDirect3DTexture9> texture1_;
		COMPtr<IDirect3DTexture9> texture2_;
	};
}