#include "Collder.hpp"
#include "ColliderAttribute.hpp"
#include <InitializeFailedException.h>
#include "UnicodeHelper.h"
using namespace DirectX;
inline auto LoadXYZ(nlohmann::json const& it)->XMFLOAT3
{
    float x{ it[u8"x"] };
    float y{ it[u8"y"] };
    float z{ it[u8"z"] };
    return XMFLOAT3{ x, y, z };
}
Kumazuma::Client::Collider::Collider():
    m_attribute{  },
    m_offset{  },
    m_position{ },
    m_scale{ },
    m_rotation{ }
{
}

Kumazuma::Client::Collider::Collider(Collider const& rhs):
    m_frameName{rhs.m_frameName},
    m_attribute{rhs.m_attribute->Clone()},
    m_offset{rhs.m_offset},
    m_position{rhs.m_position},
    m_scale{rhs.m_scale},
    m_rotation{rhs.m_rotation}
{
    
}

Kumazuma::Client::Collider::Collider(Collider&& rhs) noexcept:
    m_frameName{ std::move(rhs.m_frameName) },
    m_attribute{std::move(rhs.m_attribute)},
    m_offset{ std::move(rhs.m_offset) },
    m_position{std::move(rhs.m_position)},
    m_scale{std::move(rhs.m_scale)},
    m_rotation{std::move(rhs.m_rotation)}
{
}

Kumazuma::Client::Collider::Collider(nlohmann::json const& data):
    Collider{}
{
    if (data.is_object() == false) throw InitializeFailedException{ __FILEW__, __LINE__, L"Json Collider Item is not Object" };
    nlohmann::json::const_iterator it;
    
    if (it = data.find(u8"frame_name"); it == data.end())throw InitializeFailedException{ __FILEW__, __LINE__, L"has no 'frame_name'" };
    else if(!it->is_string() && !it->is_null())throw InitializeFailedException{ __FILEW__, __LINE__, L"has no 'frame_name'" };
    m_frameName = ConvertUTF8ToWide(*it);

    if(it = data.find(u8"offset"); it != data.end())
    {
        auto const& offset{ *it };
        m_offset = LoadXYZ(offset);
    }
    if (it = data.find(u8"transform"); it != data.end())
    {
        auto const& transform{ *it };
        m_position = LoadXYZ(transform[u8"position"]);
        m_rotation = LoadXYZ(transform[u8"rotation"]);
        m_scale = LoadXYZ(transform[u8"scale"]);
    }
    if(it = data.find(u8"type"); it == data.end())  throw InitializeFailedException{ __FILEW__, __LINE__, L"has no 'type'" };
    if(it->is_string()==false)   throw InitializeFailedException{ __FILEW__, __LINE__, L"type's type must be 'string'" };
    m_attribute.reset(ColliderAttribute::LoadFromJson(data));
}

auto Kumazuma::Client::Collider::LoadFromJson(nlohmann::json const& data) -> Collider*
{
    try
    {
        Collider tmp{ data };
        return new Collider{ std::move(tmp) };
    }
    catch (InitializeFailedException ex)
    {
        __debugbreak();
    }
    return nullptr;
}

auto Kumazuma::Client::Collider::SetPosition(DirectX::XMFLOAT3 const& val) -> void
{
    m_position = val;
}

auto Kumazuma::Client::Collider::SetRotation(DirectX::XMFLOAT3 const& val) -> void
{
    m_rotation = val;
}

auto Kumazuma::Client::Collider::SetScale(DirectX::XMFLOAT3 const& val) -> void
{
    m_scale = val;
}

auto Kumazuma::Client::Collider::SetOffset(DirectX::XMFLOAT3 const& val)  -> void
{
    m_offset = val;
}

auto Kumazuma::Client::Collider::SetType(ColliderType newType) -> void
{
    if (GetType() == newType)
    {
        return;
    }
    m_attribute.reset(ColliderAttribute::FromType(newType));
}

auto Kumazuma::Client::Collider::GetPosition() const -> DirectX::XMFLOAT3 const&
{
    return m_position;
}

auto Kumazuma::Client::Collider::GetRotation() const -> DirectX::XMFLOAT3 const&
{
    return m_rotation;
}

auto Kumazuma::Client::Collider::GetScale() const -> DirectX::XMFLOAT3 const&
{
    return m_scale;
}

auto Kumazuma::Client::Collider::GetOffset() const -> DirectX::XMFLOAT3 const&
{
    return m_offset;
}

auto Kumazuma::Client::Collider::GetType() const -> ColliderType
{
    if (m_attribute == nullptr)
    {
        return ColliderType::NONE;
    }
    return m_attribute->GetType();
}

auto Kumazuma::Client::Collider::GetAttributeRef() const -> ColliderAttribute const&
{
    return *m_attribute;
}

auto Kumazuma::Client::Collider::GetAttributeRef() -> ColliderAttribute&
{
    return *m_attribute;
}

auto Kumazuma::Client::Collider::GenerateColliderMatrix(DirectX::XMFLOAT4X4* pOut) -> void
{
    XMVECTOR vPosition{ XMVectorSetW(XMLoadFloat3(&m_position), 1) };
    XMVECTOR vRotation{ XMLoadFloat3(&m_rotation) * (XM_2PI/360.f) };
    XMVECTOR vScale{ XMLoadFloat3(&m_scale) };
    XMVECTOR vOffset{XMVectorSetW(XMLoadFloat3(&m_offset), 1) };

    XMMATRIX mOffset{ XMMatrixIdentity() };
    mOffset.r[3] = vOffset;

    XMMATRIX mPosition{ XMMatrixTranslationFromVector(vPosition) };
    XMMATRIX mRotation{ XMMatrixRotationRollPitchYawFromVector(vRotation) };
    XMMATRIX mScale{ XMMatrixScalingFromVector(vScale) };

    XMStoreFloat4x4(pOut, mOffset * mScale * mRotation * mPosition);
}
