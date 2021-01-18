#include "SphareColliderAttribute.hpp"
using namespace DirectX;
auto Kumazuma::Client::SphereColliderAttribute::GetType() const -> ColliderType 
{
    return ColliderType::SPHERE;
}

auto Kumazuma::Client::SphereColliderAttribute::Clone() const -> ColliderAttribute* 
{
    return new SphereColliderAttribute{ *this };
}

auto Kumazuma::Client::SphereColliderAttribute::DoLoadFromJson(nlohmann::json const& data) -> void
{
    if (auto it = data.find(u8"radius"); it != data.end())
    {
        m_radius = *it;
    }
}


auto Kumazuma::Client::SphereColliderAttribute::GetOBBRadius(DirectX::XMVECTOR axis, f32x44 const& transform) const -> f32
{
    auto mTransform{ LoadF32x44(transform) };
    auto radius{ StoreF32(XMVector3Length(XMVector3TransformNormal(XMVectorSet(m_radius, 0.f, 0.f, 0.f), mTransform))) };
    return radius;
}

auto Kumazuma::Client::SphereColliderAttribute::GetOBBMaximumRadius(f32x44 const& transform) const -> f32
{
    auto mTransform{ LoadF32x44(transform) };
    auto radius{ StoreF32(XMVector3Length(XMVector3TransformNormal(XMVectorSet(m_radius, 0.f, 0.f, 0.f), mTransform))) };
    return radius;
}

auto Kumazuma::Client::SphereColliderAttribute::GetRadius() const -> f32 const&
{
    return m_radius;
}

auto Kumazuma::Client::SphereColliderAttribute::SetRadius(f32 radius) -> void
{
    m_radius = radius;
}
