#include "pch.h"
#include "./include/game/TimerManager.hpp"
#include "typedef.hpp"
#include <Windows.h>


std::shared_ptr<Kumazuma::Game::TimerManager> Kumazuma::Game::TimerManager::s_pInstance = nullptr;
std::shared_ptr<Kumazuma::Game::TimerManager> Kumazuma::Game::TimerManager::Instance()
{
    return s_pInstance;
}

void Kumazuma::Game::TimerManager::Initialize()
{
    if (s_pInstance == nullptr)
    {
        s_pInstance.reset(new TimerManager{});
    }
}

void Kumazuma::Game::TimerManager::Release()
{
    s_pInstance = nullptr;
}

Kumazuma::Game::TimerManager::TimerManager()
{
    m_cpuTick = 0;
    m_lastTick = 0;
    QueryPerformanceFrequency( reinterpret_cast<LARGE_INTEGER*>(&m_cpuTick) );
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_lastTick));

}

float Kumazuma::Game::TimerManager::GetElapseTimer(const wchar_t* szTimerTag)
{
    const std::shared_ptr<Timer> pTimer{ FindTimer(szTimerTag) };
    if (pTimer == nullptr)
    {
        return 0.f;
    }

    return pTimer->GetElapse();
}

void Kumazuma::Game::TimerManager::ResumeTimer(const wchar_t* szTimerTag)
{
    const std::shared_ptr<Timer> pTimer{ FindTimer(szTimerTag) };
    if (pTimer == nullptr)
    {
        return ;
    }
    pTimer->Resume();
}

void Kumazuma::Game::TimerManager::ResetTimer(const wchar_t* szTimerTag)
{
    const std::shared_ptr<Timer> pTimer{ FindTimer(szTimerTag) };
    if (pTimer == nullptr)
    {
        return;
    }
    pTimer->Reset();
}

void Kumazuma::Game::TimerManager::PauseTimer(const wchar_t* szTimerTag)
{
    const std::shared_ptr<Timer> pTimer{ FindTimer(szTimerTag) };
    if (pTimer == nullptr)
    {
        return;
    }
    pTimer->Pause();
}

bool Kumazuma::Game::TimerManager::ReadyTimer(const wchar_t* szTimerTag)
{
    std::shared_ptr<Timer> pTimer{ FindTimer(szTimerTag) };
    if (pTimer != nullptr)
    {
        return false;
    }
    pTimer.reset(new Timer{});
    m_timerTable.emplace(szTimerTag, std::move(pTimer));
    return true;
}

bool Kumazuma::Game::TimerManager::IsPauseTimer(const wchar_t* szTimerTag)
{
    std::shared_ptr<Timer> pTimer{ FindTimer(szTimerTag) };
    if (pTimer != nullptr)
    {
        return false;
    }
    return pTimer->IsPause();
}

std::shared_ptr<Kumazuma::Game::Timer> Kumazuma::Game::TimerManager::CreateTimer()
{
    std::shared_ptr<Timer> pNewTimer{ new Timer{} };
    m_unnammedTimers.push_back(pNewTimer);
    return pNewTimer;
}

void Kumazuma::Game::TimerManager::Update()
{
    u64 currentTime{};
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentTime));
    QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&m_cpuTick));
    f32 delta{ (currentTime - m_lastTick)/static_cast<f32>(m_cpuTick) };
    m_lastTick = currentTime;
    for (auto& pTimerItem : m_timerTable)
    {
        pTimerItem.second->Update(delta);
    }
    const auto end = m_unnammedTimers.end();
    auto it = m_unnammedTimers.begin();
    while (it != end)
    {
        if (it->expired())
        {
            it = m_unnammedTimers.erase(it);
            continue;
        }
        std::shared_ptr<Timer> pTimer{ it->lock() };
        if (pTimer == nullptr)
        {
            it = m_unnammedTimers.erase(it);
            continue;
        }
        pTimer->Update(delta);
        ++it;
    }
}

std::shared_ptr<Kumazuma::Game::Timer> Kumazuma::Game::TimerManager::FindTimer(const wchar_t* szTimerTag)
{
    const auto iter = m_timerTable.find(szTimerTag);
    if (iter == m_timerTable.end())
    {
        return nullptr;
    }
    return iter->second;
}

Kumazuma::Game::Timer::Timer()
{
    m_paused = false;
    m_elapse = 0;
}

void Kumazuma::Game::Timer::Reset()
{
    m_elapse = 0;
}

void Kumazuma::Game::Timer::Resume()
{
    m_paused = false;
}

void Kumazuma::Game::Timer::Update(f32 elapse)
{
    if (m_paused == false)
    {
        m_elapse += elapse;
    }
}

void Kumazuma::Game::Timer::Pause()
{
    m_paused = true;
}
