#pragma once
#include "MapToolRender.h"
#include "TextureCollection.h"
#include "CollderRenderObject.h"
namespace MapToolRender
{
	public ref class BoxColliderMeshObject : public ColliderRenderObject
	{
	public:
		BoxColliderMeshObject(GraphicsDevice^ device);
	protected:
		BoxColliderMeshObject(BoxColliderMeshObject^ const& rhs);
	public:
		~BoxColliderMeshObject();
		!BoxColliderMeshObject();
	public:
		auto Clone()->RenderObject^ override;
		auto SetFrameMatrix(SkinnedXMeshObj^ skinnedMesh, String^ frameName)->void override;
		auto SetAttribute(MapToolCore::ColliderAttribute^ attribute)->void override;
		property float Width
		{
			auto get()->float;
			auto set(float value)->void;
		}
		property float Height
		{
			auto get()->float;
			auto set(float value)->void;
		}
		property float Depth
		{
			auto get()->float;
			auto set(float value)->void;
		}
		property MapToolCore::Offset Offset
		{
			auto get()->MapToolCore::Offset override;
			auto set(MapToolCore::Offset offset)->void override;
		}
	protected:
		auto UpdateTransform()->void override;
	private:
		DirectX::XMFLOAT4X4* m_pParentMatrix;
		System::ComponentModel::PropertyChangedEventHandler^ m_propertyChangedhandler;
		float m_width;
		float m_height;
		float m_depth;
		MapToolCore::Offset m_offset;
	};
}