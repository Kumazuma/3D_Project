#pragma once
#include<string>
#include<Windows.h>
using UTF8String = std::string;
using UTF8Char = char;
template<size_t WCHAR_BYTE_COUNT>
struct WideChar {};
template<>
struct WideChar<2>
{
	static std::wstring Convert(char const* const str)
	{

		int const needByte{ MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str,static_cast<int>(strlen(str)), nullptr, 0) };
		std::wstring buffer;
		buffer.assign(needByte, 0);
		int const actualSize{ MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str, static_cast<int>(strlen(str)), &buffer[0], static_cast<int>(buffer.size())) };

		return buffer;
	}
	static std::wstring Convert(std::string const& str)
	{
		int const needByte{ MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str.c_str(), static_cast<int>(str.size()), nullptr, 0) };
		std::wstring buffer;
		buffer.assign(needByte, 0);
		int const actualSize{ MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str.c_str(), static_cast<int>(str.size()), &buffer[0], static_cast<int>(buffer.size())) };

		return buffer;
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
inline std::string ConvertWideToUTF8(wchar_t const* const literalstr, size_t len = -1)
{
	if (len == -1)
	{
		len = wcslen(literalstr);
	}
	BOOL r{ false };
	int const needByte{ WideCharToMultiByte(CP_UTF8, 0, literalstr, static_cast<int>(len), nullptr, 0, 0, &r) };
	std::string buffer;
	buffer.assign(needByte, 0);
	int const actualSize{ WideCharToMultiByte(CP_UTF8, 0, literalstr, static_cast<int>(len), &buffer[0], static_cast<int>(buffer.size()),0, &r) };
	return buffer;
}
inline std::string ConvertWideToUTF8(const std::wstring& wstr)
{
	return ConvertWideToUTF8(wstr.c_str(), static_cast<int>(wstr.size()));
}
