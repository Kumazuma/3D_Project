#pragma once
#include<RenderComponent.hpp>
#include<SkinnedXMeshObject.h>
#include<game/runtime.hpp>
#include<SimpleColliderBoxObject.h>
#include<SimpleColliderSphareObject.h>
#include<game/TransformComponent.hpp>
#include"typedef.hpp"
namespace Kumazuma::Client
{
	class COMColliderRender : public Graphics::RenderComponent
	{
	public:
		COMColliderRender(std::shared_ptr<Graphics::GameRenderer> const& renderer, std::shared_ptr<SkinnedXMeshObject> const& skinnedMesh, std::shared_ptr<Game::Object> const& object  );
		auto Update(Game::UpdateEvent const& evt)->void;
		auto Render(RenderModule& renderModule, Graphics::GameRenderer& render)->void;
		auto RenderColliders(IDirect3DDevice9* device, RenderModule& renderModule, Graphics::GameRenderer& render, std::shared_ptr<Game::Object> object, f32x44 const& parentMatrix)->void;
		auto Clone()const->Game::Component*;
	
	private:
		std::shared_ptr<Graphics::GameRenderer> renderer_;
		std::weak_ptr<Game::Object> object_;
		std::shared_ptr<SkinnedXMeshObject> skinnedMesh_;
		std::shared_ptr<SimpleBoxObject> boxMesh_;
		std::shared_ptr<SimpleSphereObject> sphereMesh_;
	private://static
		static std::weak_ptr<SimpleBoxObject> s_boxMesh;
		static std::weak_ptr<SimpleSphereObject> s_sphereMesh;
		
	};
}