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
	NavMeshRenderingObject(NavMeshRenderingObject const& rhs);
	NavMeshRenderingObject(NavMeshRenderingObject&& rhs)noexcept;
public:
	static auto Create(RenderModule* pRenderModule, NavMeshRenderingObject** pOut)->HRESULT;
public:
	auto Clone()const->RenderObject*;
	auto PushPoint(DirectX::XMFLOAT3 const& pt, DirectX::XMFLOAT3 const& cameraAt, DirectX::XMFLOAT3 const& rotation);
	auto PrepareRender(RenderModule* pRenderModule)->void override;
private:
	//조립 중인 포인트
	std::vector<u16> m_positionIndexing;
	std::unordered_map<DirectX::XMFLOAT3, size_t> m_table;
	std::vector<Vertex > m_vertices;
	std::vector<Triangle > m_triangles;
	std::shared_ptr<NavMeshEntity> m_entity;
};
class NavMeshEntity : public RenderEntity
{
public:
	NavMeshEntity(NavMeshRenderingObject* pObj);
	auto Render(RenderModule* pRenderModule)->void override;
	NavMeshRenderingObject* m_obj;
};