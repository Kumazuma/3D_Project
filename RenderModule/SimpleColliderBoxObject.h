#pragma once
#include"RenderObject.h"
#include"RenderModule.h"
#include <d3dx9.h>
class SimpleBoxEntity;
class SimpleBoxObject: public RenderObject
{
	friend class SimpleBoxEntity;
public:
	static auto Create(RenderModule* pRenderModule, f32 width, f32 height, f32 depth, SimpleBoxObject** pOut)->HRESULT;
public:
	auto SetColor(float r, float g, float b, float a)->void;
	auto GetColor()const->DirectX::XMFLOAT4 const&;
	auto Clone()const->RenderObject*;
	auto PrepareRender(RenderModule* pRenderModule)->void override;
	auto GetWidth()const->f32;
	auto GetHeight()const->f32;
	auto GetDepth()const->f32;
	auto SetWidth(f32 val)->void;
	auto SetHeight(f32 val)->void;
	auto SetDepth(f32 val)->void;
protected:
	auto Initialize(RenderModule* pRenderModule, f32 width, f32 height, f32 depth)->HRESULT;
	SimpleBoxObject();
	SimpleBoxObject(SimpleBoxObject const& rhs);
private:

	f32 m_width;
	f32 m_depth;
	f32 m_height;
	DirectX::XMFLOAT4 m_color;
	D3DMATERIAL9 m_material;
	COMPtr<ID3DXMesh> m_pMesh;
	std::shared_ptr< SimpleBoxEntity> m_entity;
};
class SimpleBoxEntity : public RenderEntity
{
public:
	SimpleBoxEntity(SimpleBoxObject* pObj);
	auto Render(RenderModule* pRenderModule)->void override;
	SimpleBoxObject* m_obj;
};