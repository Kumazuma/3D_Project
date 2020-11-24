#include "pch.h"
#include "Frustum.h"
#include "typedef.hpp"
using namespace DirectX;
void __vectorcall Frustum::MakeFrustum(DirectX::XMMATRIX view, DirectX::XMMATRIX proj)
{
	XMMATRIX mViewInverse{ XMMatrixInverse(nullptr, view) };
	XMMATRIX mProjInverse{ XMMatrixInverse(nullptr, proj) };
	XMVECTOR vCamaraPosition{ mViewInverse.r[3] };
	XMVECTOR vPoints[4]{
		XMVectorSet(1.0f, 1.0f, 1.f, 0.f),//0
		XMVectorSet(1.0f, -1.f, 1.f, 0.f),//1
		XMVectorSet(-1.f, -1.f, 1.f, 0.f),//2
		XMVectorSet(-1.f, 1.0f, 1.f, 0.f),//3
	};
	vPoints[0] = XMVector3TransformCoord(vPoints[0], mProjInverse);
	vPoints[0] = XMVector3TransformCoord(vPoints[0], mViewInverse);

	vPoints[1] = XMVector3TransformCoord(vPoints[1], mProjInverse);
	vPoints[1] = XMVector3TransformCoord(vPoints[1], mViewInverse);

	vPoints[2] = XMVector3TransformCoord(vPoints[2], mProjInverse);
	vPoints[2] = XMVector3TransformCoord(vPoints[2], mViewInverse);
	
	vPoints[3] = XMVector3TransformCoord(vPoints[3], mProjInverse);
	vPoints[3] = XMVector3TransformCoord(vPoints[3], mViewInverse);

	XMStoreFloat4(m_sidePlanes + 0, XMPlaneFromPoints(vCamaraPosition, vPoints[0], vPoints[1]));
	XMStoreFloat4(m_sidePlanes + 1, XMPlaneFromPoints(vCamaraPosition, vPoints[1], vPoints[2]));
	XMStoreFloat4(m_sidePlanes + 2, XMPlaneFromPoints(vCamaraPosition, vPoints[2], vPoints[3]));
	XMStoreFloat4(m_sidePlanes + 3, XMPlaneFromPoints(vCamaraPosition, vPoints[3], vPoints[0]));
	XMStoreFloat4(&m_farPlane, XMPlaneFromPoints(vPoints[0], vPoints[1], vPoints[2]));
}

auto __vectorcall Frustum::Intersact(DirectX::XMVECTOR pos) const -> bool
{
	XMMATRIX mSides;
	for (u32 i = 0; i < 4; ++i)
	{
		mSides.r[i] = XMLoadFloat4(m_sidePlanes + i);
	}
	mSides = XMMatrixTranspose(mSides);
	pos = XMVectorSetW(pos, 1.f);
	XMVECTOR vRes = XMVector4Transform(pos, mSides);
	//각 면과 내적해서 모두 음수가 아니면 절두체 밖에 있다.
	bool res = XMVector4Less(vRes, XMVectorSet(0.f, 0.f, 0.f, 0.f));
	if (!res)
	{
		return false;
	}
	XMVECTOR vFarPlane{ XMLoadFloat4(&m_farPlane) };
	float t{};
	XMStoreFloat(&t, XMVector4Dot(vFarPlane, pos));
	if (t < 0.f)
	{
		return false;
	}
	return true;
}
