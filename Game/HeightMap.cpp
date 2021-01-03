#include "HeightMap.hpp"
#include <DirectXCollision.h>
#include <unordered_set>
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
auto Kumazuma::Client::HeightMapContainer::RayPicking(DirectX::XMVECTOR rayAt, DirectX::XMVECTOR rayDir) const -> std::optional<f32>
{
    std::optional<f32> res;
    if (m_boundingSphere.RayIntersact(rayAt, rayDir) == std::nullopt)return std::nullopt;

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

auto Kumazuma::Client::HeightMapContainer::GetTriangle(DirectX::XMVECTOR position, DirectX::XMVECTOR up) const->std::optional<Triangle>
{
    if (m_boundingSphere.IsVectorInSphere(position) == false) return std::nullopt;
    for (auto& it : m_children)
    {
        auto res{ it->GetTriangle(position, up) };
        if (res != std::nullopt)
        {
            return res;
        }
    }
    return std::nullopt;
}

auto Kumazuma::Client::HeightMapContainer::GetBoundingSphere() const -> BoundSphere
{
    return m_boundingSphere;
}

auto Kumazuma::Client::HeightMapContainer::IsInBoundSheres(DirectX::XMVECTOR position) const -> bool
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

auto Kumazuma::Client::HeightMapContainer::GetHeight(DirectX::XMVECTOR position) const -> std::optional<f32>
{
    if (m_boundingSphere.IsVectorInSphere(position) == false) return std::nullopt;
    std::optional<f32> res;
    for (auto& it : m_children)
    {
        auto t{ it->GetHeight(position) };
        if (t != std::nullopt &&
            t.value() < res.value_or(std::numeric_limits<f32>::max()))
        {
            res = t;
        }
    }
    return res;
}

Kumazuma::Client::HeightMapSubMesh::HeightMapSubMesh(std::vector<DirectX::XMUINT3>&& triangles, std::shared_ptr< std::vector<DirectX::XMFLOAT3A> > vertices) noexcept :
    m_vertices{ std::move(vertices) },
    m_triangles{ std::move(triangles) }
{
    std::unordered_set<u32> indices;
    for (auto& triangle : m_triangles)
    {
        indices.insert({ triangle.x,triangle.y, triangle.z });

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
        std::array<DirectX::XMFLOAT3A, 3> sideNormals;
        DirectX::XMFLOAT4 plain;
        XMStoreFloat3A(sideNormals.data() + 0, vNormalSideAB);
        XMStoreFloat3A(sideNormals.data() + 1, vNormalSideBC);
        XMStoreFloat3A(sideNormals.data() + 2, vNormalSideCA);
        XMStoreFloat4(&plain, vPlain);

        m_plains.emplace_back(plain);
        m_sideNormls.emplace_back(sideNormals);
    }
    std::vector<DirectX::XMFLOAT3A> triangleVerices;
    triangleVerices.reserve(indices.size());
    for (auto index : indices)
    {
        triangleVerices.emplace_back(m_vertices->at(index));
    }
    m_boundingSphere = BoundSphere::Calculate(triangleVerices.begin(), triangleVerices.end());

}

Kumazuma::Client::HeightMapSubMesh::HeightMapSubMesh(HeightMapSubMesh&& rhs) noexcept :
    m_boundingSphere{ std::move(rhs.m_boundingSphere) },
    m_triangles{ std::move(rhs.m_triangles) },
    m_vertices{ std::move(rhs.m_vertices) },
    m_sideNormls{std::move(rhs.m_sideNormls)},
    m_plains{ std::move(rhs.m_plains) }
{

}

auto Kumazuma::Client::HeightMapSubMesh::RayPicking(DirectX::XMVECTOR rayAt, DirectX::XMVECTOR rayDir) const -> std::optional<f32>
{
    if (m_boundingSphere.RayIntersact(rayAt, rayDir) == std::nullopt)return std::nullopt;
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

auto Kumazuma::Client::HeightMapSubMesh::GetTriangle(DirectX::XMVECTOR position, DirectX::XMVECTOR up) const->std::optional<Triangle>
{
    if (m_boundingSphere.IsVectorInSphere(position) == false) return std::nullopt;

    auto const& vertices{ *m_vertices };
    for (size_t i = 0; i < m_triangles.size(); ++i)
    {
        auto const& triangle{ m_triangles[i] };
        auto const& sideNormals{ m_sideNormls[i] };
        auto const& plain{ m_plains[i] };
        
        std::array<DirectX::XMVECTOR, 3> positions{
            XMLoadFloat3A(vertices.data() + triangle.x),
            XMLoadFloat3A(vertices.data() + triangle.y),
            XMLoadFloat3A(vertices.data() + triangle.z)
        };
        bool dotRes{ true };
        for (size_t j = 0; j < positions.size(); ++j)
        {
            auto vDelta{ position - positions[j] };
            auto vN{ XMLoadFloat3A(sideNormals.data() + j) };
            f32 dotValue;
            XMStoreFloat(&dotValue, XMVector3Dot(vN, vDelta));
            if (dotValue < 0.f)
            {
                dotRes = false;
            }
        }
        if (dotRes == false)
        {
            continue;
        }
        auto plainVertor{ XMLoadFloat4(&plain) };
        f32 t{};
        XMStoreFloat(&t, XMVector4Dot(plainVertor, position));
        if (abs(t) > 0.1f)
        {
            continue;
        }
        Triangle res;
        XMStoreFloat3A(res.vertices + 0, positions[0]);
        XMStoreFloat3A(res.vertices + 1, positions[1]);
        XMStoreFloat3A(res.vertices + 2, positions[2]);
        res.normalVector = { plain.x,plain.y ,plain.z };

        return res;
    }
    return std::nullopt;
}

auto Kumazuma::Client::HeightMapSubMesh::GetBoundingSphere() const -> BoundSphere
{
    return m_boundingSphere;
}

auto Kumazuma::Client::HeightMapSubMesh::GetHeight(DirectX::XMVECTOR position) const -> std::optional<f32>
{
    if (m_boundingSphere.IsVectorInSphere(position) == false) return std::nullopt;
    //TODO:
}

auto Kumazuma::Client::HeightMapSubMesh::IsInBoundSheres(DirectX::XMVECTOR position) const -> bool
{
    return m_boundingSphere.IsVectorInSphere(position);
}
