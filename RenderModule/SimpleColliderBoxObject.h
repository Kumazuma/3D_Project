#pragma once
#include"RenderObject.h"
#include"RenderEntity.h"
#include <memory>
#include "COMPtr.hpp"
#include <d3dx9.h>
class SimpleBoxEntity;
class DLL_CLASS SimpleBoxObject: public RenderObject
{
	friend class SimpleBoxEntity;
public:
	static auto Create(RenderModule* pRenderModule, f32 width, f32 height, f32 depth, SimpleBoxObject** pOut)->HRESULT;
public:
	auto SetColor(float r, float g, float b, float a)->void;
	auto GetColor()const->DirectX::XMFLOAT4 const&;
	auto Clone()const->RenderObject*;
	auto PrepareRender(IRenderer* pRenderer)->void override;
	auto GetWidth()const->f32;
	auto GetHeight()const->f32;
	auto GetDepth()const->f32;
	auto SetSize(f32 width, f32 height, f32 depth)->void;
	auto SetOffset(f32 x, f32 y, f32 z)->void;
protected:
	auto Initialize(RenderModule* pRenderModule, f32 width, f32 height, f32 depth)->HRESULT;
	SimpleBoxObject();
	SimpleBoxObject(SimpleBoxObject const& rhs);
private:
	DirectX::XMFLOAT3A m_size;
	DirectX::XMFLOAT3A m_offset;
	DirectX::XMFLOAT4 m_color;
	D3DMATERIAL9 m_material;
	COMPtr<ID3DXMesh> m_pMesh;
	std::shared_ptr< SimpleBoxEntity> m_entity;
};
class SimpleBoxEntity : public RenderEntity
{
public:
	SimpleBoxEntity(SimpleBoxObject* pObj);
	auto Render(RenderModule* pRenderModule, IRenderer* pRenderer)->void override;
	SimpleBoxObject* m_obj;
};