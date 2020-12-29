#include "BoxColliderAttribute.hpp"

auto Kumazuma::Client::BoxColliderAttribute::GetType() const -> ColliderType 
{
    return ColliderType::BOX;
}

auto Kumazuma::Client::BoxColliderAttribute::Clone() const -> ColliderAttribute* 
{
    return new BoxColliderAttribute{ *this };
}

auto Kumazuma::Client::BoxColliderAttribute::DoLoadFromJson(nlohmann::json const& data) -> void 
{
    if (auto it = data.find(u8"width"); it != data.end())m_width = *it;
    if (auto it = data.find(u8"height"); it != data.end())m_height = *it;
    if (auto it = data.find(u8"depth"); it != data.end())m_depth= *it;
}

auto Kumazuma::Client::BoxColliderAttribute::SetWidth(f32 width) -> void
{
    m_width = width;
}

auto Kumazuma::Client::BoxColliderAttribute::SetHeight(f32 height) -> void
{
    m_height = height;
}

auto Kumazuma::Client::BoxColliderAttribute::SetDepth(f32 depth) -> void
{
    m_depth = depth;
}

auto Kumazuma::Client::BoxColliderAttribute::GetWidth() const -> f32 const&
{
    return m_width;
}

auto Kumazuma::Client::BoxColliderAttribute::GetHeight() const -> f32 const&
{
    return m_height;
}

auto Kumazuma::Client::BoxColliderAttribute::GetDepth() const -> f32 const&
{
    return m_depth;
}
