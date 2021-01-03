#include "BoundingSphere.hpp"
#include <algorithm>
using namespace DirectX;
struct StoreVec
{
    XMVECTOR v;
    inline StoreVec(XMVECTOR v) :v{ v }
    {

    }
    inline operator float()const
    {
        float val{};
        XMStoreFloat(&val, v);
        return val;
    }
    inline operator XMFLOAT2() const
    {
        XMFLOAT2 val{};
        XMStoreFloat2(&val, v);
        return val;
    }
    inline operator XMFLOAT3() const
    {
        XMFLOAT3 val{};
        XMStoreFloat3(&val, v);
        return val;
    }
    inline operator XMFLOAT4() const
    {
        XMFLOAT4 val{};
        XMStoreFloat4(&val, v);
        return val;
    }
};

BoundSphere::BoundSphere(DirectX::XMFLOAT3 const& center, f32 radius):
    m_center{center},
    m_radius{radius}
{

}
auto __vectorcall BoundSphere::Calculate(DirectX::XMVECTOR min, DirectX::XMVECTOR max) -> BoundSphere
{
    XMVECTOR vCenter = (min + max) * 0.5f;
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

auto __vectorcall BoundSphere::RayIntersact(DirectX::XMVECTOR rayOrigin, DirectX::XMVECTOR rayDir) const-> std::optional<f32>
{
    XMVECTOR vCenter{ XMLoadFloat3(&m_center) };
    XMVECTOR vDir{ rayOrigin - vCenter };
    f32 const C{ StoreVec(XMVector3Dot(vDir, vDir)) - m_radius * m_radius };
    f32 const B{ 2.f * StoreVec(XMVector3Dot(rayDir, vDir)) };
    f32 discriminant{ B * B - 4.f * C };
    if (discriminant < 0.f)
    {
        return std::nullopt;
    }
    discriminant =  sqrtf(discriminant);
    f32 const s0{ (-B + discriminant) * 0.5f };
    f32 const s1{ (-B - discriminant) * 0.5f };
    auto [min, max] = std::minmax(s0, s1);
    if (min < 0.f && max < 0.f)
    {
        return std::nullopt;
    }
    if (min >= 0.f && max >= 0.f)
    {
        return min;
    }
    return max;
}
