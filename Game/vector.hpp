#pragma once
#include<DirectXMath.h>
inline float ToFloat(DirectX::XMVECTOR v);
inline DirectX::XMFLOAT3 ToFloat3(DirectX::XMVECTOR v);
inline DirectX::XMFLOAT4 ToFloat4(DirectX::XMVECTOR v);
inline DirectX::XMVECTOR From(const DirectX::XMFLOAT4& f4);
inline DirectX::XMVECTOR From(const DirectX::XMFLOAT3& f3);
inline DirectX::XMVECTOR From(const DirectX::XMFLOAT2& f2);
template<typename _floatT>
inline _floatT To(DirectX::XMVECTOR v);
struct FromVec
{
	FromVec(DirectX::XMVECTOR v) :v{ v } {}
	inline operator float()const
	{
		float res{};
		DirectX::XMStoreFloat(&res, v);
		return res;
	}
	inline operator DirectX::XMFLOAT2() const
	{
		DirectX::XMFLOAT2 res{};
		DirectX::XMStoreFloat2(&res, v);
		return res;
	}
	inline operator DirectX::XMFLOAT3() const
	{
		DirectX::XMFLOAT3 res{};
		DirectX::XMStoreFloat3(&res, v);
		return res;
	}
	inline operator DirectX::XMFLOAT4() const
	{
		DirectX::XMFLOAT4 res{};
		DirectX::XMStoreFloat4(&res, v);
		return res;
	}
	DirectX::XMVECTOR v;
};
template<>
inline DirectX::XMFLOAT4 To<DirectX::XMFLOAT4>(DirectX::XMVECTOR v)
{
	DirectX::XMFLOAT4 res{};
	DirectX::XMStoreFloat4(&res, v);
	return res;
}
template<>
inline DirectX::XMFLOAT3 To<DirectX::XMFLOAT3>(DirectX::XMVECTOR v)
{
	DirectX::XMFLOAT3 res{};
	DirectX::XMStoreFloat3(&res, v);
	return res;
}
template<>
inline DirectX::XMFLOAT2 To<DirectX::XMFLOAT2>(DirectX::XMVECTOR v)
{
	DirectX::XMFLOAT2 res{};
	DirectX::XMStoreFloat2(&res, v);
	return res;
}
template<>
inline float To<float>(DirectX::XMVECTOR v)
{
	float res{};
	DirectX::XMStoreFloat(&res, v);
	return res;
}

//x, y, z의 세 요소를 길이를 구한다.
inline float length(DirectX::XMVECTOR op1)
{
	return ToFloat(DirectX::XMVector3Length(op1));
}
//x, y, z세 요소의 단위벡터로 변환시킨다.
inline DirectX::XMVECTOR Normalization(DirectX::XMVECTOR op1)
{
	return DirectX::XMVector3Normalize(op1);
}
inline float ToFloat(DirectX::XMVECTOR v)
{
	float x{ 0.f };
	DirectX::XMStoreFloat(&x, v);
	return x;
}

inline DirectX::XMFLOAT3 ToFloat3(DirectX::XMVECTOR v)
{
	DirectX::XMFLOAT3 f3{};
	DirectX::XMStoreFloat3(&f3, v);
	return f3;
}

inline DirectX::XMFLOAT4 ToFloat4(DirectX::XMVECTOR v)
{
	DirectX::XMFLOAT4 f4{};
	DirectX::XMStoreFloat4(&f4, v);
	return f4;
}

inline DirectX::XMVECTOR From(const DirectX::XMFLOAT4& f4)
{
	return DirectX::XMLoadFloat4(&f4);
}

inline DirectX::XMVECTOR From(const DirectX::XMFLOAT3& f3)
{
	return DirectX::XMLoadFloat3(&f3);
}

inline DirectX::XMVECTOR From(const DirectX::XMFLOAT2& f2)
{
	return DirectX::XMLoadFloat2(&f2);
}
