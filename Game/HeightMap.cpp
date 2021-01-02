#include "HeightMap.hpp"
#include <DirectXCollision.h>
#undef max
#undef min
using namespace DirectX;
Kumazuma::Client::HeightMapContainer::HeightMapContainer(std::vector<std::unique_ptr<HeightMap>>&& children) noexcept:
    m_children{std::move(children)}
{
constexpr f32 f32_min{ std::numeric_limits<f32>::min() };
constexpr f32 f32_max{ std::numeric_limits<f32>::max() };

    XMVECTOR vNormalizedPlus{ {1.f, 1.f, 1.f, 0.f} };
    XMVECTOR vNormalizedMinus{ -1 * vNormalizedPlus };

    XMVECTOR vMin{ f32_max, f32_max, f32_max, f32_max };
    XMVECTOR vMax{ f32_min,f32_min, f32_min, f32_min };

    for (auto const& child : m_children)
    {
        auto sphere{ child->GetBoundingSphere() };
        auto vCenter{ XMLoadFloat3(&sphere.GetCenter()) };
        auto radius{ sphere.GetRadius() };
        vMin = XMVectorMin(vMin, vCenter + radius * vNormalizedMinus);
        vMax = XMVectorMax(vMax, vCenter + radius * vNormalizedPlus);
    }
    m_boundingSphere = BoundSphere::Calculate(vMin, vMax);

}
auto __vectorcall Kumazuma::Client::HeightMapContainer::RayPicking(DirectX::XMVECTOR rayAt, DirectX::XMVECTOR rayDir) const -> std::optional<f32>
{
    std::optional<f32> res;
    for (auto& it : m_children)
    {
        auto t{ it->RayPicking(rayAt, rayDir) };
        if (t != std::nullopt &&
            t.value() < res.value_or(std::numeric_limits<f32>::max()))
        {
            res = t;
        }
    }
    return res;
}

auto __vectorcall Kumazuma::Client::HeightMapContainer::IsOnTriangles(DirectX::XMVECTOR position) const -> bool
{
    if (m_boundingSphere.IsVectorInSphere(position) == false) return false;
    for (auto& it : m_children)
    {
        if (it->IsOnTriangles(position))
        {
            return true;
        }
    }
    return false;
}

auto Kumazuma::Client::HeightMapContainer::GetBoundingSphere() const -> BoundSphere
{
    return m_boundingSphere;
}

auto __vectorcall Kumazuma::Client::HeightMapContainer::IsInBoundSheres(DirectX::XMVECTOR position) const -> bool
{
    if (m_boundingSphere.IsVectorInSphere(position) == false) return false;
    for (auto& it : m_children)
    {
        if (it->IsInBoundSheres(position))
        {
            return true;
        }
    }
    return false;
}

Kumazuma::Client::HeightMapSubMesh::HeightMapSubMesh(std::vector<DirectX::XMUINT3>&& triangles, std::shared_ptr< std::vector<DirectX::XMFLOAT3A> > vertices) noexcept :
    m_vertices{ std::move(vertices) },
    m_triangles{ std::move(triangles) }
{
    m_boundingSphere = BoundSphere::Calculate(m_vertices->begin(), m_vertices->end());
    for (auto& triangle : m_triangles)
    {
        auto vA{ XMLoadFloat3A(&(*m_vertices)[triangle.x]) };
        auto vB{ XMLoadFloat3A(&(*m_vertices)[triangle.y]) };
        auto vC{ XMLoadFloat3A(&(*m_vertices)[triangle.z]) };
        auto vSideAB{ vB - vA };
        auto vSideBC{ vC - vB };
        auto vSideCA{ vA - vC };
        auto vPlain{ XMPlaneFromPoints(vA, vB, vC) };
        //auto vNormal{ XMVector3Normalize(XMVector3Cross(vB - vA, vC - vA)) };
        auto vNormalSideAB = XMVector3Normalize(XMVector3Cross(vPlain, vSideAB));
        auto vNormalSideBC = XMVector3Normalize(XMVector3Cross(vPlain, vSideBC));
        auto vNormalSideCA = XMVector3Normalize(XMVector3Cross(vPlain, vSideCA));
        XMMATRIX mMatrix{};
        mMatrix.r[0] = vNormalSideAB;
        mMatrix.r[1] = vNormalSideBC;
        mMatrix.r[2] = vNormalSideCA;
        mMatrix.r[3] = vPlain;
        XMFLOAT4X4 normals{};
        XMStoreFloat4x4(&normals, mMatrix);
        m_normalVectors.emplace_back(normals);
    }
}

