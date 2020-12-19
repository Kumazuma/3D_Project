#pragma once
#include<thread>
#include<memory>
#include<mutex>
#include<tbb/concurrent_queue.h>
#include<functional>
#include<vector>
#include<Windows.h>
namespace Kumazuma
{
	class TaskContext;
	class TaskItem;
	class Task;
	class ThreadPoolManager
	{
		ThreadPoolManager();
		~ThreadPoolManager();
	public:
		static auto Instance()->std::shared_ptr<ThreadPoolManager>;
	public:
		auto QueueTask(std::function<void(TaskContext&)> process)->std::shared_ptr<Task>;
	private:
	private:
		static auto OnDelete(ThreadPoolManager* ptr)->void;
		static auto OnDeleteWork(Task* ptr)->void;
		static auto CALLBACK Worker(PTP_CALLBACK_INSTANCE instance, void* parameter, PTP_WORK work)->void;

	private:
		static std::shared_ptr<ThreadPoolManager> s_pInstance;
		static std::mutex s_pCreateMutex;
	private:
		tbb::concurrent_queue<TaskItem> m_waitTaskQueue;
		tbb::concurrent_queue<TaskContext> m_completeTaskQueue;
		TP_CALLBACK_ENVIRON m_callbackEnv;
		PTP_POOL m_pThreadPool;
		PTP_CLEANUP_GROUP m_pCleanupGroup;
	};

	class TaskItem
	{
	public:
		std::function<void(TaskContext&)> process;
	};
	class TaskContext
	{
	public:
		template<typename Fn, typename ...Args>
		auto SetEnd(Fn fn, Args&& ...args)->void;
		auto GetEnd()->std::function<void()> const&;
	private:
		std::function<void()> process;
	};

	template<typename Fn, typename ...Args>
	inline auto TaskContext::SetEnd(Fn fn, Args && ...args) -> void
	{
		process = std::function<void()>([args...]()->void{fn(std::forward<Args>(args)...); });
	}
	inline auto Kumazuma::TaskContext::GetEnd() -> std::function<void()> const&
	{
		return process;
	}
	class Task
	{
		friend class ThreadPoolManager;
		Task() = default;
		Task(Task const&) = delete;
		~Task() = default;
	public:
		auto Wait()->void;
	private:
		PTP_WORK m_pWork;
	};

}
