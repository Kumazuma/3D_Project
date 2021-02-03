#pragma once
#include<RenderComponent.hpp>
#include<SkinnedXMeshObject.h>
#include<game/runtime.hpp>
namespace Kumazuma::Client
{
	class COMSkinnedMeshRender : public Graphics::RenderComponent
	{
	public:
		COMSkinnedMeshRender(std::shared_ptr<Graphics::GameRenderer> const& renderer, std::shared_ptr<SkinnedXMeshObject> const& obj);
		auto Clone()const->Game::Component* override;
		auto Update(Game::UpdateEvent const& evt)->void;
		auto Render(RenderModule& renderModule, Graphics::GameRenderer& render)->void override;
		auto GetMesh()const->std::shared_ptr<SkinnedXMeshObject>;
		auto SetVisible(bool flag)->void;
		auto IsVisible()const->bool;
		auto GetRenderer()const->std::shared_ptr<Graphics::GameRenderer>;
	private:
		std::shared_ptr<Graphics::GameRenderer> renderer_;
		std::shared_ptr<SkinnedXMeshObject> skinnedMesh_;
		bool visible_;
	};
}