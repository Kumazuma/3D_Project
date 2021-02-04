#include "pch.h"
#include "TargetObject.h"
#include <RenderModule/SimpleColliderSphareObject.h>
MapToolRender::TargetObject::TargetObject(GraphicsDevice^ device)
{
    SimpleSphereObject* pSphareObject{ nullptr };
    SimpleSphereObject::Create(device->Handle, &pSphareObject);
    if (pSphareObject == nullptr)
    {
        throw gcnew Exception("failed creating sphare render object");
    }
    m_pNativeObject = pSphareObject;
}

MapToolRender::TargetObject::TargetObject(TargetObject^ rhs):
    RenderObject{ rhs }
{
    
}

auto MapToolRender::TargetObject::Clone() -> RenderObject^ 
{
    return gcnew TargetObject{ this };
}
