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
    m_propertyChangedhandler = gcnew PropertyChangedEventHandler(this, &SphareMesh::OnColliderChangedChanged);
    PropertyChanged += gcnew PropertyChangedEventHandler(this, &MapToolRender::SphareMesh::OnSelfChanged);
}
//TODO:
MapToolRender::SphareMesh::SphareMesh(SphareMesh^ const& rhs):
    RenderObject(this),
    m_collider(nullptr)
{
    PropertyChanged += gcnew PropertyChangedEventHandler(this, &MapToolRender::SphareMesh::OnSelfChanged);
}

auto MapToolRender::SphareMesh::Clone() -> RenderObject^ 
{
    return gcnew SphareMesh{ this };
}