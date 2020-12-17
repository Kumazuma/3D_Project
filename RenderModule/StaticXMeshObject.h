#pragma once
#include"RenderEntity.h"

#include"RenderObject.h"
#include"COMPtr.hpp"
#include<vector>
#include<memory>
#include<string>
#include<d3dx9.h>
#include "MeshObject.h"
class StaticXMeshObjectSubset;
class DLL_CLASS StaticXMeshObject : public XMeshObject
{
	friend class StaticXMeshObjectSubset;
	using Entity = StaticXMeshObjectSubset;
protected:
	StaticXMeshObject();
	StaticXMeshObject(StaticXMeshObject const& rhs);
	auto Initialize(RenderModule* pRenderModule, std::wstring const& filePath)->HRESULT;
public:
	static auto Create(RenderModule* pRenderModule, std::wstring const& filePath, StaticXMeshObject** pOut)->HRESULT;
	auto PrepareRender(IRenderer* pRenderer)->void override;
	auto GetMaterialCount()const->u32;
	auto Clone()const->RenderObject*;
	//특정 서브셋의 알파값을 정의한다.
	auto SetEnableSubsetAlpha(u32 idx, bool enable = true)->void;
private:
	COMPtr<ID3DXMesh> m_pMesh;// 노말 정보를 삽입하여 변환시킨 메쉬 컴객체
	COMPtr<ID3DXBuffer> m_pAdjacency;
	COMPtr<ID3DXBuffer> m_pSubset;
	std::shared_ptr< std::vector<DirectX::XMFLOAT3A> >  m_pVertices;
	std::vector<std::shared_ptr<Entity> > m_entities;
};
class StaticXMeshObjectSubset : public RenderEntity
{
public:
	StaticXMeshObjectSubset(StaticXMeshObject* mesh, u32 idx);
	auto Render(RenderModule* pRenderModule, IRenderer* pRenderer)->void override;
	auto EnableAlpha(bool enabled)->void;
	auto IsEnableAlpha()const->bool;
private:
	StaticXMeshObject* m_pMeshObject;
	u32 m_subsetIndex;
	bool m_enableAlpha;
};