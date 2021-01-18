#include "pch.h"
#include "MapToolRender.h"
#include "WowMapMeshObject.h"
#include <RenderModule/WavefrontOBJMesh.h>
#include "msclr\marshal_cppstd.h"
using namespace msclr::interop;

MapToolRender::WowMapMesh::WowMapMesh(GraphicsDevice^ device, System::String^ filePath)
{
    marshal_context ctx;
    std::wstring szFilePath{ ctx.marshal_as<std::wstring>(filePath) };
    WavefrontOBJMesh* obj;
    WavefrontOBJMesh::Create(device->Handle, szFilePath, &obj);
    if (obj == nullptr)
    {
        throw gcnew System::Exception("Failed Create Mesh Object");
    }
    m_pNativeObject = obj;
    m_filePath = filePath;
    m_usage = MapToolRender::Usage::None;
}

auto MapToolRender::WowMapMesh::Clone() -> RenderObject^
{
    return gcnew WowMapMesh(this);
}

MapToolRender::WowMapMesh::WowMapMesh(WowMapMesh^ const& rhs):
    RenderObject{ rhs },
    m_filePath{rhs->m_filePath},
    m_usage{rhs->m_usage}
{
    
}
auto MapToolRender::WowMapMesh::MeshFilePath::get()->System::String^
{
    return m_filePath;
}

auto MapToolRender::WowMapMesh::Center::get()->MapToolCore::Position
{
    DirectX::XMFLOAT3 position = static_cast<WavefrontOBJMesh*>(m_pNativeObject)->GetCenter();
    return MapToolCore::Position(position.x, position.y, position.z);
}

auto MapToolRender::WowMapMesh::Usage::get()->MapToolRender::Usage
{
    return m_usage;
}
auto MapToolRender::WowMapMesh::Usage::set(MapToolRender::Usage value)->void
{
    m_usage = value;
    MapObject::BroadcastPropertyChanged("Usage");
}