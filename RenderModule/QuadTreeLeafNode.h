#pragma once
#include"TerrainObject.h"
#include"QuedNode.h"
class TerrainObject::LeafNode : public TerrainObject::QuadNode
{
public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="pRenderModule">렌더모듈</param>
	/// <param name="heightMap">높이맵의 비트맵(RGB 32bpp)</param>
	/// <param name="offsetWidth">폭의 오프셋</param>
	/// <param name="offsetDepth">깊이의 오프셋</param>
	/// <param name="width">해당 노드의 폭</param>
	/// <param name="depth">해당 노드의 깊이</param>
	/// <param name="terrainWidth">터레인의 폭</param>
	/// <param name="terrainDepth">터레인의 깊이</param>
	/// <param name="interval">터레인의 인터벌</param>
	/// <param name="maxHeight">터레인의 최대 높이</param>
	/// <returns></returns>
	static auto Create(
		RenderModule* pRenderModule,
		u8 const* heightMap,
		u32 offsetWidth,
		u32 offsetDepth,
		u32 width,
		u32 depth,
		u32 terrainWidth,
		u32 terrainDepth,
		f32 interval,
		f32 maxHeight)->LeafNode*;
	auto Render(TerrainObject* terrain, RenderModule* renderModule)->void;
protected:

	LeafNode(
		RenderModule* pRenderModule,
		u8 const* heightMap,
		u32 offsetWidth,
		u32 offsetDepth,
		u32 width,
		u32 depth,
		u32 terrainWidth,
		u32 terrainDepth,
		f32 interval,
		f32 maxHeight);
	
private:
	COMPtr<IDirect3DIndexBuffer9> m_indexBuffer;
	std::vector<Index<INDEX_TYPE> > m_indices;

};