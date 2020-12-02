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