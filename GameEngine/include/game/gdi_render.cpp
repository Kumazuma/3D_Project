#include "pch.h"
#include "gdi_render.hpp"

Kumazuma::gdi::Renderer::Renderer(HWND hWindow)
{
	RECT rc{};
	GetClientRect(hWindow, &rc);
	//������ DC�� ��´�.
	HDC hDC = GetDC(hWindow);
	//����۸� �����Ѵ�.
	CreateCompatibleBitmap(hDC,)
}
