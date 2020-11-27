#pragma once
#include"TerrainObject.h"
#include<array>
#include<memory>
class TerrainObject::QuadNode
{
public:
	enum class Children { LeftTop = 0, LeftBottom = 1, RightTop = 2, RightBottom = 3 };
	enum class Kind { Branch, Leaf };
protected:
	QuadNode(Kind kind);
public:
	static auto GenerateQuadTree(
		RenderModule* pRenderModule,
		u8 const* heightMap,
		u32 offsetWidth,
		u32 offsetDepth,
		u32 width,
		u32 depth,
		u32 terrainWidth,
		u32 terrainDepth,
		f32 interval,
		f32 maxHeight)->std::unique_ptr<QuadNode>;
	auto GetKind()const->Kind;
	auto GetBoundBoxPoints()const->std::array<DirectX::XMFLOAT3A, 8> const&;
	auto GetRadius()const->f32;
	auto GetCenter()const->DirectX::XMFLOAT3A const&;
public:
	virtual ~QuadNode() = default;
	virtual auto Render(TerrainObject * terrain, RenderModule * renderModule)->void = 0;
private:
	Kind m_kind;
protected:
	bool m_inFrustum;
	std::array<DirectX::XMFLOAT3A, 8> m_boundBoxPoint;
	f32 m_radius;
	DirectX::XMFLOAT3A m_center;
};
class TerrainObject::BranchNode: public TerrainObject::QuadNode
{
public:

private:
	std::array<std::shared_ptr<QuadNode>, 4> m_children;
};
