#pragma once
#include<xhash>
#include<DirectXMath.h>
namespace std
{
	template<>
	class hash<DirectX::XMFLOAT3>
	{
		static size_t _Do_hash(const DirectX::XMFLOAT3& _Keyval) noexcept {
			return
				_Hash_representation<float>(_Keyval.x == 0.f ? 0.f : _Keyval.x) ^
				_Hash_representation<float>(_Keyval.y == 0.f ? 0.f : _Keyval.y) ^
				_Hash_representation<float>(_Keyval.x == 0.f ? 0.f : _Keyval.y);
		}
	};
	template<>
	class equal_to<DirectX::XMFLOAT3>
	{
		constexpr bool operator()(const DirectX::XMFLOAT3& _Left, const DirectX::XMFLOAT3& _Right) const {
			return
				_Left.x == _Right.x &&
				_Left.y == _Right.y &&
				_Left.z == _Right.z;
		}
	};
	template<>
	class hash<DirectX::XMFLOAT2>
	{
		static size_t _Do_hash(const DirectX::XMFLOAT2& _Keyval) noexcept {
			return
				_Hash_representation<float>(_Keyval.x == 0.f ? 0.f : _Keyval.x) ^
				_Hash_representation<float>(_Keyval.y == 0.f ? 0.f : _Keyval.y);
		}
	};
	template<>
	class equal_to<DirectX::XMFLOAT2>
	{
		constexpr bool operator()(const DirectX::XMFLOAT2& _Left, const DirectX::XMFLOAT2& _Right) const {
			return
				_Left.x == _Right.x &&
				_Left.y == _Right.y;
		}
	};
}