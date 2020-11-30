#pragma once
#include<string>
#include <codecvt>
using UTF8String = std::string;
using UTF8Char = char;
template<size_t WCHAR_BYTE_COUNT>
struct WideChar {};
template<>
struct WideChar<2>
{
	static std::wstring Convert(char const* const str)
	{

		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		return converter.from_bytes(str);
	}
	static std::wstring Convert(std::string const& str)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		return converter.from_bytes(str);
	}
};
template<>
struct WideChar<4>
{
	static std::wstring Convert(char const* const str)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		return converter.from_bytes(str);
	}
	static std::wstring Convert(std::string const& str)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		return converter.from_bytes(str);
	}
};
inline std::wstring ConvertUTF8ToWide(UTF8String const& utf8String)
{
	return WideChar<sizeof(wchar_t)>::Convert(utf8String);
}
inline std::wstring ConvertUTF8ToWide(char const* const utf8String)
{
	return WideChar<sizeof(wchar_t)>::Convert(utf8String);
}

inline std::string ConvertWideToUTF8(const std::wstring& wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	return converter.to_bytes(wstr);
}
inline std::string ConvertWideToUTF8(wchar_t const* const literalstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	return converter.to_bytes(literalstr);
}