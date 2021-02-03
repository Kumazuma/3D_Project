#pragma once
#include"RenderObject.h"
#include"RenderEntity.h"
#include <memory>
#include "COMPtr.hpp"
#include <d3dx9.h>
class SimpleSphareEntity;
class DLL_CLASS SimpleSphereObject : public RenderObject
{
	friend class SimpleSphareEntity;
	using Entity = SimpleSphareEntity;
protected:
	SimpleSphereObject(RenderModule*);
	SimpleSphereObject(SimpleSphereObject const& rhs);
	SimpleSphereObject(SimpleSphereObject&& rhs)noexcept;
public:
	static auto Create(RenderModule* pRenderModule, SimpleSphereObject** pOut)->HRESULT;
public:
	auto Clone()const->RenderObject*;
	//auto PrepareRender(IRenderer* pRenderModule)->void override;
	auto Render(IRendererBase* renderer, ID3DXEffect* effect)->void;
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
	SimpleSphareEntity(SimpleSphereObject* pObj);
	auto Render(RenderModule* pRenderModule, IRenderer* pRenderer)->void override;
	SimpleSphereObject* m_obj;
};