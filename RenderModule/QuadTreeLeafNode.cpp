#include"pch.h"
#include"QuadTreeLeafNode.h"
#include"RenderModule.h"

auto TerrainObject::LeafNode::Create(RenderModule* pRenderModule, u8 const* heightMap, u32 offsetWidth, u32 offsetDepth, u32 width, u32 depth, u32 terrainWidth, u32 terrainDepth, f32 interval, f32 maxHeight) -> LeafNode*
{
	return new LeafNode(pRenderModule, heightMap, offsetWidth, offsetDepth, width, depth, terrainWidth, terrainDepth, interval, maxHeight);
}

auto TerrainObject::LeafNode::Render(TerrainObject* terrain, RenderModule* renderModule) -> void
{
}

TerrainObject::LeafNode::LeafNode(
	RenderModule* pRenderModule,
	u8 const* heightMap,
	u32 offsetWidth,
	u32 offsetDepth,
	u32 width,
	u32 depth,
	u32 terrainWidth,
	u32 terrainDepth,
	f32 interval,
	f32 maxHeight):
	QuadNode{ Kind::Leaf }
{
	COMPtr<IDirect3DDevice9> pDevice;
	pRenderModule->GetDevice(&pDevice);
	pDevice->CreateIndexBuffer(
		TerrainObject::INDEX_SIZE * width * depth * 2,
		0,
		TerrainObject::INDEX_TYPE,
		D3DPOOL_DEFAULT,
		&m_indexBuffer,
		nullptr
	);
	Index<TerrainObject::INDEX_TYPE>* pIndices{};
	m_indexBuffer->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);
	u32 lastIdx{};
	for (u32 row = 0; row < depth; ++row)
	{
		for (u32 cols = 0; cols < width; ++cols)
		{
			u32 vertexIdx{ (row + offsetDepth) * terrainWidth + cols + offsetWidth };
			for (u32 k = 0; k < 2; ++k)
			{
				u32 idx{ lastIdx++ };

				pIndices[idx][0]= vertexIdx + terrainWidth ;
				pIndices[idx][1]= vertexIdx + terrainWidth * (!k?1:0) + 1;
				pIndices[idx][2]= vertexIdx + (!k ? 1 : 0);
			}
		}
	}
	m_indexBuffer->Unlock();
}
