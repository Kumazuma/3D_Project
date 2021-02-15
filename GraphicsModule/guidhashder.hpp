#pragma once
#include<windows.h>
#include<type_traits>
struct GUIDEx : public GUID
{
	GUIDEx(GUID const& guid);
	size_t hashValue_;
};

namespace std
{

	template<>
	struct hash<GUID>
	{
		_NODISCARD size_t operator()(GUID const& _Keyval) const noexcept {
			size_t res{};
			constexpr size_t s{ sizeof(_Keyval) };
			size_t const* p{ reinterpret_cast<size_t const*>(&_Keyval) };
			for (int i = 0; i < s / sizeof(size_t); ++i)
			{
				res = res ^ *p;
				++p;
			}
			return res;
		}
	};
	template<>
	struct hash<GUIDEx>
	{
		_NODISCARD size_t operator()(GUIDEx const& _Keyval) const noexcept {
			return _Keyval.hashValue_;
		}
	};

}

inline GUIDEx::GUIDEx(GUID const& guid) :
	GUID{ guid },
	hashValue_{ std::hash<GUID>{}(guid) }
{

}