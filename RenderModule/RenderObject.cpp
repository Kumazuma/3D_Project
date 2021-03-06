#include "pch.h"
#include "RenderObject.h"
#include "Ray.h"
RenderObject::RenderObject():
    //m_transform{},
    m_visible{ true }
{
    //for (size_t i = 0; i < 4; ++i)
    //{
    //    m_transform(i, i) = 1.f;
    //}
}

auto RenderObject::CanRayPicking() const -> bool
{
    return false;
}

auto RenderObject::RayPicking(DirectX::XMFLOAT3 const& rayAt, DirectX::XMFLOAT3 const& rayDirection, f32* pOut) -> bool
{
    return false;
}

auto RenderObject::RayPicking(Ray* pRay, f32* pOut) -> bool
{
    return RayPicking(pRay->GetRayOrigin(), pRay->GetRayDirection(), pOut);
}

//auto MeshObject::SetTransform(DirectX::XMFLOAT4X4 const& transform) -> void
//{
//    m_transform = transform;
//
//}
//auto MeshObject::GetTransform() const -> DirectX::XMFLOAT4X4 const&
//{
//    return m_transform;
//}

auto RenderObject::SetVisible(bool visible) -> void
{
    m_visible = visible;
}

auto RenderObject::IsVisible() const -> bool
{
    return m_visible;
}
