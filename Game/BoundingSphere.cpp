#include "BoundingSphere.hpp"
using namespace DirectX;
BoundSphere::BoundSphere(DirectX::XMFLOAT3 const& center, f32 radius):
    m_center{center},
    m_radius{radius}
{

}
auto __vectorcall BoundSphere::Calculate(DirectX::XMVECTOR min, DirectX::XMVECTOR max) -> BoundSphere
{
    XMVECTOR vCenter = (min + max) / 0.5f;
    XMFLOAT3 centerPosition{};
    f32 radius{};
    XMStoreFloat3(&centerPosition, vCenter);
    XMStoreFloat(&radius, XMVector3Length(max - vCenter));
    return BoundSphere{ centerPosition, radius };
}

auto BoundSphere::GetCenter() const -> DirectX::XMFLOAT3 const&
{
    return m_center;
}

auto BoundSphere::GetRadius() const -> f32
{
    return m_radius;
}

auto __vectorcall BoundSphere::IsVectorInSphere(DirectX::XMVECTOR pos) const -> bool
{
    XMVECTOR vSphereCenter{ XMLoadFloat3(&m_center) };
    f32 distance{  };
    XMStoreFloat(&distance, XMVector3Length(vSphereCenter - pos));
    return distance <= m_radius;
}
