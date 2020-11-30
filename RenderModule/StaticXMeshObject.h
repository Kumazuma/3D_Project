#pragma once
#include"RenderModule.h"
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
	auto Initialize(RenderModule* pRenderModule, std::wstring const& filePath)->HRESULT;
public:
	static auto Create(RenderModule* pRenderModule, std::wstring const& filePath, StaticXMeshObject** pOut)->HRESULT;
	auto PrepareRender(RenderModule* pRenderModule)->void override;
	auto GetMaterialCount()const->u32;
	auto Clone()const->RenderObject*;
	//Ư�� ������� ���İ��� �����Ѵ�.
	auto SetEnableSubsetAlpha(u32 idx, bool enable = true)->void;
private:
	COMPtr<ID3DXMesh> m_pMesh;// �븻 ������ �����Ͽ� ��ȯ��Ų �޽� �İ�ü
	COMPtr<ID3DXBuffer> m_pAdjacency;
	COMPtr<ID3DXBuffer> m_pSubset;
	std::vector<DirectX::XMFLOAT3A> m_vertices;
	std::vector<std::shared_ptr<Entity> > m_entities;
};
class StaticXMeshObjectSubset : public RenderEntity
{
public:
	StaticXMeshObjectSubset(StaticXMeshObject* mesh, u32 idx);
	auto Render(RenderModule* pRenderModule)->void override;
	auto EnableAlpha(bool enabled)->void;
	auto IsEnableAlpha()const->bool;
private:
	StaticXMeshObject* m_pMeshObject;
	DirectX::XMFLOAT4X4 m_worldTransform;
	u32 m_subsetIndex;
	bool m_enableAlpha;
};