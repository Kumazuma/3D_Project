#include "pch.h"
#include "gdi_render.hpp"

Kumazuma::gdi::Renderer::Renderer(HWND hWindow)
{
	RECT rc{};
	GetClientRect(hWindow, &rc);
	//윈도우 DC를 얻는다.
	HDC hDC = GetDC(hWindow);
	//백버퍼를 생성한다.
	CreateCompatibleBitmap(hDC,)
}
