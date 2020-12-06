// Game.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include "pch.h"
#include "framework.h"
#include "Game.h"
#include "app.h"
#include "TestScene.hpp"
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    CoInitialize(nullptr);
    if (Kumazuma::App::Initialize(hInstance) == false)
    {
        return -1;
    }
    auto app = Kumazuma::App::Instance();

    app->LoadScene<Kumazuma::Client::TestLoadingScene>();
    int res = app->Loop();

    app.reset();
    Kumazuma::App::Release();

    CoUninitialize();
    return res;
}