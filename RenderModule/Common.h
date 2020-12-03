#pragma once
#ifdef DLL_EXPORT
#define DLL_CLASS _declspec(dllexport)
#else
#define DLL_CLASS _declspec(dllimport)
#endif
#include "typedef.hpp"

inline constexpr bool operator == (DirectX::XMFLOAT3 const& lhs, DirectX::XMFLOAT3 const& rhs)
{
	return
		lhs.x == rhs.x &&
		lhs.y == rhs.y &&
		lhs.z == rhs.z;
}
inline constexpr bool operator != (DirectX::XMFLOAT3 const& lhs, DirectX::XMFLOAT3 const& rhs)
{
	return
		lhs.x != rhs.x ||
		lhs.y != rhs.y ||
		lhs.z != rhs.z;
}
inline constexpr bool operator == (DirectX::XMFLOAT2 const& lhs, DirectX::XMFLOAT2 const& rhs)
{
	return
		lhs.x == rhs.x &&
		lhs.y == rhs.y;
}
inline constexpr bool operator != (DirectX::XMFLOAT2 const& lhs, DirectX::XMFLOAT2 const& rhs)
{
	return
		lhs.x != rhs.x ||
		lhs.y != rhs.y;
}
inline constexpr auto equal(DirectX::XMFLOAT3 const& lhs, DirectX::XMFLOAT3 const& rhs)->bool
{
	return lhs == rhs;
}
inline constexpr auto equal(DirectX::XMFLOAT2 const& lhs, DirectX::XMFLOAT2 const& rhs)->bool
{
	return lhs == rhs;
}
struct StoreVec
{
	StoreVec(DirectX::XMVECTOR v) :v{ v } {}
	inline operator DirectX::XMFLOAT3A()const&&
	{
		DirectX::XMFLOAT3A var{};
		DirectX::XMStoreFloat3A(&var, v);
		return var;
	}
	inline operator DirectX::XMFLOAT2A()const&&
	{
		DirectX::XMFLOAT2A var{};
		DirectX::XMStoreFloat2A(&var, v);
		return var;
	}
	inline operator DirectX::XMFLOAT3()const&&
	{
		DirectX::XMFLOAT3 var{};
		DirectX::XMStoreFloat3(&var, v);
		return var;
	}
	inline operator DirectX::XMFLOAT2()const&&
	{
		DirectX::XMFLOAT2 var{};
		DirectX::XMStoreFloat2(&var, v);
		return var;
	}
	inline operator f32()const&&
	{
		f32 var{};
		DirectX::XMStoreFloat(&var, v);
		return var;
	}
	DirectX::XMVECTOR v;
};
