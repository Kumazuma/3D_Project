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
			/// resetȤ�� start�� �������� ���� �� �ʰ� �������� ��ȯ���ִ� �Լ�
			/// </summary>
			/// <param name="szTimerTag"></param>
			/// <returns></returns>
			float GetElapseTimer(const wchar_t* szTimerTag);
			/// <summary>
			/// ������ Ÿ�̸Ӹ� �簳�ϴ� �Լ�
			/// </summary>
			/// <param name="szTimerTag"></param>
			/// <returns></returns>
			void ResumeTimer(const wchar_t* szTimerTag);
			/// <summary>
			/// Ÿ�̸��� �ð��� 0���� �ʱ�ȭ�ϴ� �Լ�
			/// </summary>
			/// <param name="szTimerTag"></param>
			void ResetTimer(const wchar_t* szTimerTag);
			/// <summary>
			/// Ÿ�̸Ӹ� �Ͻ�������Ų��.
			/// </summary>
			/// <param name="szTimerTag"></param>
			void PauseTimer(const wchar_t* szTimerTag);
			/// <summary>
			/// Ÿ�̸Ӹ� �����ϴ� �Լ�
			/// </summary>
			/// <param name="szTimerTag"></param>
			/// <returns></returns>
			bool ReadyTimer(const wchar_t* szTimerTag);
			/// <summary>
			/// �ش� Ÿ�̸Ӱ� ���� ������ Ȯ���ϴ� �Լ�
			/// </summary>
			/// <param name="szTimerTag"></param>
			/// <returns></returns>
			bool IsPauseTimer(const wchar_t* szTimerTag);
			/// <summary>
			/// Ÿ�̸Ӹ� �����Ѵ�.
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