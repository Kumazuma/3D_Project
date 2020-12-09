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
		property float Radius
		{
			auto get()->float;
			auto set(float value)->void;
		}
		
	protected:
	private:
		MapToolCore::Collider^ m_collider;
		System::ComponentModel::PropertyChangedEventHandler^ m_propertyChangedhandler;
		float m_radius;

	};
}