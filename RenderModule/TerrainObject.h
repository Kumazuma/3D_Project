#pragma once
#include"RenderObject.h"
#include"COMPtr.hpp"
#include<vector>
#include<memory>

class DLL_CLASS TerrainObject : public RenderObject
{
public:
	static auto Create(RenderModule* pRenderModule, u32 width, u32 height, f32 interval, f32 terrainMaxHeight, u8 const* pArray, TerrainObject** pObj)->HRESULT;
	
	auto Render(RenderModule* pRenderModule)->void override;
	auto Clone()const->RenderObject*;
	auto SetDiffuseTexture(IDirect3DTexture9* pTexture)->void;
	auto SetInterval(f32 value)->void;
	auto GetInterval()const->f32;
	auto SetMaxHeight(f32 value)->void;
	auto GetMaxHeight()const->f32;
protected:
	auto Initialize(RenderModule* pRenderModule, u32 width, u32 height, f32 interval, f32 terrainMaxHeight, u8 const* pArray)->HRESULT;
	TerrainObject();
	TerrainObject(TerrainObject const* rhs);

private:
	static constexpr unsigned long FVF = FVF_TEX;
	static constexpr size_t VERTEX_SIZE = sizeof(VERTEX<FVF>);
	static constexpr D3DFORMAT INDEX_TYPE = D3DFMT_INDEX32;
	static constexpr size_t INDEX_SIZE = sizeof(Index<INDEX_TYPE>);
	unsigned short m_fvf;
	u32 m_vertexCount;
	u32 m_indexCount;
	u32 m_width;
	u32 m_depth;
	f32 m_interval;
	f32 m_maxHeight;
	
	COMPtr<IDirect3DVertexBuffer9> m_pVertexBuffer;
	COMPtr<IDirect3DIndexBuffer9> m_pIndexBuffer;
	 COMPtr<IDirect3DTexture9>  m_pTexture;
	std::shared_ptr< std::vector<DirectX::XMFLOAT3A>> m_pVertexPositions;
	//Copied플래그, Copy on write를 구현하기 위한 변수
	bool m_copied;
};