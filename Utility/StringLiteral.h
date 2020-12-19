#pragma once
#include<cstring>
#include<array>
namespace std
{
	template<typename T>
	struct hash;
}
template<typename _chType>
class StringLiteral
{
	friend struct std::hash< StringLiteral<_chType> >;
public:
	constexpr StringLiteral(_chType const* const _literal);
	constexpr StringLiteral(StringLiteral const& rhs);
public:
	auto constexpr operator ==(StringLiteral const& rhs)const->bool;
	auto constexpr operator ==(_chType const* const _literal)const->bool;
	auto constexpr operator !=(StringLiteral const& rhs)const->bool;
	auto constexpr operator !=(_chType const* const _literal)const->bool;

	constexpr operator _chType const* ()const;
	auto constexpr GetLiteral()const->_chType const*;
	auto constexpr GetLength()const->size_t;
	auto operator <(StringLiteral const& rhs)const->bool;
	auto operator <(_chType const* const rhs)const->bool;
	auto operator >(StringLiteral const& rhs)const->bool;
	auto operator >(_chType const* const rhs)const->bool;
private:
	static constexpr auto GetLength(_chType const* const)->size_t;
	constexpr auto IsSame(_chType const* rhs, size_t len = -1)const->bool;
	static constexpr auto Hash(_chType const* const)->size_t;
private:
	_chType const* const m_literal;
	size_t const m_length;
	size_t const m_hash;
};
template<typename CH>
struct StringHelper{};
template<>
struct StringHelper<wchar_t>
{
	using CH = wchar_t;
	static auto StrCmp(CH const* lhs, CH const* rhs)->int
	{
		return wcscmp(lhs, rhs);
	}
};
template<>
struct StringHelper<char>
{
	using CH = char;
	static auto StrCmp(CH const* lhs, CH const* rhs)->int
	{
		return strcmp(lhs, rhs);
	}
};
template<typename _chType>
inline constexpr StringLiteral<_chType>::StringLiteral(_chType const* const _literal):
	m_literal{_literal},
	m_length{GetLength(_literal)},
	m_hash{ Hash(_literal)}
{
}

template<typename _chType>
inline constexpr StringLiteral<_chType>::StringLiteral(StringLiteral const& rhs):
	m_literal{ rhs.m_literal },
	m_length{ rhs.m_length },
	m_hash{rhs.m_hash}
{
}

template<typename _chType>
inline constexpr auto StringLiteral<_chType>::operator==(StringLiteral const& rhs) const -> bool
{
	if (rhs.m_literal == m_literal)return true;
	
	return IsSame(rhs.m_literal, rhs.m_length);
}

template<typename _chType>
inline constexpr auto StringLiteral<_chType>::operator==(_chType const* const _literal) const -> bool
{
	if (_literal == m_literal)return true;
	return IsSame(_literal);
}
template<typename _chType>
inline constexpr auto StringLiteral<_chType>::operator!=(StringLiteral const& rhs) const -> bool
{
	if (rhs.m_literal != m_literal)return true;

	return !IsSame(rhs.m_literal, rhs.m_length);
}

template<typename _chType>
inline constexpr auto StringLiteral<_chType>::operator!=(_chType const* const _literal) const -> bool
{
	if (_literal != m_literal)return true;
	return !IsSame(_literal);
}

template<typename _chType>
inline constexpr StringLiteral<_chType>::operator _chType const* () const
{
	return m_literal;
}

template<typename _chType>
inline constexpr auto StringLiteral<_chType>::GetLiteral() const -> _chType const*
{
	return m_literal;
}

template<typename _chType>
inline  auto StringLiteral<_chType>::operator<(StringLiteral const& rhs) const -> bool
{
	if (rhs.m_literal == m_literal)return false;
	return StringHelper<_chType>::StrCmp(m_literal, rhs.m_literal) < 0;
}

template<typename _chType>
inline  auto StringLiteral<_chType>::operator<(_chType const* const rhs) const -> bool
{
	if (rhs == m_literal)return false;
	return StringHelper<_chType>::StrCmp(m_literal, rhs) < 0;
}

template<typename _chType>
inline  auto StringLiteral<_chType>::operator>(StringLiteral const& rhs) const -> bool
{
	if (rhs.m_literal == m_literal)return false;
	return StringHelper<_chType>::StrCmp(m_literal, rhs.m_literal) > 0;
}

template<typename _chType>
inline  auto StringLiteral<_chType>::operator>(_chType const* const rhs) const -> bool
{
	if (rhs == m_literal)return false;
	return StringHelper<_chType>::StrCmp(m_literal, rhs) > 0;
}

template<typename _chType>
inline constexpr auto StringLiteral<_chType>::GetLength(_chType const* const literal) -> size_t
{
	size_t i{};
	while (literal[i] != static_cast<_chType>(0)) ++i;
	return i;
}
template<typename _chType>
inline constexpr auto StringLiteral<_chType>::IsSame(_chType const* rhs, size_t len) const -> bool
{
	if (rhs == m_literal)return true;
	if (len == -1) len = GetLength(rhs);
	if (len != m_length) return false;
	for (auto i = 0; i < len; ++i)
	{
		if (rhs[i] != m_literal[i])return false;
	}
	return true;
}
template<typename _chType>
inline constexpr auto StringLiteral<_chType>::GetLength()const -> size_t
{
	return m_length;
}
template<typename _chType>
inline constexpr auto StringLiteral<_chType>::Hash(_chType const* const str) -> size_t
{
	constexpr size_t arraySize{ sizeof(size_t) / sizeof(_chType) };
	size_t length{ GetLength(str) };
	size_t hashRes{};

	_chType rHash[arraySize]{  };
	for (size_t i = 0; i < length; ++i)
	{
		rHash[i % arraySize] ^= str[i];
	}
	for (size_t i = 0; i < arraySize; ++i)
	{
		hashRes |= rHash[arraySize - i - 1];
		hashRes = hashRes << (sizeof(_chType) * 8);
	}
	return hashRes;
}
namespace std
{
	template<typename _chType>
	struct hash<StringLiteral<_chType> >
	{
		auto operator()(StringLiteral<_chType> const& rhs)const noexcept->size_t
		{
			return rhs.m_hash;
		}
	};
}
