#pragma once
#include"include/game/ThreadPoolMgr.hpp"
#include<Windows.h>
#include<mutex>
#include<tbb/concurrent_queue.h>
#include<functional>
class TaskImpl;
class ThreadPoolManagerImpl : public Kumazuma::ThreadPool::Manager
{
	ThreadPoolManagerImpl();
	~ThreadPoolManagerImpl();
public:
	static auto Instance()->std::shared_ptr<ThreadPoolManagerImpl>;
public:
	auto QueueTask(std::function<void(Kumazuma::ThreadPool::TaskContext&)> process)->std::shared_ptr<Kumazuma::ThreadPool::Task> override;
	auto DispatchTask()->void override;
private:
	static auto OnDelete(ThreadPoolManagerImpl* ptr)->void;
	static auto OnDeleteWork(TaskImpl* ptr)->void;
	static auto CALLBACK Worker(PTP_CALLBACK_INSTANCE instance, void* parameter, PTP_WORK work)->void;
private:
	static std::shared_ptr<ThreadPoolManagerImpl> s_pInstance;
	static std::mutex s_pCreateMutex;

	tbb::concurrent_queue<Kumazuma::ThreadPool::TaskContext> m_completeTaskQueue;
	TP_CALLBACK_ENVIRON m_callbackEnv;
	PTP_POOL m_pThreadPool;
	PTP_CLEANUP_GROUP m_pCleanupGroup;
};

class TaskItem
{
public:
	std::function<void(Kumazuma::ThreadPool::TaskContext&)> process;
};
class TaskImpl : public Kumazuma::ThreadPool::Task
{
	friend class ThreadPoolManagerImpl;
public:
	auto Wait()->void override;
private:
	PTP_WORK work;
};