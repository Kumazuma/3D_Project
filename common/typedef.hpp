#pragma once
#include<DirectXMath.h>
#include<stddef.h>
using HLSLfloat4 = DirectX::XMFLOAT4;
using HLSLfloat2 = DirectX::XMFLOAT2;
using HLSLMatrix = DirectX::XMMATRIX;
using f32x2 = DirectX::XMFLOAT2;
using f32x3 = DirectX::XMFLOAT3;
using f32x3A = DirectX::XMFLOAT3A;
using f32x4 = DirectX::XMFLOAT4;
using f32x44 = DirectX::XMFLOAT4X4;
using i64 = long long;
using i32 = long;
using i16 = short;
using i8 = char;
using usize = size_t;
using u64 = unsigned long long;
using u32 = unsigned int;
using u16 = unsigned short;
using u8 = unsigned char;

using f32 = float;
using f64 = double;



#ifndef _BEGIN_NS
#define _BEGIN_NS(ns) namespace ns {
#endif
#ifndef _END_NS
#define _END_NS }
#endif

#ifndef fn_
#define fn_ auto
#endif
inline auto __vectorcall StoreF32(DirectX::XMVECTOR va)->f32
{
	f32 v;
	DirectX::XMStoreFloat(&v, va);
	return v;
}
inline auto __vectorcall StoreF32x3(DirectX::XMVECTOR va)->f32x3
{
	f32x3 v;
	DirectX::XMStoreFloat3(&v, va);
	return v;
}
inline auto __vectorcall StoreF32x44(DirectX::XMMATRIX va)->f32x44
{
	f32x44 v;
	DirectX::XMStoreFloat4x4(&v, va);
	return v;
}
inline auto LoadF32x44(f32x44 const& va)->DirectX::XMMATRIX
{
	return DirectX::XMLoadFloat4x4(&va);
}
template<typename T>
inline auto LoadF32x3(T&& val)->DirectX::XMVECTOR
{
	return XMLoadFloat3(&val);
}