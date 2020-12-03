#include "pch.h"
#include "typedef.hpp"
#include "Ray.h"

using namespace DirectX;
Ray::Ray(XMFLOAT3 const& rayOrigin, XMFLOAT3 const& focusAt)
{
	m_rayOrigin = rayOrigin;
	XMStoreFloat3(&m_rayDir, XMVector3Normalize(XMLoadFloat3(&focusAt) - XMLoadFloat3(&rayOrigin)));
}

auto Ray::GetRayOrigin() const -> DirectX::XMFLOAT3 const&
{
	return m_rayOrigin;
}

auto Ray::GetRayDirection() const -> DirectX::XMFLOAT3 const&
{
	return m_rayDir;
}
auto Ray::GetPosition(f32 t) const -> DirectX::XMFLOAT3
{
	XMFLOAT3 position{};
	XMStoreFloat3(&position, XMLoadFloat3(&m_rayOrigin) + XMLoadFloat3(&m_rayDir) * t);
	return position;
}