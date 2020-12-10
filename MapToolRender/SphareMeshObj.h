#pragma once
#include "MapToolRender.h"
#include "TextureCollection.h"
#include "CollderRenderObject.h"
#include <DirectXMath.h>
namespace DirectX
{
	struct XMMATRIX;
	struct XMFLOAT4X4;
}
namespace MapToolRender
{
	public ref class SphareMesh : public ColliderRenderObject
	{
	public:
		SphareMesh(GraphicsDevice^ device);
		~SphareMesh();
		!SphareMesh();
	protected:
		SphareMesh(SphareMesh^ const& rhs);
	public:
		auto Clone()->RenderObject^ override;
		auto SetFrameMatrix(SkinnedXMeshObj^ skinnedMesh, String^ frameName)->void override;
		auto SetAttribute(MapToolCore::ColliderAttribute^ attribute)->void override;
		property float Radius
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
		float m_radius;
		MapToolCore::Offset m_offset;

	};
}