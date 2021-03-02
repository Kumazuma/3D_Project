#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#define NOMINMAX
#include <windows.h>
#include <wrl.h>
#undef min
#undef max
extern HMODULE g_hDLL;