#pragma once
#ifdef DLL_EXPORT
#define DLL_CLASS _declspec(dllexport)
#else
#define DLL_CLASS _declspec(dllimport)
#endif
#include "typedef.hpp"