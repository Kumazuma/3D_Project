#include "pch.h"
#include "MapToolRender.h"
#include "WowMapMesh.h"
#include <RenderModule/WowMapMeshObject.h>
#include "msclr\marshal_cppstd.h"
using namespace msclr::interop;
MapToolRender::WowMapMesh::WowMapMesh(GraphicsDevice^ device, System::String^ filePath)
{
    marshal_context ctx;
    std::wstring szFilePath{ ctx.marshal_as<std::wstring>(filePath) };
    WowMapMeshObject* obj;
    WowMapMeshObject::Create(device->Handle, szFilePath, &obj);
    if (obj == nullptr)
    {
        throw gcnew System::Exception("Failed Create Mesh Object");
    }
    m_pNativeObject = obj;
    m_filePath = filePath;
}

auto MapToolRender::WowMapMesh::Clone() -> RenderObject^
{
    return gcnew WowMapMesh(this);
}

MapToolRender::WowMapMesh::WowMapMesh(WowMapMesh^ const& rhs):
    RenderObject{ rhs },
    m_filePath{rhs->m_filePath}
{
    
}
auto MapToolRender::WowMapMesh::MeshFilePath::get()->System::String^
{
    return m_filePath;
}

auto MapToolRender::WowMapMesh::Center::get()->MapToolCore::Position
{
    DirectX::XMFLOAT3 position = static_cast<WowMapMeshObject*>(m_pNativeObject)->GetCenter();
    return MapToolCore::Position(position.x, position.y, position.z);
}