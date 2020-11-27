#pragma once
#include<DirectXMath.h>
class Frustum
{
public:
	auto __vectorcall MakeFrustum(DirectX::XMMATRIX view, DirectX::XMMATRIX proj)->void;
	/// <summary>
	/// 월드 공간에 있는 점이 절두체 안에 있는 지 판별한다.
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	auto __vectorcall Intersact(DirectX::XMVECTOR pos)const->bool;
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
	auto __vectorcall Intersact(DirectX::XMVECTOR pos, float radius)const->bool;
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