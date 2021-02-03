#include "pch.h"
#include "RenderTarget.hpp"

IRenderTarget::IRenderTarget():
	cnt_{ 1 }
{
}

IRenderTarget::IRenderTarget(IRenderTarget const& rhs):
	cnt_{ 1 }
{
}

IRenderTarget::IRenderTarget(IRenderTarget&& rhs) noexcept:
	cnt_{ std::move(rhs.cnt_) }
{
}

auto __stdcall IRenderTarget::AddRef() -> DWORD
{
	auto res{ InterlockedIncrement(&cnt_) };
	return res;
}

auto __stdcall IRenderTarget::Release() -> DWORD
{
	auto res{ InterlockedDecrement(&cnt_) };
	if (res == 0)
	{
		delete this;
	}
	return res;
}
