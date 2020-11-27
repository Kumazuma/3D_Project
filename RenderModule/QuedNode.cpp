#include "pch.h"
#include "QuedNode.h"
#include"RenderModule.h"
#include "QuadTreeLeafNode.h"
TerrainObject::QuadNode::QuadNode(Kind kind):
	m_kind{kind}
{
}
auto TerrainObject::QuadNode::GenerateQuadTree(
	RenderModule* pRenderModule,
	u8 const* heightMap,
	u32 offsetWidth,
	u32 offsetDepth,
	u32 width,
	u32 depth,
	u32 terrainWidth,
	u32 terrainDepth,
	f32 interval,
	f32 maxHeight) -> std::unique_ptr<QuadNode>
{
	f32 size{depth * interval * width * interval};
	if (size <= interval * interval)
	{
		return std::unique_ptr<QuadNode>{LeafNode::Create(pRenderModule, heightMap, offsetWidth, offsetDepth, width, depth, terrainWidth, terrainDepth, interval, maxHeight)};
	}
	else//4분할 한다.
	{

	}
}

auto TerrainObject::QuadNode::GetKind() const -> Kind
{
	return m_kind;
}

auto TerrainObject::QuadNode::GetBoundBoxPoints() const->std::array<DirectX::XMFLOAT3A, 8> const&
{
	return m_boundBoxPoint;
}

auto TerrainObject::QuadNode::GetRadius() const -> f32
{
	return m_radius;
}

auto TerrainObject::QuadNode::GetCenter() const -> DirectX::XMFLOAT3A const&
{
	return m_center;
}
