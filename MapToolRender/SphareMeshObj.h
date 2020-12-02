#pragma once
#include "MapToolRender.h"
#include "TextureCollection.h"
namespace MapToolRender
{
	public ref class SphareMesh : public RenderObject
	{
	public:
		SphareMesh(GraphicsDevice^ device);
	protected:
		SphareMesh(SphareMesh^ const& rhs);
	public:
		auto Clone()->RenderObject^ override;
		property MapToolCore::SphareCollider^ Collider
		{
			auto get()->MapToolCore::SphareCollider^;
			auto set(MapToolCore::SphareCollider^)->void;
		}
	protected:
		auto OnColliderChangedChanged(System::Object^ sener, System::ComponentModel::PropertyChangedEventArgs^ e)->void;
		auto OnSelfChanged(System::Object^ sener, System::ComponentModel::PropertyChangedEventArgs^ e)->void;
	private:
		MapToolCore::SphareCollider^ m_collider;
		System::ComponentModel::PropertyChangedEventHandler^ m_propertyChangedhandler;
	};
}