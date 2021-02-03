#pragma once
#include<RenderComponent.hpp>
#include<WavefrontOBJMesh.h>
#include<game/runtime.hpp>
#include<GameRenderer.hpp>
namespace Kumazuma::Client
{
	class COMTerrainRender : public Graphics::RenderComponent
	{
	public:
		COMTerrainRender(std::shared_ptr<Graphics::GameRenderer> renderer, std::shared_ptr<WavefrontOBJMesh> const& mesh);
		auto Update(Game::UpdateEvent const& evt)->void;
		auto Render(RenderModule& renderModule, Graphics::GameRenderer& renderer)->void override;
		auto Clone()const->Game::Component* override;
	private:
		std::shared_ptr<Graphics::GameRenderer> renderer_;
		std::shared_ptr<WavefrontOBJMesh> mesh_;
	};
}