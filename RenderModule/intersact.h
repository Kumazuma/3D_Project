#pragma once
#include<DirectXMath.h>
#include"typedef.hpp"
auto __vectorcall Intersact(DirectX::XMVECTOR rayAt, DirectX::XMVECTOR rayDir, DirectX::XMVECTOR sphareCenter, f32 radius, f32* pOut)->bool;
