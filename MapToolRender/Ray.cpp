#include "pch.h"
#include "Ray.h"
#include "MapToolRender.h"
#include "Transform.h"
#include <RenderModule/Ray.h>

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

auto MapToolRender::Ray::Create3DLine(GraphicsDevice^ graphicsDev) -> Line3D^
{
    
}
