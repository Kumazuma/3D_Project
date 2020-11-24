#pragma once
#include"RenderObject.h"
#include"COMPtr.hpp"
#include<vector>
#include<memory>
#include<string>
#include<d3dx9.h>
#include "MeshObject.h"
class DLL_CLASS DynamicXMeshObject : public XMeshObject
{
protected:
	DynamicXMeshObject();
	auto Initialize(RenderModule* pRenderModule, std::wstring const& filePath)->HRESULT;
public:
	static auto Create(RenderModule* pRenderModule, std::wstring const& filePath, DynamicXMeshObject** pOut)->HRESULT;
	auto Render(RenderModule* pRenderModule)->void override;
	auto GetMaterialCount()const->u32;
	auto Clone()const->RenderObject*;
private:
	COMPtr<ID3DXMesh> m_pOriMesh;// ���� �ε� ������ �����ϴ� �޽� �İ�ü
	COMPtr<ID3DXMesh> m_pMesh;// �븻 ������ �����Ͽ� ��ȯ��Ų �޽� �İ�ü
	COMPtr<ID3DXBuffer> m_pAdjacency;
	COMPtr<ID3DXBuffer> m_pSubset;
	std::vector<DirectX::XMFLOAT3A> m_vertices;
};