#include "pch.h"
#include "RenderObject.h"

RenderObject::RenderObject():
    m_transform{}
{
    for (size_t i = 0; i < 4; ++i)
    {
        m_transform(i, i) = 1.f;
    }
}

auto RenderObject::SetTransform(DirectX::XMFLOAT4X4 const& transform) -> void
{
    m_transform = transform;
}


auto RenderObject::GetTransform() const -> DirectX::XMFLOAT4X4 const&
{
    return m_transform;
}