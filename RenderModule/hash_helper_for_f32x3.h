#pragma once
#include<xhash>
#include<DirectXMath.h>
namespace std
{
	template<>
	struct hash<DirectX::XMFLOAT3>
	{
		size_t operator()(const DirectX::XMFLOAT3& _Keyval)const noexcept {
			return
				std::_Hash_representation<float>(_Keyval.x == 0.f ? 0.f : _Keyval.x) ^
				std::_Hash_representation<float>(_Keyval.y == 0.f ? 0.f : _Keyval.y) ^
				std::_Hash_representation<float>(_Keyval.x == 0.f ? 0.f : _Keyval.y);
		}
	};
	template<>
	struct equal_to<DirectX::XMFLOAT3>
	{
		_CXX17_DEPRECATE_ADAPTOR_TYPEDEFS typedef DirectX::XMFLOAT3 _FIRST_ARGUMENT_TYPE_NAME;
		_CXX17_DEPRECATE_ADAPTOR_TYPEDEFS typedef DirectX::XMFLOAT3 _SECOND_ARGUMENT_TYPE_NAME;
		_CXX17_DEPRECATE_ADAPTOR_TYPEDEFS typedef bool _RESULT_TYPE_NAME;
		bool operator()(const DirectX::XMFLOAT3& _Left, const DirectX::XMFLOAT3& _Right) const {

			return ::equal(_Left, _Right);
		}
	};
	template<>
	struct hash<DirectX::XMFLOAT3A>
	{
		size_t operator()(const DirectX::XMFLOAT3A& _Keyval)const noexcept {
			return
				std::_Hash_representation<float>(_Keyval.x == 0.f ? 0.f : _Keyval.x) ^
				std::_Hash_representation<float>(_Keyval.y == 0.f ? 0.f : _Keyval.y) ^
				std::_Hash_representation<float>(_Keyval.x == 0.f ? 0.f : _Keyval.y);
		}
	};
	template<>
	struct equal_to<DirectX::XMFLOAT3A>
	{
		_CXX17_DEPRECATE_ADAPTOR_TYPEDEFS typedef DirectX::XMFLOAT3 _FIRST_ARGUMENT_TYPE_NAME;
		_CXX17_DEPRECATE_ADAPTOR_TYPEDEFS typedef DirectX::XMFLOAT3 _SECOND_ARGUMENT_TYPE_NAME;
		_CXX17_DEPRECATE_ADAPTOR_TYPEDEFS typedef bool _RESULT_TYPE_NAME;
		bool operator()(const DirectX::XMFLOAT3A& _Left, const DirectX::XMFLOAT3A& _Right) const {

			return ::equal(_Left, _Right);
		}
	};
	template<>
	struct hash<DirectX::XMFLOAT2>
	{
		size_t operator()(const DirectX::XMFLOAT2& _Keyval)const noexcept {
			return
				std::_Hash_representation<float>(_Keyval.x == 0.f ? 0.f : _Keyval.x) ^
				std::_Hash_representation<float>(_Keyval.y == 0.f ? 0.f : _Keyval.y) ^
				std::_Hash_representation<float>(_Keyval.x == 0.f ? 0.f : _Keyval.y);
		}
	};
	template<>
	struct equal_to<DirectX::XMFLOAT2>
	{
		_CXX17_DEPRECATE_ADAPTOR_TYPEDEFS typedef DirectX::XMFLOAT2 _FIRST_ARGUMENT_TYPE_NAME;
		_CXX17_DEPRECATE_ADAPTOR_TYPEDEFS typedef DirectX::XMFLOAT2 _SECOND_ARGUMENT_TYPE_NAME;
		_CXX17_DEPRECATE_ADAPTOR_TYPEDEFS typedef bool _RESULT_TYPE_NAME;
		bool operator()(const DirectX::XMFLOAT2& _Left, const DirectX::XMFLOAT2& _Right) const {

			return ::equal(_Left, _Right);
		}
	};
	template<>
	struct hash<DirectX::XMFLOAT2A>
	{
		size_t operator()(const DirectX::XMFLOAT2A& _Keyval)const noexcept {
			return
				std::_Hash_representation<float>(_Keyval.x == 0.f ? 0.f : _Keyval.x) ^
				std::_Hash_representation<float>(_Keyval.y == 0.f ? 0.f : _Keyval.y) ^
				std::_Hash_representation<float>(_Keyval.x == 0.f ? 0.f : _Keyval.y);
		}
	};
	template<>
	struct equal_to<DirectX::XMFLOAT2A>
	{
		_CXX17_DEPRECATE_ADAPTOR_TYPEDEFS typedef DirectX::XMFLOAT2A _FIRST_ARGUMENT_TYPE_NAME;
		_CXX17_DEPRECATE_ADAPTOR_TYPEDEFS typedef DirectX::XMFLOAT2A _SECOND_ARGUMENT_TYPE_NAME;
		_CXX17_DEPRECATE_ADAPTOR_TYPEDEFS typedef bool _RESULT_TYPE_NAME;
		bool operator()(const DirectX::XMFLOAT2A& _Left, const DirectX::XMFLOAT2A& _Right) const {

			return ::equal(_Left, _Right);
		}
	};
}