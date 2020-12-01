#pragma once
#ifdef DLL_EXPORT
#define DLL_CLASS _declspec(dllexport)
#else
#define DLL_CLASS _declspec(dllimport)
#endif
#include "typedef.hpp"
inline bool operator == (DirectX::XMFLOAT3 const& lhs, DirectX::XMFLOAT3 const& rhs)
{
	return
		lhs.x == rhs.x &&
		lhs.y == rhs.y &&
		lhs.z == rhs.z;
}
inline bool operator != (DirectX::XMFLOAT3 const& lhs, DirectX::XMFLOAT3 const& rhs)
{
	return
		lhs.x != rhs.x ||
		lhs.y != rhs.y ||
		lhs.z != rhs.z;
}
inline bool operator == (DirectX::XMFLOAT2 const& lhs, DirectX::XMFLOAT2 const& rhs)
{
	return
		lhs.x == rhs.x &&
		lhs.y == rhs.y;
}
inline bool operator != (DirectX::XMFLOAT2 const& lhs, DirectX::XMFLOAT2 const& rhs)
{
	return
		lhs.x != rhs.x ||
		lhs.y != rhs.y;
}