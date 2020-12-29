#include "SphareColliderAttribute.hpp"

auto Kumazuma::Client::SphareColliderAttribute::GetType() const -> ColliderType 
{
    return ColliderType::SPHARE;
}

auto Kumazuma::Client::SphareColliderAttribute::Clone() const -> ColliderAttribute* 
{
    return new SphareColliderAttribute{ *this };
}

auto Kumazuma::Client::SphareColliderAttribute::DoLoadFromJson(nlohmann::json const& data) -> void
{
    if (auto it = data.find(u8"radius"); it != data.end())
    {
        m_radius = *it;
    }
}

auto Kumazuma::Client::SphareColliderAttribute::GetRadius() const -> f32 const&
{
    return m_radius;
}

auto Kumazuma::Client::SphareColliderAttribute::SetRadius(f32 radius) -> void
{
    m_radius = radius;
}
