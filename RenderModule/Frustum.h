#pragma once
#include<DirectXMath.h>
#include"Common.h"
class Frustum
{
public:
	inline auto __vectorcall MakeFrustum(DirectX::XMMATRIX view, DirectX::XMMATRIX proj)->void;
	/// <summary>
	/// 월드 공간에 있는 점이 절두체 안에 있는 지 판별한다.
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	inline auto __vectorcall Intersact(DirectX::XMVECTOR pos)const->bool;
	/// <summary>
	/// 월드 공간에 있는 점이 절두체 안에 있는 지 판별한다.
	/// </summary>
	/// <param name="pos">점의 위치</param>
	/// <returns></returns>
	inline auto Intersact(DirectX::XMFLOAT3 const* pos)const->bool;
	/// <summary>
	/// 월드 공간에 있는 구가 절두체와 겹치는 지 판별한다.
	/// </summary>
	/// <param name="pos">구의 중심 좌표</param>
	/// <param name="radius">구의 반지름</param>
	/// <returns></returns>
	inline auto __vectorcall Intersact(DirectX::XMVECTOR pos, float radius)const->bool;
	/// <summary>
	/// 월드 공간에 있는 구가 절두체와 겹치는 지 판별한다.
	/// </summary>
	/// <param name="pos">구의 중심 좌표</param>
	/// <param name="radius">구의 반지름</param>
	/// <returns></returns>
	inline auto Intersact(DirectX::XMFLOAT3 const* pos, float radius)const->bool;

private:
	DirectX::XMFLOAT4 m_sidePlanes[4];
	DirectX::XMFLOAT4 m_farPlane;

};
inline auto Frustum::Intersact(DirectX::XMFLOAT3 const* pos)const->bool
{
	return Intersact(DirectX::XMLoadFloat3(pos));
}
inline auto Frustum::Intersact(DirectX::XMFLOAT3 const* pos, float radius)const->bool
{
	return Intersact(DirectX::XMLoadFloat3(pos), radius);
}

inline auto __vectorcall Frustum::MakeFrustum(DirectX::XMMATRIX view, DirectX::XMMATRIX proj)->void
{
	using namespace DirectX;
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

inline auto __vectorcall Frustum::Intersact(DirectX::XMVECTOR pos) const -> bool
{
	using namespace DirectX;

	XMMATRIX mSides;
	for (u32 i = 0; i < 4; ++i)
	{
		mSides.r[i] = XMLoadFloat4(m_sidePlanes + i);
	}
	mSides = XMMatrixTranspose(mSides);
	pos = XMVectorSetW(pos, 1.f);
	XMVECTOR vRes = XMVector4Transform(pos, mSides);
	//각 면과 내적해서 모두 음수가 아니면 절두체 밖에 있다.
	bool res = XMVector4LessOrEqual(vRes, XMVectorSet(0.f, 0.f, 0.f, 0.f));
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
inline auto __vectorcall Frustum::Intersact(DirectX::XMVECTOR pos, float radius)const->bool
{
	using namespace DirectX;
	XMMATRIX mSides;
	for (u32 i = 0; i < 4; ++i)
	{
		mSides.r[i] = XMLoadFloat4(m_sidePlanes + i);
	}
	mSides = XMMatrixTranspose(mSides);
	pos = XMVectorSetW(pos, 1.f);
	XMVECTOR vRes = XMVector4Transform(pos, mSides);

	//각 면과 내적해서 모두 음수가 아니면 절두체 밖에 있다.
	bool res = XMVector4LessOrEqual(vRes, XMVectorSet(radius, radius, radius, radius));
	//side[1] dot pos > -radius
	if (!res)
	{
		return false;
	}
	XMVECTOR vFarPlane{ XMLoadFloat4(&m_farPlane) };
	float t{};
	XMStoreFloat(&t, XMVector4Dot(vFarPlane, pos));
	if (t < -radius)
	{
		return false;
	}
	return true;
}
