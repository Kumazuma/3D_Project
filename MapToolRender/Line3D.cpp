#include "pch.h"
#include "Line3D.h"
#include <RenderModule/Line3D.h>
MapToolRender::Line3D::Line3D(GraphicsDevice^ graphicsDevice, Position^ startPosition, Position^ endPosition)
{
    HRESULT hr{};
    ::Line3D* pNativeObj{};
    hr = ::Line3D::Create(
        graphicsDevice->Handle,
        {startPosition->X,startPosition->Y ,startPosition->Z },
        { endPosition->X,endPosition->Y ,endPosition->Z },
        &pNativeObj
        );
    if (FAILED(hr))
    {
        throw gcnew System::Exception("Can not Create Line3D!");
    }
    m_pNativeObject = pNativeObj;
}

MapToolRender::Line3D::Line3D(GraphicsDevice^ graphicsDevice, DirectX::XMFLOAT3* startPosition, DirectX::XMFLOAT3* endPosition)
{
    HRESULT hr{};
    ::Line3D* pNativeObj{};
    hr = ::Line3D::Create(
        graphicsDevice->Handle,
        *startPosition,
        *endPosition,
        &pNativeObj
    );
    if (FAILED(hr))
    {
        throw gcnew System::Exception("Can not Create Line3D!");
    }
    m_pNativeObject = pNativeObj;
}

MapToolRender::Line3D::Line3D(Line3D^ rhs):
    RenderObject{rhs}
{
}

auto MapToolRender::Line3D::Clone() -> RenderObject^ 
{
    return gcnew Line3D{ this };
}
