#include "Collder.hpp"
#include "ColliderAttribute.hpp"
#include <InitializeFailedException.h>
#include "UnicodeHelper.h"
#include "typedef.hpp"
#include "SphareColliderAttribute.hpp"
#include "BoxColliderAttribute.hpp"
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

auto Kumazuma::Client::Collider::SetFrameName(std::wstring const& id) -> void
{
    m_frameName = id;
}

auto Kumazuma::Client::Collider::GetFrameName() const -> std::wstring const&
{
    return m_frameName;
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

auto Kumazuma::Client::Collider::CheckCollision(
    DirectX::XMFLOAT4X4 const& lhsTransform,
    Collider const& rhs,
    DirectX::XMFLOAT4X4 const& rhsTransform) -> bool
{
    if (GetType() == ColliderType::SPHERE && rhs.GetType() == ColliderType::SPHERE)
    {
        return CheckSphere(lhsTransform, rhs, rhsTransform);
    }
    else
    {
        return CheckOBB(lhsTransform, rhs, rhsTransform);
    }
    return false;
}

auto Kumazuma::Client::Collider::CheckSphere(
    DirectX::XMFLOAT4X4 const& lhsTransform,
    Collider const& rhs,
    DirectX::XMFLOAT4X4 const& rhsTransform) -> bool
{
    
    XMMATRIX mLhsTransform{ XMLoadFloat4x4(&lhsTransform) };
    XMVECTOR vLhsCenter{ XMVector3TransformCoord(XMLoadFloat3(&m_offset), mLhsTransform) };
    XMMATRIX mRhsTransform{ XMLoadFloat4x4(&rhsTransform) };
    XMVECTOR vRhsCenter{ XMVector3TransformCoord(XMLoadFloat3(&rhs.m_offset), mRhsTransform) };
    f32 lhsRadius{ static_cast<SphereColliderAttribute*>(m_attribute.get())->GetRadius()};
    f32 rhsRadius{ static_cast<SphereColliderAttribute*>(rhs.m_attribute.get())->GetRadius() };
    lhsRadius = StoreF32(XMVector3Length(XMVector3TransformNormal(XMVectorSet(lhsRadius, 0.f, 0.f, 0.f), mLhsTransform) ) );
    rhsRadius = StoreF32(XMVector3Length(XMVector3TransformNormal(XMVectorSet(rhsRadius, 0.f, 0.f, 0.f), mRhsTransform) ) );
    f32 distance{ StoreF32(XMVector3Length(vLhsCenter - vRhsCenter)) };
 
    return distance <= lhsRadius + rhsRadius;
}

auto Kumazuma::Client::Collider::CheckOBB(DirectX::XMFLOAT4X4 const& lhsTransform, Collider const& rhs, DirectX::XMFLOAT4X4 const& rhsTransform) -> bool
{
    XMMATRIX mLhsTransform{ XMLoadFloat4x4(&lhsTransform) };
    XMMATRIX mRhsTransform{ XMLoadFloat4x4(&rhsTransform) };
    XMVECTOR vLhsCenter{ XMVector3TransformCoord(XMLoadFloat3(&m_offset), mLhsTransform) };
    XMVECTOR vRhsCenter{ XMVector3TransformCoord(XMLoadFloat3(&rhs.m_offset), mRhsTransform) };
    XMVECTOR vDifference{ vRhsCenter - vLhsCenter };
    f32 deltaLength{ StoreF32(XMVector3Length(vDifference)) };
    //if (deltaLength < m_attribute->GetOBBMaximumRadius(lhsTransform) + rhs.GetAttributeRef().GetOBBMaximumRadius(rhsTransform))
    //{
    //    return false;
    //}

    XMVECTOR vLhsNormals[3]{
        {1.f, 0.f, 0.f, 0.f},
        {0.f, 1.f, 0.f, 0.f},
        {0.f, 0.f, 1.f, 0.f}
    };
    XMVECTOR vRhsNormals[3]{
        {1.f, 0.f, 0.f, 0.f},
        {0.f, 1.f, 0.f, 0.f},
        {0.f, 0.f, 1.f, 0.f}
    };
    //각 법선 벡터를 월드단으로 변형시키고, 법선화시킵니다
    XMVector4TransformStream((XMFLOAT4*)vLhsNormals, sizeof(XMVECTOR), (XMFLOAT4*)vLhsNormals, sizeof(XMVECTOR), 3, mLhsTransform);
    XMVector4TransformStream((XMFLOAT4*)vRhsNormals, sizeof(XMVECTOR), (XMFLOAT4*)vRhsNormals, sizeof(XMVECTOR), 3, mRhsTransform);
    for (i32 i = 0; i < 3; ++i)
    {
        vLhsNormals[i] = XMVector3Normalize(vLhsNormals[i]);
        vRhsNormals[i] = XMVector3Normalize(vRhsNormals[i]);
    }

    for (i32 i = 0; i < 3; ++i)
    {
        f32 r0{ m_attribute->GetOBBRadius(vLhsNormals[i], lhsTransform) };
        f32 r1{ rhs.GetAttributeRef().GetOBBRadius(vLhsNormals[i], rhsTransform) };
        f32 r{ fabsf(StoreF32(XMVector3Dot(vDifference, vLhsNormals[i]))) };
        if (r > r0 + r1) return false;
    }
    for (i32 i = 0; i < 3; ++i)
    {
        f32 r0{ m_attribute->GetOBBRadius(vRhsNormals[i], lhsTransform)  };
        f32 r1{ rhs.GetAttributeRef().GetOBBRadius(vRhsNormals[i], rhsTransform)  };
        f32 r{ fabsf(StoreF32(XMVector3Dot(vDifference, vRhsNormals[i]))) };
        if (r > r0 + r1) return false;
    }
    for (i32 i = 0; i < 3; ++i)
    {
        for (i32 j = 0; j < 3; ++j)
        {
            XMVECTOR vAxis{ XMVector3Normalize(XMVector3Cross(vLhsNormals[i], vRhsNormals[j])) };

            f32 r0{ m_attribute->GetOBBRadius(vAxis, lhsTransform)};
            f32 r1{ rhs.GetAttributeRef().GetOBBRadius(vAxis, rhsTransform)};
            f32 r{ fabsf(StoreF32(XMVector3Dot(vDifference, vAxis))) };
            if (r > r0 + r1) return false;

        }
    }
    return true;
}

typedef	float real;

class CBox
{
public:
    f32x3 center;
    f32x44 axis;
    f32x3 extent;
    f32x3 translation;

    void GetTransform(f32x44& t)
    {
        DirectX::XMMATRIX m{ LoadF32x44(axis) };
        m.r[3] = XMVectorSetW(LoadF32x3(translation), 1.f);
    }
};

//int BoxBoxIntersectionTest(const CBox& box0, const CBox& box1)
//{
//    // compute difference of box centers,D=C1-C0
//    XMVECTOR vD{ LoadF32x3(box1.center) - LoadF32x3(box0.center)};
//    XMMATRIX mBox0Axis{ LoadF32x44(box0.axis) };
//    XMMATRIX mBox1Axis{ LoadF32x44(box0.axis) };
//    XMMATRIX mC{ mBox0Axis * XMMatrixTranspose(mBox1Axis) };
//    XMMATRIX mAbsC{};
//    for (auto i = 0; i < 4; ++i)
//    {
//        mAbsC.r[i] = XMVectorAbs(mC.r[i]);
//    }
//    f32 ad[3]{};
//
//    f32 R{};
//    f32 R0{};
//    f32 R1{};
//    f32 R01{};
//
//
//    real C[3][3];    //matrix C=A^T B,c_{ij}=Dot(A_i,B_j)
//    real absC[3][3]; //|c_{ij}|
//    real AD[3];      //Dot(A_i,D)
//
//
//    //A0
//    C[0][0] = DotProduct(box0.axis[0], box1.axis[0]); //
//    C[0][1] = DotProduct(box0.axis[0], box1.axis[1]); //
//    C[0][2] = DotProduct(box0.axis[0], box1.axis[2]); //
//    AD[0] = DotProduct(box0.axis[0], D);
//    absC[0][0] = (real)fc_fabs(C[0][0]);
//    absC[0][1] = (real)fc_fabs(C[0][1]);
//    absC[0][2] = (real)fc_fabs(C[0][2]);
//    R = (real)fc_fabs(AD[0]);
//    R1 = box1.extent[0] * absC[0][0] + box1.extent[1] * absC[0][1] + box1.extent[2] * absC[0][2];
//    R01 = box0.extent[0] + R1;
//    if (R > R01)return 0;
//
//    //A1
//    C[1][0] = DotProduct(box0.axis[1], box1.axis[0]);
//    C[1][1] = DotProduct(box0.axis[1], box1.axis[1]);
//    C[1][2] = DotProduct(box0.axis[1], box1.axis[2]);
//    AD[1] = DotProduct(box0.axis[1], D);
//    absC[1][0] = (real)fc_fabs(C[1][0]);
//    absC[1][1] = (real)fc_fabs(C[1][1]);
//    absC[1][2] = (real)fc_fabs(C[1][2]);
//    R = (real)fc_fabs(AD[1]);
//    R1 = box1.extent[0] * absC[1][0] + box1.extent[1] * absC[1][1] + box1.extent[2] * absC[1][2];
//    R01 = box0.extent[1] + R1;
//    if (R > R01)return 0;
//
//    //A2
//    C[2][0] = DotProduct(box0.axis[2], box1.axis[0]);
//    C[2][1] = DotProduct(box0.axis[2], box1.axis[1]);
//    C[2][2] = DotProduct(box0.axis[2], box1.axis[2]);
//    AD[2] = DotProduct(box0.axis[2], D);
//    absC[2][0] = (real)fc_fabs(C[2][0]);
//    absC[2][1] = (real)fc_fabs(C[2][1]);
//    absC[2][2] = (real)fc_fabs(C[2][2]);
//    R = (real)fc_fabs(AD[2]);
//    R1 = box1.extent[0] * absC[2][0] + box1.extent[1] * absC[2][1] + box1.extent[2] * absC[2][2];
//    R01 = box0.extent[2] + R1;
//    if (R > R01)return 0;
//
//    //B0
//    R = (real)fc_fabs(DotProduct(box1.axis[0], D));
//    R0 = box0.extent[0] * absC[0][0] + box0.extent[1] * absC[1][0] + box0.extent[2] * absC[2][0];
//    R01 = R0 + box1.extent[0];
//    if (R > R01)return 0;
//
//    //B1
//    R = (real)fc_fabs(DotProduct(box1.axis[1], D));
//    R0 = box0.extent[0] * absC[0][1] + box0.extent[1] * absC[1][1] + box0.extent[2] * absC[2][1];
//    R01 = R0 + box1.extent[1];
//    if (R > R01)return 0;
//
//    //B2
//    R = (real)fc_fabs(DotProduct(box1.axis[2], D));
//    R0 = box0.extent[0] * absC[0][2] + box0.extent[1] * absC[1][2] + box0.extent[2] * absC[2][2];
//    R01 = R0 + box1.extent[2];
//    if (R > R01)return 0;
//
//    //A0xB0
//    R = (real)fc_fabs(AD[2] * C[1][0] - AD[1] * C[2][0]);
//    R0 = box0.extent[1] * absC[2][0] + box0.extent[2] * absC[1][0];
//    R1 = box1.extent[1] * absC[0][2] + box1.extent[2] * absC[0][1];
//    R01 = R0 + R1;
//    if (R > R01)return 0;
//
//    //A0xB1
//    R = (real)fc_fabs(AD[2] * C[1][1] - AD[1] * C[2][1]);
//    R0 = box0.extent[1] * absC[2][1] + box0.extent[2] * absC[1][1];
//    R1 = box1.extent[0] * absC[0][2] + box1.extent[2] * absC[0][0];
//    R01 = R0 + R1;
//    if (R > R01)return 0;
//
//    //A0xB2
//    R = (real)fc_fabs(AD[2] * C[1][2] - AD[1] * C[2][2]);
//    R0 = box0.extent[1] * absC[2][2] + box0.extent[2] * absC[1][2];
//    R1 = box1.extent[0] * absC[0][1] + box1.extent[1] * absC[0][0];
//    R01 = R0 + R1;
//    if (R > R01)return 0;
//
//    //A1xB0
//    R = (real)fc_fabs(AD[0] * C[2][0] - AD[2] * C[0][0]);
//    R0 = box0.extent[0] * absC[2][0] + box0.extent[2] * absC[0][0];
//    R1 = box1.extent[1] * absC[1][2] + box1.extent[2] * absC[1][1];
//    R01 = R0 + R1;
//    if (R > R01)return 0;
//
//    //A1xB1
//    R = (real)fc_fabs(AD[0] * C[2][1] - AD[2] * C[0][1]);
//    R0 = box0.extent[0] * absC[2][1] + box0.extent[2] * absC[0][1];
//    R1 = box1.extent[0] * absC[1][2] + box1.extent[2] * absC[1][0];
//    R01 = R0 + R1;
//    if (R > R01)return 0;
//
//    //A1xB2
//    R = (real)fc_fabs(AD[0] * C[2][2] - AD[2] * C[0][2]);
//    R0 = box0.extent[0] * absC[2][2] + box0.extent[2] * absC[0][2];
//    R1 = box1.extent[0] * absC[1][1] + box1.extent[1] * absC[1][0];
//    R01 = R0 + R1;
//    if (R > R01)return 0;
//
//    //A2xB0
//    R = (real)fc_fabs(AD[1] * C[0][0] - AD[0] * C[1][0]);
//    R0 = box0.extent[0] * absC[1][0] + box0.extent[1] * absC[0][0];
//    R1 = box1.extent[1] * absC[2][2] + box1.extent[2] * absC[2][1];
//    R01 = R0 + R1;
//    if (R > R01)return 0;
//
//    //A2xB1
//    R = (real)fc_fabs(AD[1] * C[0][1] - AD[0] * C[1][1]);
//    R0 = box0.extent[0] * absC[1][1] + box0.extent[1] * absC[0][1];
//    R1 = box1.extent[0] * absC[2][2] + box1.extent[2] * absC[2][0];
//    R01 = R0 + R1;
//    if (R > R01)return 0;
//
//    //A2xB2
//    R = (real)fc_fabs(AD[1] * C[0][2] - AD[0] * C[1][2]);
//    R0 = box0.extent[0] * absC[1][2] + box0.extent[1] * absC[0][2];
//    R1 = box1.extent[0] * absC[2][1] + box1.extent[1] * absC[2][0];
//    R01 = R0 + R1;
//    if (R > R01)return 0;
//
//    return 1;
//}
