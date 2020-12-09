#include "pch.h"
#include "SphareMeshObj.h"
#include <RenderModule/SimpleColliderSphareObject.h>
using namespace System::ComponentModel;
MapToolRender::SphareMesh::SphareMesh(GraphicsDevice^ device)
{
    SimpleSphareObject* sphareObj{};
    HRESULT hr{};
    hr = SimpleSphareObject::Create(device->Handle, &sphareObj);
    if (FAILED(hr))
    {
        throw gcnew System::Exception("Could not create Sphare Mesh!");
    }
    m_pNativeObject = sphareObj;
}
//TODO:
MapToolRender::SphareMesh::SphareMesh(SphareMesh^ const& rhs):
    RenderObject(this),
    m_collider(nullptr)
{
}

auto MapToolRender::SphareMesh::Clone() -> RenderObject^ 
{
    return gcnew SphareMesh{ this };
}

auto MapToolRender::SphareMesh::Radius::get() -> float
{
    return m_radius;
}

auto MapToolRender::SphareMesh::Radius::set(float value) -> void
{
    m_radius = value;
}