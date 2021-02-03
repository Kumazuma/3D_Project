#pragma once
#include"COMPtr.hpp"
#ifdef DLL_EXPORT
#define DLL_CLASS _declspec(dllexport)
#else
#define DLL_CLASS _declspec(dllimport)
#endif