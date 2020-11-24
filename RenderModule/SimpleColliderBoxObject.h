#pragma once
#include"RenderObject.h"
#include"RenderModule.h"
#include <d3dx9.h>

class SimpleBoxObject: public RenderObject
{
public:
	static auto Create(RenderModule* pRenderModule, f32 width, f32 height, f32 depth, SimpleBoxObject** pOut)->HRESULT;
public:
	auto SetColor(float r, float g, float b, float a)->void;
	auto GetColor()const->DirectX::XMFLOAT4 const&;
	auto Clone()const->RenderObject*;
	auto Render(RenderModule* pRenderModule)->void;
protected:
	auto Initialize(RenderModule* pRenderModule, f32 width, f32 height, f32 depth)->HRESULT;
private:

	u32 m_width;
	u32 m_depth;
	u32 m_height;
	DirectX::XMFLOAT4 m_color;
	D3DMATERIAL9 m_material;
	COMPtr<ID3DXMesh> m_pMesh;
};