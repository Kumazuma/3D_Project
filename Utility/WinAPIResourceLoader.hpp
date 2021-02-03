#pragma once
#include<Windows.h>
class WinAPIResourceLoader
{
public:
	WinAPIResourceLoader(HMODULE hModule);
	WinAPIResourceLoader(WinAPIResourceLoader const&) = delete;

	~WinAPIResourceLoader();
	auto Open(wchar_t const* id, wchar_t const* resourceType);
	auto Close()->void;
	auto Ptr()const->void*;
	auto Size()const->size_t;
private:
	HMODULE hModule_;
	HGLOBAL hGlobal_;
	void* ptr_;
	size_t size_;
};
inline WinAPIResourceLoader::WinAPIResourceLoader(HMODULE hModule):
	hModule_{hModule},
	hGlobal_{ nullptr },
	ptr_{},
	size_{}
{
}
inline WinAPIResourceLoader::~WinAPIResourceLoader()
{
	Close();
}

inline auto WinAPIResourceLoader::Open(wchar_t const* id, wchar_t const* resourceType)
{
	Close();
	HRSRC hResMeta = FindResourceW(hModule_, id, resourceType);
	if (hResMeta == NULL)
	{
		throw E_FAIL;
	}
	hGlobal_ = LoadResource(hModule_, hResMeta);
	if (hGlobal_ == NULL)
	{
		throw E_FAIL;
	}
	ptr_ = LockResource(hGlobal_);
	size_ = static_cast<size_t>(SizeofResource(hModule_, hResMeta));
}

inline auto WinAPIResourceLoader::Close()->void
{
	if (hGlobal_ != nullptr)
	{
		FreeResource(hGlobal_);
		hGlobal_ = nullptr;
	}
}

inline auto WinAPIResourceLoader::Ptr() const -> void*
{
	return ptr_;
}

inline auto WinAPIResourceLoader::Size() const -> size_t
{
	return size_;
}
