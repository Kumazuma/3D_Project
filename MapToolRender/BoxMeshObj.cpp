#include "pch.h"
#include "BoxMeshObj.h"
#include <RenderModule/SimpleColliderBoxObject.h>
using namespace System::ComponentModel;
MapToolRender::BoxColliderMeshObject::BoxColliderMeshObject(GraphicsDevice^ device):
    m_propertyChangedhandler{ gcnew PropertyChangedEventHandler(this, &BoxColliderMeshObject::OnColliderChangedChanged) }
{
    HRESULT hr{};
    SimpleBoxObject* pBox{};
    hr = SimpleBoxObject::Create(device->Handle, 1.f, 1.f, 1.f, &pBox);
    if (FAILED(hr))
    {
        throw gcnew System::Exception(L"error!!!!");
    }
    m_pNativeObject = pBox;
    pBox->SetColor(1.f, 0.f, 0.f, 1.f);
    this->PropertyChanged += gcnew System::ComponentModel::PropertyChangedEventHandler(this, &MapToolRender::BoxColliderMeshObject::OnBoxObjChanged);
}

MapToolRender::BoxColliderMeshObject::BoxColliderMeshObject(BoxColliderMeshObject^ const& rhs):
    RenderObject(rhs),
    m_collider{  },
    m_propertyChangedhandler{ gcnew PropertyChangedEventHandler(this, &BoxColliderMeshObject::OnColliderChangedChanged) }
{
    if (rhs->m_collider != nullptr)
    {
        m_collider = static_cast<MapToolCore::BoxCollider^>(rhs->m_collider->Clone());
        m_collider->PropertyChanged += m_propertyChangedhandler;
    }
    this->PropertyChanged += gcnew System::ComponentModel::PropertyChangedEventHandler(this, &MapToolRender::BoxColliderMeshObject::OnBoxObjChanged);

}
auto MapToolRender::BoxColliderMeshObject::Clone() -> RenderObject^ 
{
    return gcnew BoxColliderMeshObject(this);
}
auto MapToolRender::BoxColliderMeshObject::OnColliderChangedChanged(System::Object^ sener, System::ComponentModel::PropertyChangedEventArgs^ e) -> void
{
    OnPropertyChanged("Collider");
}
auto MapToolRender::BoxColliderMeshObject::OnBoxObjChanged(System::Object^ sener, System::ComponentModel::PropertyChangedEventArgs^ e) -> void
{
    if (e->PropertyName == "Collider")
    {
        auto boxObj{ static_cast<SimpleBoxObject*>(m_pNativeObject) };
        boxObj->SetSize(m_collider->Width, m_collider->Height, m_collider->Depth);
        boxObj->SetOffset(m_collider->Offset->X, m_collider->Offset->Y, m_collider->Offset->Z);
    }
    
}
auto MapToolRender::BoxColliderMeshObject::Collider::get()->MapToolCore::BoxCollider^
{
    return m_collider;
}
auto MapToolRender::BoxColliderMeshObject::Collider::set(MapToolCore::BoxCollider^ obj)-> void
{
    if (m_collider != nullptr)
    {
        m_collider->PropertyChanged -= m_propertyChangedhandler;
    }
    m_collider = obj;
    m_collider->PropertyChanged += m_propertyChangedhandler;
    OnPropertyChanged("Collider");
}