#pragma once
#include<DirectXMath.h>
#include"Common.h"
class DLL_CLASS Ray
{
public:
	Ray(DirectX::XMFLOAT3 const& RayOrigin, DirectX::XMFLOAT3 const& FocusAt);
	auto GetRayOrigin()const->DirectX::XMFLOAT3 const&;
	auto GetRayDirection()const->DirectX::XMFLOAT3 const&;
	auto GetPosition(f32 t)const->DirectX::XMFLOAT3;
private:
	DirectX::XMFLOAT3 m_rayOrigin;
	DirectX::XMFLOAT3 m_rayDir;
};