#pragma once
#undef max
#undef min
#define NOMINMAX
#include<DirectXMath.h>
#include"typedef.hpp"
#include<numeric>
class BoundSphere
{
public:
	BoundSphere() :m_center{}, m_radius{} {}
	BoundSphere(DirectX::XMFLOAT3 const& center, f32 radius);
	
	static auto __vectorcall Calculate(DirectX::XMVECTOR min, DirectX::XMVECTOR max)->BoundSphere;
	template<typename FLOAT3IT>
	static inline auto Calculate(FLOAT3IT begin, FLOAT3IT end)->BoundSphere;
	auto GetCenter()const->DirectX::XMFLOAT3 const&;
	auto GetRadius()const->f32;
	template<typename FLOAT3>
	inline auto IsInSphere(FLOAT3 const& pos)const->bool;
	auto __vectorcall IsVectorInSphere(DirectX::XMVECTOR pos)const->bool;
private:
	DirectX::XMFLOAT3 m_center;
	f32 m_radius;
};

template<typename FLOAT3IT>
inline auto BoundSphere::Calculate(FLOAT3IT begin, FLOAT3IT end) -> BoundSphere
{
	using namespace DirectX;
	constexpr f32 f32_min{ std::numeric_limits<f32>::min() };
	constexpr f32 f32_max{ std::numeric_limits<f32>::max() };
	int int_max{ std::numeric_limits<int>::max() };
	int int_int{ std::numeric_limits<int>::min() };

	XMVECTOR vMin{ XMVectorSet(f32_max, f32_max, f32_max, f32_max) };
	XMVECTOR vMax{ XMVectorSet(f32_min,f32_min, f32_min, f32_min) };
	for (auto it = begin; it != end; ++it)
	{
		XMVECTOR vPos{ XMVectorSet(it->x,it->y,it->z, 1.f) };
		vMin = XMVectorMin(vMin, vPos);
		vMax = XMVectorMax(vMax, vPos);
	}
	return BoundSphere::Calculate(vMin, vMax);
}

template<typename FLOAT3>
inline auto BoundSphere::IsInSphere(FLOAT3 const& pos) const -> bool
{
	return IsVectorInSphere(XMVectorSet(pos.x, pos.y, pos.z, 1.f));
}
