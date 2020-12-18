#pragma once
#include<string>
class InitializeFailedException
{
public:
	InitializeFailedException(wchar_t const* const fileName, int line, std::string const& what);
	inline auto FileName()const->wchar_t const*;
	inline auto Line()const->int;
	inline auto What()const->std::string const&;
private:
	wchar_t const* m_file;
	int m_line;
	std::string m_errorMsg;
};
inline InitializeFailedException::InitializeFailedException(
	wchar_t const* const fileName,
	int line, std::string const& what):
	m_file{fileName},
	m_line{ line },
	m_errorMsg{what}
{

}

inline auto InitializeFailedException::FileName() const -> wchar_t const*
{
	return m_file;
}

inline auto InitializeFailedException::Line() const -> int
{
	return m_line;
}

inline auto InitializeFailedException::What() const -> std::string const&
{
	return m_errorMsg;
}
