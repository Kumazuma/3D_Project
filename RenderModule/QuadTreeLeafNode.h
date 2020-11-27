#pragma once
#include"TerrainObject.h"
#include"QuedNode.h"
class TerrainObject::LeafNode : public TerrainObject::QuadNode
{
public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="pRenderModule">�������</param>
	/// <param name="heightMap">���̸��� ��Ʈ��(RGB 32bpp)</param>
	/// <param name="offsetWidth">���� ������</param>
	/// <param name="offsetDepth">������ ������</param>
	/// <param name="width">�ش� ����� ��</param>
	/// <param name="depth">�ش� ����� ����</param>
	/// <param name="terrainWidth">�ͷ����� ��</param>
	/// <param name="terrainDepth">�ͷ����� ����</param>
	/// <param name="interval">�ͷ����� ���͹�</param>
	/// <param name="maxHeight">�ͷ����� �ִ� ����</param>
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