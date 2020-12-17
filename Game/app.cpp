#include "pch.h"
#include "Resource.h"
#include "app.h"
#include <d3dx9.h>
#include<fstream>
#include"typedef.hpp"
#include<game/TimerManager.hpp>
#include"RenderModule.h"
#include"ResourceManager.hpp"
#include "Env.hpp"
#include<d3d9.h>
using namespace Kumazuma;
std::shared_ptr<App> Kumazuma::App::s_instance = nullptr;
#pragma comment(lib, "gameengine.lib")
#pragma comment(lib, "kumawin.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "RenderModule.lib")

//#pragma comment(lib, "FMODSoundManager.lib")

auto App::GetCursorPosition() const -> POINT
{
    return m_mainWindow.GetClientCursorPos();
}
auto App::Exit() -> void
{
    m_isRunning = false;
}
auto App::GetClientWidth() const -> u32
{
    return m_clientWidth;
}
auto App::GetClientHeight() const -> u32
{
    return m_clientHeight;
}
App::App(HINSTANCE hInstance)
{
    m_isRunning = true;
    m_pNowScene = nullptr;
    m_pNextScene = nullptr;
    Kumazuma::WindowSystem::Window::Initialize(hInstance);
    Game::Runtime::Initialize();
    Client::Enviroment::Initialize();
    RECT rc{ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    constexpr auto WS_STYLE = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;
    AdjustWindowRect(&rc, WS_STYLE, false);
    m_mainWindow = Window{ Kumazuma::WindowSystem::Window::Builder{}
        .title(L"hello!")
        .size(rc.right - rc.left, rc.bottom - rc.top)
        .style(WS_STYLE)
    };
    assert(m_mainWindow.IsValid() == true);
    if (m_mainWindow.IsValid() == false)
    {
        throw E_FAIL;  
    }
    m_mainWindow.Show();
    m_mainWindow.Connect(Kumazuma::WindowSystem::EVT_Destroy, this, &App::OnMainWindowDestory);
    RenderModule* renderModule;
    if (FAILED(RenderModule::Create(m_mainWindow.GetHandle(), WINDOW_WIDTH, WINDOW_HEIGHT, &renderModule))) 
    {
        throw E_FAIL;
    }
    
    m_renderModule.reset(renderModule);
    Kumazuma::Client::ResourceManager::Initialize(m_renderModule);
    
    Kumazuma::Game::TimerManager::Initialize();
    auto pInputManager = InputManager::Instance();
    pInputManager->Bind(PLAYER_INPUT::FIRE, 'Z');
    pInputManager->Bind(PLAYER_INPUT::MOVE_BACKWARD, VK_DOWN);
    pInputManager->Bind(PLAYER_INPUT::MOVE_FORWARD, VK_UP);
    pInputManager->Bind(PLAYER_INPUT::MOVE_LEFT, VK_LEFT);
    pInputManager->Bind(PLAYER_INPUT::MOVE_RIGHT, VK_RIGHT);
    pInputManager->Bind(PLAYER_INPUT::PASS_SCENE, VK_RETURN);
    
}   
    
App::~App()
{
    m_mainWindow.Disconnect();
    m_mainWindow.Close();
  
    //SoundManager::Release();
}
#include<time.h>
auto App::Loop()->int
{
    srand((unsigned)time(nullptr));

    MSG msg{};
    int64_t now;
    int64_t frequency = 0;
    m_preTick = GetTickCount64();
    frequency = 1000;
    /*QueryPerformanceFrequency(&(LARGE_INTEGER&)frequency);
    QueryPerformanceCounter(&(LARGE_INTEGER&)m_preTick);*/
    auto pNextScene = m_pNextScene.load(std::memory_order_acquire);
    if (pNextScene != nullptr)
    {
        if (m_pNowScene != nullptr)
        {
            m_pNowScene->Unloaded();
            delete m_pNowScene;
        }
        m_pNowScene = pNextScene;
        m_pNextScene.store(nullptr, std::memory_order_release);
        m_pNowScene->Loaded();
    }
    auto runtime{ Game::Runtime::Instance() };
    auto pTimerMgr{ Kumazuma::Game::TimerManager::Instance() };
    auto pInputMgr{ InputManager::Instance() };
    COMPtr<IDirect3DSwapChain9> swapChain;
    COMPtr<IDirect3DDevice9> pDevice;
    D3DLIGHT9		tLightInfo{};

    tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

    tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    tLightInfo.Direction = D3DVECTOR{ 1.f, -1.f, 1.f };
    DirectX::XMStoreFloat3(reinterpret_cast<DirectX::XMFLOAT3*>(&tLightInfo.Direction),
        DirectX::XMVector3Normalize(
            DirectX::XMLoadFloat3(
                reinterpret_cast<DirectX::XMFLOAT3*>(
                    &tLightInfo.Direction
                    )
            )
        )
    );
    m_renderModule->GetDefaultSwapChain(&swapChain);
    m_renderModule->GetDevice(&pDevice);
    pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
    pDevice->SetLight(0, &tLightInfo);
    pDevice->LightEnable(0, TRUE);
    pDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020); /// 환경광원의 값 설정
    D3DMATERIAL9 material{};    // create the material struct

    material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set diffuse color to white
    material.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.1f);    // set ambient color to white
    pDevice->SetMaterial(&material);    // set the globably-used material to &material

    while (msg.message != WM_QUIT)
    {

        if (m_isRunning == false)
        {
            break;
        }
        //QueryPerformanceCounter(&(LARGE_INTEGER&)now);
        now = GetTickCount64();
        auto pNextScene = m_pNextScene.load(std::memory_order_acquire);
        if (pNextScene != nullptr)
        {
            m_pNowScene->Unloaded();
            delete m_pNowScene;
            runtime->GC();
            m_pNowScene = pNextScene;
            m_pNextScene.store(nullptr, std::memory_order_release);
            m_pNowScene->Loaded();
            m_preTick = now;
            continue;
        }
        
        if (now != m_preTick)
        {
            auto rc{ m_mainWindow.GetRect() };
            
            m_clientWidth = rc.right - rc.left;
            m_clientHeight = rc.bottom - rc.top;

            //InputManager::Instance().Update();
            float delta = (now - m_preTick) / (float)frequency;
            pTimerMgr->Update();
            pInputMgr->Update();
            m_pNowScene->Update(delta);
            //pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

            runtime->GC();
            if (m_isRunning == false)
            {
                break;
            }
            m_preTick = now;
        }

        if (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (!TranslateAcceleratorW(msg.hwnd, m_hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
            m_preTick = GetTickCount64();
        }
        
    }
    pNextScene = m_pNextScene.load(std::memory_order_acquire);
    if (pNextScene != nullptr)
    {
        delete pNextScene;
        m_pNextScene.store(nullptr, std::memory_order_release);
    }
    if (m_pNowScene != nullptr)
    {
        m_pNowScene->Unloaded();
        delete m_pNowScene;
        m_pNowScene = nullptr;
    }
    runtime->GC();
    m_mainWindow.Close();
    return (int)0;
}

auto App::OnMainWindowDestory(Kumazuma::WindowSystem::Event&)->void
{
    PostQuitMessage(0);
}

auto App::OnMainWindowPaint(Kumazuma::WindowSystem::Event&)->void
{
    ValidateRect(m_mainWindow.GetHandle(), nullptr);
}

auto App::OnKeyDown(Kumazuma::WindowSystem::WinMsgEvent& event)->void
{
 
}

auto App::OnTimer(Kumazuma::WindowSystem::Event&)->void
{

}

auto App::OnSized(Kumazuma::WindowSystem::WinSizeEvent& event)->void
{
    if (event.IsMaximized() || event.IsRestored())
    {
        //m_renderer.NotifyedResizeClientRect(*Runtime::Instance(), event.GetWidth(), event.GetHeight());
    }
}

fn_ ::App::LoadJSONData() -> void
{
}

auto App::_LoadScene(Scene* pScene)->bool
{
    Scene* pValue = nullptr;
    //nullptr이 아니면 이미 로드하려는 씬이 있는 것이니 나중에 온 것은 해제한다.
    if (m_pNextScene.compare_exchange_weak(pValue, pScene, std::memory_order_seq_cst) == false)
    {
        delete pScene;
        return false;
    }
    return true;
}
auto App::SetDPICheck() -> void
{
    const HMODULE user32 = LoadLibraryW(L"User32");
    if (user32 == nullptr)
    {
        return;
    }
    auto SetProcessDpiAwarenessContext_ = reinterpret_cast<decltype(SetProcessDpiAwarenessContext)*>(GetProcAddress(user32, "SetProcessDpiAwarenessContext"));
    if (SetProcessDpiAwarenessContext_ != NULL)
    {
        SetProcessDpiAwarenessContext_(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);
    }
    FreeLibrary(user32);
}
#include<time.h>
auto App::Initialize(HINSTANCE hInstance)->bool
{
    srand((unsigned)time(nullptr));
    if (s_instance == nullptr)
    {
        if (s_instance == nullptr)
        {
            try
            {
                auto pApp{ new  App{hInstance} };
                s_instance.reset(pApp);
            }
            catch (HRESULT hr)
            {
                return false;
            }
        }
    }
    return true;
}

fn_ App::Release()->void
{ 
    Kumazuma::Game::Runtime::Release();
    Kumazuma::Game::TimerManager::Release();
    Client::ResourceManager::Release();
    Client::Enviroment::Release();
    s_instance = nullptr;
}
