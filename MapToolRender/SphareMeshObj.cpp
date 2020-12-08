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

auto MapToolRender::SphareMesh::OnColliderChangedChanged(System::Object^ sener, System::ComponentModel::PropertyChangedEventArgs^ e) -> void
{
    RenderObject::OnPropertyChanged("Collider");
}

auto MapToolRender::SphareMesh::OnSelfChanged(System::Object^ sener, System::ComponentModel::PropertyChangedEventArgs^ e) -> void
{
    auto sphareObj{ static_cast<SimpleSphareObject*>(m_pNativeObject) };
    sphareObj->SetOffset(m_collider->Offset->X, m_collider->Offset->Y, m_collider->Offset->Z);
    sphareObj->SetRadius(m_collider->Radius);

}
auto MapToolRender::SphareMesh::Collider::set(MapToolCore::SphareCollider^ value)->void
{
    if (m_collider != nullptr)
    {
        m_collider->PropertyChanged -= m_propertyChangedhandler;
    }
    m_collider = value;
    m_collider->PropertyChanged += m_propertyChangedhandler;
    RenderObject::OnPropertyChanged("Collider");
}
auto MapToolRender::SphareMesh::Collider::get()->MapToolCore::SphareCollider^
{
    return m_collider;
}