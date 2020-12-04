#include "pch.h"
#include "Ray.h"
#include "MapToolRender.h"
#include "Transform.h"
#include <RenderModule/Ray.h>
#include "Line3D.h"
MapToolRender::Ray::Ray(Position^ rayStart, Position^ rayAt)
{
    m_pNative = new ::Ray(
        { rayStart->X,rayStart->Y,rayStart->Z },
        { rayAt->X,rayAt->Y,rayAt->Z });
}

MapToolRender::Ray::Ray(DirectX::XMFLOAT3* rayStart, DirectX::XMFLOAT3* rayAt)
{
    m_pNative = new ::Ray(*rayStart, *rayAt);
}

MapToolRender::Ray::!Ray()
{
    if (m_pNative != nullptr)
    {
        delete m_pNative;
    }
}

auto MapToolRender::Ray::Create3DLine(GraphicsDevice^ graphicsDev, float length) -> Line3D^
{
    auto startPosition{ m_pNative->GetRayOrigin() };
    auto dir{ m_pNative->GetRayDirection() };
    auto endPosition{ startPosition };
    endPosition.x += dir.x * length;
    endPosition.y += dir.y * length;
    endPosition.z += dir.z * length;
    return gcnew Line3D(graphicsDev, &startPosition, &endPosition);
}

auto MapToolRender::Ray::GetPosition(float t) -> Position^
{
    auto res = m_pNative->GetPosition(t);
    return gcnew Position(res.x, res.y, res.z);
}
