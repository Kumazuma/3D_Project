#pragma once
#include<DirectXMath.h>
class Frustum
{
public:
	auto __vectorcall MakeFrustum(DirectX::XMMATRIX view, DirectX::XMMATRIX proj)->void;
	auto __vectorcall Intersact(DirectX::XMVECTOR pos)const->bool;

private:
	DirectX::XMFLOAT4 m_sidePlanes[4];
	DirectX::XMFLOAT4 m_farPlane;
};