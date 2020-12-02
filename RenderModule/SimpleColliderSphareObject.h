#pragma once
#include"RenderObject.h"
#include"RenderEntity.h"
#include <memory>
#include "COMPtr.hpp"
#include <d3dx9.h>
class SimpleSphareEntity;
class DLL_CLASS SimpleSphareObject : public RenderObject
{
	friend class SimpleSphareEntity;
	using Entity = SimpleSphareEntity;
protected:
	SimpleSphareObject(RenderModule*);
	SimpleSphareObject(SimpleSphareObject const& rhs);
	SimpleSphareObject(SimpleSphareObject&& rhs)noexcept;
public:
	static auto Create(RenderModule* pRenderModule, SimpleSphareObject** pOut)->HRESULT;
public:
	auto Clone()const->RenderObject*;
	auto PrepareRender(RenderModule* pRenderModule)->void override;
	auto SetRadius(f32 radius)->void;
	auto SetOffset(f32 x, f32 y, f32 z)->void;

private:
	DirectX::XMFLOAT4 m_offsetAndRadius;
	D3DMATERIAL9 m_material;
	COMPtr<ID3DXMesh> m_pMesh;
	std::shared_ptr<SimpleSphareEntity> m_entity;
};
class SimpleSphareEntity : public RenderEntity
{
public:
	SimpleSphareEntity(SimpleSphareObject* pObj);
	auto Render(RenderModule* pRenderModule)->void override;
	SimpleSphareObject* m_obj;
};