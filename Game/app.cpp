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
#include <game/ThreadPoolMgr.hpp>
#include "PhysicsManager.hpp"
#include "Env.hpp"
#include <chrono>
using namespace Kumazuma;
std::shared_ptr<App> Kumazuma::App::s_instance = nullptr;
#pragma comment(lib, "gameengine.lib")
#pragma comment(lib, "kumawin.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "RenderModule.lib")
#pragma comment(lib, "GameRenderer.lib")

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
    if (FAILED(RenderModule::Create(m_mainWindow.GetHandle(), WINDOW_WIDTH, WINDOW_HEIGHT, false, &renderModule))) 
    {
        throw E_FAIL;
    }
    
    m_renderModule.reset(renderModule);
    Kumazuma::Client::ResourceManager::Initialize(m_renderModule);
    
    Kumazuma::Game::TimerManager::Initialize();
    auto pInputManager = InputManager::Instance();
    pInputManager->Bind(PLAYER_INPUT::FIRE, 'Z');
    pInputManager->Bind(PLAYER_INPUT::MOVE_BACKWARD, 'S');
    pInputManager->Bind(PLAYER_INPUT::MOVE_FORWARD, 'W');
    pInputManager->Bind(PLAYER_INPUT::MOVE_LEFT, 'A');
    pInputManager->Bind(PLAYER_INPUT::MOVE_RIGHT, 'D');
    pInputManager->Bind(PLAYER_INPUT::PASS_SCENE, VK_RETURN);
    pInputManager->Bind(PLAYER_INPUT::MOUSE_LBUTTON, VK_LBUTTON);
    pInputManager->Bind(PLAYER_INPUT::MOUSE_RBUTTON, VK_RBUTTON);
    pInputManager->Bind(PLAYER_INPUT::TOGLE_COLLIDER_BOX, VK_F2);
    pInputManager->Bind(PLAYER_INPUT::RUN, VK_SHIFT);
    pInputManager->Bind(PLAYER_INPUT::SKILL_01, '1');
    pInputManager->Bind(PLAYER_INPUT::SKILL_02, '2');
    pInputManager->Bind(PLAYER_INPUT::SKILL_03, '3');
    pInputManager->Bind(PLAYER_INPUT::SKILL_04, '4');
    auto baseDir{ Client::Enviroment::GetValue<std::wstring>(Client::Enviroment::BASE_DIR) };
    Client::PhysicsManager::Initialize();
    SoundManager::Initialize({
        {Client::SoundID::Ragnaros, baseDir + L"/resource/sound/ragnarosattacka.ogg" },
        {Client::SoundID::Attack,baseDir + L"/resource/sound/1h_sword_npc_hit_armor_plate_01.ogg" },
        {Client::SoundID::Roar,baseDir + L"/resource/sound/vo_pcorcmaleroar01.ogg" },
        {Client::SoundID::RagnarosAttack,baseDir + L"/resource/sound/fx_ragnaros_sulfuras_impact_01.ogg" },
        {Client::SoundID::RagnarosDeath ,baseDir + L"/resource/sound/ragnarosdeath.ogg" },
    });
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
    pDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020); /// ȯ�汤���� �� ����
    D3DMATERIAL9 material{};    // create the material struct

    material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set diffuse color to white
    material.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.1f);    // set ambient color to white
    pDevice->SetMaterial(&material);    // set the globably-used material to &material
    auto threadPoolMgr{ Kumazuma::ThreadPool::Manager::Instance() };
    std::chrono::system_clock::time_point  pretick{ std::chrono::system_clock::now() };
    while (msg.message != WM_QUIT)
    {

        if (m_isRunning == false)
        {
            break;
        }
        //QueryPerformanceCounter(&(LARGE_INTEGER&)now);
        std::chrono::system_clock::time_point now{ std::chrono::system_clock::now() };
        auto pNextScene = m_pNextScene.load(std::memory_order_acquire);
        if (pNextScene != nullptr)
        {
            m_pNowScene->Unloaded();
            delete m_pNowScene;
            m_pNowScene = pNextScene;
            m_pNextScene.store(nullptr, std::memory_order_release);
            m_pNowScene->Loaded();
            //m_preTick = now;
            pretick = now;
            continue;
        }
        
        if (now != pretick)
        {
            auto rc{ m_mainWindow.GetRect() };
            
            m_clientWidth = rc.right - rc.left;
            m_clientHeight = rc.bottom - rc.top;

            //InputManager::Instance().Update();
            std::chrono::nanoseconds  duration{ now - pretick };
            using second = std::chrono::duration<float, std::ratio<1, 1>>;
            float delta = std::chrono::duration_cast<second>(duration).count();

            pTimerMgr->Update();
            pInputMgr->Update();
            SoundManager::Instance().Update();
            m_pNowScene->Update(delta);
            //pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
            threadPoolMgr->DispatchTask();
            if (m_isRunning == false)
            {
                break;
            }
            pretick = now;
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
        //m_renderer.NotifyedResizeClientRect(*Runtime::Instance(), event.GetWidth(), event.GetAccel());
    }
}

fn_ ::App::LoadJSONData() -> void
{
}

auto App::_LoadScene(Scene* pScene)->bool
{
    Scene* pValue = nullptr;
    //nullptr�� �ƴϸ� �̹� �ε��Ϸ��� ���� �ִ� ���̴� ���߿� �� ���� �����Ѵ�.
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
            catch (HRESULT )
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
    Client::PhysicsManager::Release();
    s_instance = nullptr;
}
