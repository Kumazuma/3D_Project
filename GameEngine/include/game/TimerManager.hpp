#pragma once
#include<cstdint>
#include<memory>
#include<unordered_map>
#include "typedef.hpp"
namespace Kumazuma
{
	namespace Game
	{
		class Timer;
		class TimerManager
		{
		private:
			static std::shared_ptr<TimerManager> s_pInstance;
		public:
			static std::shared_ptr<TimerManager> Instance();
			static void Initialize();
			static void Release();
		private:
			TimerManager();
		public:
			/// <summary>
			/// reset혹은 start한 시점으로 부터 몇 초가 지났는지 반환해주는 함수
			/// </summary>
			/// <param name="szTimerTag"></param>
			/// <returns></returns>
			float GetElapseTimer(const wchar_t* szTimerTag);
			/// <summary>
			/// 정지한 타이머를 재개하는 함수
			/// </summary>
			/// <param name="szTimerTag"></param>
			/// <returns></returns>
			void ResumeTimer(const wchar_t* szTimerTag);
			/// <summary>
			/// 타이머의 시간을 0으로 초기화하는 함수
			/// </summary>
			/// <param name="szTimerTag"></param>
			void ResetTimer(const wchar_t* szTimerTag);
			/// <summary>
			/// 타이머를 일시정지시킨다.
			/// </summary>
			/// <param name="szTimerTag"></param>
			void PauseTimer(const wchar_t* szTimerTag);
			/// <summary>
			/// 타이머를 세팅하는 함수
			/// </summary>
			/// <param name="szTimerTag"></param>
			/// <returns></returns>
			bool ReadyTimer(const wchar_t* szTimerTag);
			/// <summary>
			/// 해당 타이머가 정지 중인지 확인하는 함수
			/// </summary>
			/// <param name="szTimerTag"></param>
			/// <returns></returns>
			bool IsPauseTimer(const wchar_t* szTimerTag);
			/// <summary>
			/// 타이머를 생성한다.
			/// </summary>
			/// <returns></returns>
			std::shared_ptr<Timer> CreateTimer();
			void Update();
		private:
			std::shared_ptr<Timer> FindTimer(const wchar_t* szTimerTag);
		private:
			std::unordered_map<std::wstring, std::shared_ptr<Timer> > m_timerTable;
			std::list<std::weak_ptr<Timer> > m_unnammedTimers;
			uint64_t m_lastTick;
			uint64_t m_cpuTick;
			uint64_t m_fixTime;
		};
		
	}
}
class Kumazuma::Game::Timer
{
public:
	Timer();
	void Reset();
	void Resume();
	void Update(f32 elapse);
	void Pause();
	float GetElapse() const { return m_elapse; }
	bool IsPause()const { return m_paused; }
private:
	bool m_paused;
	f32 m_elapse;
};