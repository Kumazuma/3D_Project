#pragma once
#include"RenderObject.h"
#include"RenderEntity.h"
#include<array>
#include<memory>
class Line3DEntity;
class DLL_CLASS Line3D : public RenderObject
{
	friend class Line3DEntity;
	static constexpr DWORD FVF = FVF_NAVI;
	using Entity = Line3DEntity;
	using Vertex = RenderObject::VERTEX<FVF>;
	using Self = Line3D;
protected:
	Line3D(RenderModule* renderModule, DirectX::XMFLOAT3 const& start, DirectX::XMFLOAT3 const& end);
	Line3D(Line3D const& rhs);
	Line3D(Line3D && rhs) noexcept;
public:
	static auto Create(RenderModule* renderModule, DirectX::XMFLOAT3 const& start, DirectX::XMFLOAT3 const& end, Line3D** pOut)->HRESULT;
	auto GetStartPositin()const->DirectX::XMFLOAT3;
	auto GetEndPositin()const->DirectX::XMFLOAT3;
	auto Clone()const->RenderObject*;
	auto PrepareRender(RenderModule* renderModule)->void;
protected:
	std::array< Vertex, 2> m_line;
	std::shared_ptr<Entity> m_pEntity;
};
class Line3DEntity : public RenderEntity
{
public:
	Line3DEntity(Line3D* pObj);
	auto SetObject(Line3D* pObj)->void;
	auto Render(RenderModule* renderModule)->void;
private:
	Line3D* m_pObj;
};