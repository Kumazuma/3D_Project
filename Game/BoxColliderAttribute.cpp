#include "BoxColliderAttribute.hpp"
using namespace DirectX;
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
    if (auto it = data.find(u8"width"); it != data.end())m_size.x = *it;
    if (auto it = data.find(u8"height"); it != data.end())m_size.y= *it;
    if (auto it = data.find(u8"depth"); it != data.end())m_size.z = *it;
}

auto Kumazuma::Client::BoxColliderAttribute::GetOBBRadius(DirectX::XMVECTOR axis, f32x44 const& transform) const -> f32 
{
    auto vForward{ XMVectorSet(m_size.x * 0.5f, 0.f, 0.f, 0.f) };
    auto vUp{ XMVectorSet(0.f, m_size.y * 0.5f, 0.f, 0.f) };
    auto vRight{ XMVectorSet(0.f, 0.f, m_size.z * 0.5f, 0.f) };
    auto mTransform{ LoadF32x44(transform) };
    vForward = XMVector3TransformNormal(vForward, mTransform);
    vUp = XMVector3TransformNormal(vUp, mTransform);
    vRight = XMVector3TransformNormal(vRight, mTransform);

    return
        abs(StoreF32(XMVector3Dot(vForward, axis))) +
        abs(StoreF32(XMVector3Dot(vUp, axis))) +
        abs(StoreF32(XMVector3Dot(vRight, axis)));
}
auto Kumazuma::Client::BoxColliderAttribute::GetOBBMaximumRadius(f32x44 const& transform) const -> f32 
{
    auto vSize{ LoadF32x3(m_size) };
    auto mTransform{ LoadF32x44(transform) };
    vSize = XMVector3TransformNormal(vSize, mTransform);
    return StoreF32(XMVector3Length(vSize));
}
auto Kumazuma::Client::BoxColliderAttribute::SetWidth(f32 width) -> void
{
    m_size.x = width;
}

auto Kumazuma::Client::BoxColliderAttribute::SetHeight(f32 height) -> void
{
    m_size.y = height;
}

auto Kumazuma::Client::BoxColliderAttribute::SetDepth(f32 depth) -> void
{
    m_size.z = depth;
}

auto Kumazuma::Client::BoxColliderAttribute::GetWidth() const -> f32 const&
{
    return m_size.x;
}

auto Kumazuma::Client::BoxColliderAttribute::GetHeight() const -> f32 const&
{
    return m_size.y;
}

auto Kumazuma::Client::BoxColliderAttribute::GetDepth() const -> f32 const&
{
    return m_size.z;
}

auto Kumazuma::Client::BoxColliderAttribute::UpdatePositions() -> void
{
}
