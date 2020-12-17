#pragma once
#include"RenderObject.h"
#include"RenderEntity.h"
#include <memory>
#include "COMPtr.hpp"
#include <d3dx9.h>
#include <vector>
#include <unordered_map>
#include "hash_helper_for_f32x3.h"
class NavMeshEntity;

class DLL_CLASS NavMeshRenderingObject : public RenderObject
{
	friend class NavMeshEntity;
	using Entity = NavMeshEntity;
	static constexpr unsigned long FVF = FVF_NAVI;
	static constexpr D3DFORMAT INDEX_FMT = D3DFMT_INDEX16;
	using Triangle = Index<INDEX_FMT>;
	using Vertex = VERTEX<FVF>;
protected:
	NavMeshRenderingObject(RenderModule*);
	NavMeshRenderingObject(RenderModule* pRenderModule, DirectX::XMFLOAT4 const* vertices, u16 const* indices, u32 verticesCount, u32 indexCout );
	NavMeshRenderingObject(NavMeshRenderingObject const& rhs);
	NavMeshRenderingObject(NavMeshRenderingObject&& rhs)noexcept;
public:
	static auto Create(RenderModule* pRenderModule, NavMeshRenderingObject** pOut)->HRESULT;
	static auto Load(
		RenderModule* pRenderModule,
		DirectX::XMFLOAT4 const* vertices,
		u16 const* indices,
		u32 verticesCount,
		u32 indexCout,
		NavMeshRenderingObject** pOut)->HRESULT;
public:
	auto Clone()const->RenderObject*;
	auto PushPoint(DirectX::XMFLOAT3 const& pt, DirectX::XMFLOAT3 const& cameraAt, DirectX::XMFLOAT3 const& rotation)->void;
	auto GetVertexCount()const->u32;
	auto GetVertex(u32 index)const->DirectX::XMFLOAT3 const&;
	auto SetVertex(u32 index, DirectX::XMFLOAT3 const& position)->void;
	auto RemoveLastTriangle()->void;
	auto SetWeldDistance(f32 value)->void;
	auto GetWeldDistance()const->f32;
	auto PrepareRender(IRenderer* pRenderer)->void override;
	auto GetIndexCount()const->u32;
	auto GetVertices(DirectX::XMFLOAT4* const pBuffer, u32 bufferSize)->HRESULT;
	auto GetIndices(u16* const pBuffer, u32 bufferSize)->HRESULT;
private:
	f32 m_weldDistance;
	//조립 중인 포인트
	std::vector<u16> m_positionIndexing;
	std::vector<Vertex > m_vertices;
	std::vector<Triangle > m_triangles;
	std::shared_ptr<NavMeshEntity> m_entity;
};
class NavMeshEntity : public RenderEntity
{
public:
	NavMeshEntity(NavMeshRenderingObject* pObj);
	auto Render(RenderModule* pRenderModule, IRenderer* pRenderer)->void override;
	NavMeshRenderingObject* m_obj;
};