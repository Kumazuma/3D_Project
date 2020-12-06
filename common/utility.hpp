#pragma once
#include <string>
#include <codecvt>
namespace Kumazuma
{
	constexpr float PI = 3.141592f;
	constexpr float RADIAN = PI;
	constexpr float TAU = RADIAN * 2;//새로 정의된 원주율
	constexpr float DEGREE = 360.f;
	
	struct Radian
	{
		Radian(float rad) :rad{ rad } {}
		float ToDegree() const& { return rad * DEGREE / TAU; }
		float ToDegree() const&& { return rad * DEGREE / TAU; }
		constexpr static float ToDegree(float degree) { return degree * DEGREE / TAU; }
		//constexpr float ToDegree() const&& { return rad * DEGREE / TAU; }
		//float ToDegree() const&& { return rad * DEGREE / TAU; }
		float rad;
	};
	struct Degree
	{
		//constexpr Degree(float degree) :degree{ degree } {}
		Degree(float degree) :degree{ degree } {}
		float ToRadian() const&& { return degree * TAU / DEGREE; }
		float ToRadian() const& { return degree * TAU / DEGREE; }
		//constexpr float ToRadian() const& { return degree * TAU/ DEGREE; }
		//constexpr float ToRadian() const&& { return degree * TAU / DEGREE; }
		constexpr static float ToRadian(float degree) {return degree * TAU / DEGREE;}
		float degree;
	};
	constexpr float Degree2Radian(float degree)
	{
		return degree * TAU / DEGREE;
	}
	constexpr float Radian2Degree(float rad)
	{
		return rad * DEGREE / TAU;
	}
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
	/*template<>
	struct WideChar<4>
	{
		static std::wstring Convert(char const* const str)
		{
			std::wstring_convert<std::codecvt_utf8<char32_t>> converter;
			return converter.from_bytes(str);
		}
		static std::wstring Convert(std::string const& str)
		{
			std::wstring_convert<std::codecvt_utf8<char32_t>> converter;
			return converter.from_bytes(str);
		}
	};*/
	inline std::wstring ConvertUTF8ToWide(UTF8String const& utf8String)
	{
		return WideChar<sizeof(wchar_t)>::Convert(utf8String);
	}
	inline std::wstring ConvertUTF8ToWide(char const* const utf8String)
	{
		return WideChar<sizeof(wchar_t)>::Convert(utf8String);
	}
}