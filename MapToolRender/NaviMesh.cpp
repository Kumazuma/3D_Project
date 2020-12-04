#include "pch.h"
#include "NaviMesh.h"
#include <RenderModule/NavMesh.h>
MapToolRender::NaviMesh::NaviMesh(GraphicsDevice^ device)
{
	HRESULT hr{};
	::NavMeshRenderingObject* pOut{};
	hr = ::NavMeshRenderingObject::Create(device->Handle, &pOut);
	if (FAILED(hr))
	{
		throw gcnew System::Exception("Can not Create Nav Mesh!");
	}
	m_pNativeObject = pOut;
}

MapToolRender::NaviMesh::NaviMesh(NaviMesh^ const& rhs)
{

}

auto MapToolRender::NaviMesh::Clone() -> RenderObject^ 
{
	return nullptr;
}

auto MapToolRender::NaviMesh::PushPoint(float x, float y, float z, Camera^ camera) -> void
{
	auto obj{ static_cast<::NavMeshRenderingObject*>(m_pNativeObject) };
	obj->PushPoint({ x,y,z }, *camera->PositionPtr, *camera->RotationPtr);
}
auto MapToolRender::NaviMesh::WeldDistance::get()->float
{
	auto obj{ static_cast<::NavMeshRenderingObject*>(m_pNativeObject) };
	return obj->GetWeldDistance();
}
auto MapToolRender::NaviMesh::WeldDistance::set(float value)->void
{
	auto obj{ static_cast<::NavMeshRenderingObject*>(m_pNativeObject) };
	obj->SetWeldDistance(value);
	this->OnPropertyChanged("WeldDistance");
}