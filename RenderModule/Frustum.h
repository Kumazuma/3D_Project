#pragma once
#include<DirectXMath.h>
class Frustum
{
public:
	auto __vectorcall MakeFrustum(DirectX::XMMATRIX view, DirectX::XMMATRIX proj)->void;
	/// <summary>
	/// ���� ������ �ִ� ���� ����ü �ȿ� �ִ� �� �Ǻ��Ѵ�.
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	auto __vectorcall Intersact(DirectX::XMVECTOR pos)const->bool;
	/// <summary>
	/// ���� ������ �ִ� ���� ����ü �ȿ� �ִ� �� �Ǻ��Ѵ�.
	/// </summary>
	/// <param name="pos">���� ��ġ</param>
	/// <returns></returns>
	inline auto Intersact(DirectX::XMFLOAT3 const* pos)const->bool;
	/// <summary>
	/// ���� ������ �ִ� ���� ����ü�� ��ġ�� �� �Ǻ��Ѵ�.
	/// </summary>
	/// <param name="pos">���� �߽� ��ǥ</param>
	/// <param name="radius">���� ������</param>
	/// <returns></returns>
	auto __vectorcall Intersact(DirectX::XMVECTOR pos, float radius)const->bool;
	/// <summary>
	/// ���� ������ �ִ� ���� ����ü�� ��ġ�� �� �Ǻ��Ѵ�.
	/// </summary>
	/// <param name="pos">���� �߽� ��ǥ</param>
	/// <param name="radius">���� ������</param>
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