Kumazuma::Client::HeightMapSubMesh::HeightMapSubMesh(HeightMapSubMesh&& rhs) noexcept :
    m_boundingSphere{ std::move(rhs.m_boundingSphere) },
    m_triangles{ std::move(rhs.m_triangles) },
    m_vertices{ std::move(rhs.m_vertices) },
    m_normalVectors{ std::move(rhs.m_normalVectors) }
{

}

auto __vectorcall Kumazuma::Client::HeightMapSubMesh::RayPicking(DirectX::XMVECTOR rayAt, DirectX::XMVECTOR rayDir) const -> std::optional<f32>
{
    f32 res{ std::numeric_limits<f32>::max() };
    for (auto& triangle : m_triangles)
    {
        auto vA{ XMLoadFloat3A(&(*m_vertices)[triangle.x]) };
        auto vB{ XMLoadFloat3A(&(*m_vertices)[triangle.y]) };
        auto vC{ XMLoadFloat3A(&(*m_vertices)[triangle.z]) };
        f32 t{ };
        if (DirectX::TriangleTests::Intersects(rayAt, rayDir, vA, vB, vC, t))
        {
            if (t < res)//가장 짧은 값을 가져 와야만 한다.
            {
                res = t;
            }
        }
    }
    return res != std::numeric_limits<f32>::max() ? std::optional<f32>{res} : std::nullopt;
}

auto __vectorcall Kumazuma::Client::HeightMapSubMesh::IsOnTriangles(DirectX::XMVECTOR position) const -> bool
{
    auto triangleIt{ m_triangles.begin() };
    auto normalsIt{ m_normalVectors.begin() };
    auto const triangleEndIt{ m_triangles.end() };
    auto const normalsEndIt{ m_normalVectors.end() };
    for (; triangleIt != triangleEndIt; ++triangleIt, ++normalsIt)
    {
        auto vA{ XMLoadFloat3A(&(*m_vertices)[triangleIt->x]) };
        auto vB{ XMLoadFloat3A(&(*m_vertices)[triangleIt->y]) };
        auto vC{ XMLoadFloat3A(&(*m_vertices)[triangleIt->z]) };
        auto mNormals{ XMLoadFloat4x4(&*normalsIt) };
        auto vAP{ position - vA };
        auto vBP{ position - vB };
        auto vCP{ position - vC };
        XMMATRIX mD{
            vAP,
            vBP,
            vCP,
            {0.f, 0.f, 0.f, 1.f}
        };
        //행렬로 계산하면 4개의 벡터를 각각 내적 연산한 것을 한 번에 구할 수 있다.
        XMMATRIX mDot = mD * DirectX::XMMatrixTranspose(mNormals);
        float t{};
        XMStoreFloat(&t, XMVector4Dot(position, mNormals.r[3]));
        bool res{
            XMVector3GreaterOrEqual(mDot.r[0], XMVectorZero()) &&
            XMVector3GreaterOrEqual(mDot.r[1], XMVectorZero()) &&
            XMVector3GreaterOrEqual(mDot.r[2], XMVectorZero()) &&
            abs(t) <= 0.00000000001f
        };
        if (res)
        {
            return true;
        }
    }
    return false;
}

auto Kumazuma::Client::HeightMapSubMesh::GetBoundingSphere() const -> BoundSphere
{
    return m_boundingSphere;
}

auto __vectorcall Kumazuma::Client::HeightMapSubMesh::IsInBoundSheres(DirectX::XMVECTOR position) const -> bool
{
    return m_boundingSphere.IsVectorInSphere(position);
}
