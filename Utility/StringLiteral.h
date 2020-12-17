#pragma once
#include<cstring>

template<typename _chType>
class StringLiteral
{
public:
	constexpr StringLiteral(_chType const* const _literal);
	StringLiteral(StringLiteral const& rhs);
public:
	auto operator ==(StringLiteral const& rhs)const->bool;
	auto operator ==(_chType const* const _literal)const->bool;
	constexpr operator _chType const* ()const;
	auto constexpr GetLiteral()const->_chType const*;
	auto constexpr operator <(StringLiteral const& rhs)const->bool;
	auto constexpr operator <(_chType const* const rhs)const->bool;
	auto constexpr operator >(StringLiteral const& rhs)const->bool;
	auto constexpr operator >(_chType const* const rhs)const->bool;
private:
	static constexpr auto GetLength(_chType const* const)->size_t;
private:
	_chType const* const m_literal;
	size_t const m_length;
};
template<typename CH>
struct StringHelper{};
template<>
struct StringHelper<wchar_t>
{
	using CH = wchar_t;
	static auto StrCmp(CH const* lhs, CH const* rhs)->bool
	{
		return wcscmp(lhs, rhs);
	}
};
template<>
struct StringHelper<char>
{
	using CH = char;
	static auto StrCmp(CH const* lhs, CH const* rhs)->bool
	{
		return strcmp(lhs, rhs);
	}
};
template<typename _chType>
inline constexpr StringLiteral<_chType>::StringLiteral(_chType const* const _literal):
	m_literal{_literal},
	m_length{GetLength(_literal)}
{
}

template<typename _chType>
inline StringLiteral<_chType>::StringLiteral(StringLiteral const& rhs):
	m_literal{ rhs.m_literal },
	m_length{ rhs.m_length }
{
}

template<typename _chType>
inline auto StringLiteral<_chType>::operator==(StringLiteral const& rhs) const -> bool
{
	if (rhs.m_literal == m_literal)return true;
	
	return StringHelper<_chType>::StrCmp(rhs.m_literal, m_literal) == 0;
}

template<typename _chType>
inline auto StringLiteral<_chType>::operator==(_chType const* const _literal) const -> bool
{
	if (_literal == m_literal)return true;
	return StringHelper<_chType>::StrCmp(_literal, m_literal) == 0;
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
inline constexpr auto StringLiteral<_chType>::operator<(StringLiteral const& rhs) const -> bool
{
	if (rhs.m_literal == m_literal)return false;
	return StringHelper<_chType>::StrCmp(m_literal, rhs.m_literal) < 0;
}

template<typename _chType>
inline constexpr auto StringLiteral<_chType>::operator<(_chType const* const rhs) const -> bool
{
	if (rhs == m_literal)return false;
	return StringHelper<_chType>::StrCmp(m_literal, rhs) < 0;
}

template<typename _chType>
inline constexpr auto StringLiteral<_chType>::operator>(StringLiteral const& rhs) const -> bool
{
	if (rhs.m_literal == m_literal)return false;
	return StringHelper<_chType>::StrCmp(m_literal, rhs.m_literal) > 0;
}

template<typename _chType>
inline constexpr auto StringLiteral<_chType>::operator>(_chType const* const rhs) const -> bool
{
	if (rhs == m_literal)return false;
	return StringHelper<_chType>::StrCmp(m_literal, rhs) > 0;
}

template<typename _chType>
inline constexpr auto StringLiteral<_chType>::GetLength(_chType const* const literal) -> size_t
{
	size_t i{};
	for (i = 0; literal[i] != static_cast<_chType>(0); ++i)
	{

	}
	return i;
}
