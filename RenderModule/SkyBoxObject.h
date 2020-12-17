#pragma once
#include"typedef.hpp"
#include"RenderEntity.h"

#include"RenderObject.h"
#include"COMPtr.hpp"
#include<memory>
class SkyBoxEntity;
class DLL_CLASS SkyBoxObject : public RenderObject
{
	friend class SkyBoxEntity;
public:
	using Entity = SkyBoxEntity;
	static auto Create(RenderModule* pRenderModule, SkyBoxObject** pObj)->HRESULT;

	auto PrepareRender(IRenderer* pRenderer)->void override;
	auto Clone()const->RenderObject*;
	auto SetDiffuseTexture(IDirect3DCubeTexture9* pTexture)->void;
protected:
	auto Initialize(RenderModule* pRenderModule)->HRESULT;
	SkyBoxObject();
	SkyBoxObject(SkyBoxObject const* rhs);
private:
	std::shared_ptr< SkyBoxEntity> m_entity;
	static constexpr unsigned long FVF = FVF_TEX3D;
	static constexpr size_t VERTEX_SIZE = sizeof(VERTEX<FVF>);
	static constexpr D3DFORMAT INDEX_TYPE = D3DFMT_INDEX16;
	static constexpr size_t INDEX_SIZE = sizeof(Index<INDEX_TYPE>);
	u32 m_vertexCount;
	u32 m_indexCount;
	u32 m_width;
	u32 m_depth;
	f32 m_interval;
	f32 m_maxHeight;
	COMPtr<IDirect3DVertexBuffer9> m_pVertexBuffer;
	COMPtr<IDirect3DIndexBuffer9> m_pIndexBuffer;
	COMPtr<IDirect3DCubeTexture9>  m_pTexture;
};
class DLL_CLASS SkyBoxEntity : public RenderEntity
{
public:
	SkyBoxEntity(SkyBoxObject* m_pTerrain);
	auto Render(RenderModule* pRenderModule, IRenderer* pRenderer)->void override;
private:
	SkyBoxObject* m_pObject;
};