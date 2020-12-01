#pragma once
#include "MapToolRender.h"
#include "TextureCollection.h"
namespace MapToolRender
{
	public ref class BoxColliderMeshObject : public RenderObject
	{
	public:
		BoxColliderMeshObject(GraphicsDevice^ device);
	protected:
		BoxColliderMeshObject(BoxColliderMeshObject^ const& rhs);
	public:
		auto Clone()->RenderObject^ override;
		property MapToolCore::BoxCollider^ Collider
		{
			auto get()->MapToolCore::BoxCollider^;
			auto set(MapToolCore::BoxCollider^ )->void;
		}
	private:
		MapToolCore::BoxCollider^ m_collider;
	};
}