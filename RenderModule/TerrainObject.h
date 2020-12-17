#pragma once
#include"RenderEntity.h"

#include"RenderObject.h"
#include"COMPtr.hpp"
#include<vector>
#include<memory>
#include<array>
class DLL_CLASS TerrainObject : public RenderObject
{
	class QuadNode;
	class LeafNode;
	class BranchNode;
	class Entity : public RenderEntity
	{
	public:
		Entity(TerrainObject* m_pTerrain);
		auto Render(RenderModule* pRenderModule, IRenderer* pRenderer)->void override;
	private:
		TerrainObject* m_pTerrain;
	};
public:
	static auto Create(RenderModule* pRenderModule, u32 width, u32 height, f32 interval, f32 terrainMaxHeight, u8 const* pArray, TerrainObject** pObj)->HRESULT;
	
	auto PrepareRender(IRenderer* pRenderer)->void override;
	auto Clone()const->RenderObject*;
	auto SetDiffuseTexture(IDirect3DTexture9* pTexture)->void;
	auto SetInterval(f32 value)->void;
	auto GetInterval()const->f32;
	auto SetMaxHeight(f32 value)->void;
	auto GetMaxHeight()const->f32;

protected:
	auto ResetTerrain(f32 newMaxHeight, f32 newInterval)->void;
	auto GenerateSubMeshBoundingBox()->void;
	auto Initialize(RenderModule* pRenderModule, u32 width, u32 height, f32 interval, f32 terrainMaxHeight, u8 const* pArray)->HRESULT;
	TerrainObject();
	TerrainObject(TerrainObject const* rhs);
private:
	static constexpr unsigned long FVF = FVF_TEX;
	static constexpr size_t VERTEX_SIZE = sizeof(VERTEX<FVF>);
	static constexpr D3DFORMAT INDEX_TYPE = D3DFMT_INDEX32;
	static constexpr size_t INDEX_SIZE = sizeof(Index<INDEX_TYPE>);
	static constexpr u32 ROW_COUNT = 8;
	static constexpr u32 COLS_COUNT = 8;
	unsigned short m_fvf;
	u32 m_vertexCount;
	u32 m_width;
	u32 m_depth;
	f32 m_interval;
	f32 m_maxHeight;
	u32 m_totalTriangleCount;
	COMPtr<IDirect3DVertexBuffer9> m_pVertexBuffer;
	std::array < COMPtr<IDirect3DIndexBuffer9>, ROW_COUNT* COLS_COUNT> m_pIndexBufferes;
	std::array<SIZE, ROW_COUNT* COLS_COUNT> m_triangleCounts;
	
	std::array<std::array<DirectX::XMFLOAT3A, 8>, ROW_COUNT* COLS_COUNT> m_subsetBoundingBoxes;
	
	COMPtr<IDirect3DTexture9>  m_pTexture;
	std::shared_ptr<Entity> m_entity;
	std::shared_ptr< std::vector<DirectX::XMFLOAT3A>> m_pVertexPositions;
	//Copied플래그, Copy on write를 구현하기 위한 변수
	bool m_copied;
};
