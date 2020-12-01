#include "pch.h"
#include "BoxMeshObj.h"
#include <RenderModule/SimpleColliderBoxObject.h>
MapToolRender::BoxColliderMeshObject::BoxColliderMeshObject(GraphicsDevice^ device)
{
    HRESULT hr{};
    SimpleBoxObject* pBox{};
    hr = SimpleBoxObject::Create(device->Handle, 1.f, 1.f, 1.f, &pBox);
    if (FAILED(hr))
    {
        throw gcnew System::Exception(L"error!!!!");
    }
    m_pNativeObject = pBox;
}

MapToolRender::BoxColliderMeshObject::BoxColliderMeshObject(BoxColliderMeshObject^ const& rhs):
    RenderObject(rhs),
    m_collider{  }
{
    if (rhs->m_collider != nullptr)
    {
        m_collider = static_cast<MapToolCore::BoxCollider^>(rhs->m_collider->Clone());
    }
}
auto MapToolRender::BoxColliderMeshObject::Clone() -> RenderObject^ 
{
    return gcnew BoxColliderMeshObject(this);
}
auto MapToolRender::BoxColliderMeshObject::Collider::get()->MapToolCore::BoxCollider^
{
    return m_collider;
}
auto MapToolRender::BoxColliderMeshObject::Collider::set(MapToolCore::BoxCollider^ obj)-> void
{
    m_collider = obj;
}