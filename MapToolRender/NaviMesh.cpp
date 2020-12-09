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
auto MapToolRender::NaviMesh::Vertices::get()->cli::array<MapToolCore::Position>^
{
	auto obj{ static_cast<::NavMeshRenderingObject*>(m_pNativeObject) };
	unsigned int count{ obj->GetVertexCount() };
	std::vector<DirectX::XMFLOAT4> rawVertices(count, DirectX::XMFLOAT4{});
	
	obj->GetVertices(rawVertices.data(), count * sizeof(DirectX::XMFLOAT4));
	cli::array<MapToolCore::Position>^ res = gcnew cli::array<MapToolCore::Position>(count);
	for (unsigned int i = 0; i < count; ++i)
	{
		auto& vertex{ rawVertices[i] };
		res[i] = MapToolCore::Position(vertex.x, vertex.y, vertex.z);
	}
	return res;
}
auto MapToolRender::NaviMesh::Indices::get()->cli::array<unsigned short>^
{
	auto obj{ static_cast<::NavMeshRenderingObject*>(m_pNativeObject) };
	unsigned int count{ obj->GetIndexCount() };
	std::vector<unsigned short> rawIndices(count, unsigned short{});

	obj->GetIndices(rawIndices.data(), count * sizeof(unsigned short));
	cli::array<unsigned short>^ res = gcnew cli::array<unsigned short>(count);
	
	for (unsigned int i = 0; i < count; ++i)
	{
		res[i] = rawIndices[i];
	}
	return res;
}
