#include "pch.h"
#include "Ray.h"
#include "MapToolRender.h"
#include "Transform.h"
#include <RenderModule/Ray.h>
MapToolRender::Ray::Ray(MapToolCore::Position rayStart, MapToolCore::Position rayAt)
{
    m_pNative = new ::Ray(
        { rayStart.X,rayStart.Y,rayStart.Z },
        { rayAt.X,rayAt.Y,rayAt.Z });
}

MapToolRender::Ray::Ray(DirectX::XMFLOAT3* rayStart, DirectX::XMFLOAT3* rayAt)
{
    m_pNative = new ::Ray(*rayStart, *rayAt);
}

MapToolRender::Ray::~Ray()
{
    if (m_pNative != nullptr)
    {
        delete m_pNative;
        m_pNative = nullptr;
    }
}

MapToolRender::Ray::!Ray()
{
    this->~Ray();
}

auto MapToolRender::Ray::GetPosition(float t) -> MapToolCore::Position
{
    auto res = m_pNative->GetPosition(t);
    return MapToolCore::Position(res.x, res.y, res.z);
}
