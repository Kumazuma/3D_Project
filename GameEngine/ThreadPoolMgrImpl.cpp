#include "pch.h"
#include "ThreadPoolMgrImpl.hpp"
#ifdef NDEBUG

#pragma comment(lib, "tbb.lib")
#pragma comment(lib, "tbbbind.lib")
#pragma comment(lib, "tbbmalloc.lib")
#pragma comment(lib, "tbbmalloc_proxy.lib")

#else

#pragma comment(lib, "tbb_debug.lib")
#pragma comment(lib, "tbbbind_debug.lib")
#pragma comment(lib, "tbbmalloc_debug.lib")
#pragma comment(lib, "tbbmalloc_proxy_debug.lib")

#endif
using namespace Kumazuma::ThreadPool;
std::shared_ptr<ThreadPoolManagerImpl> ThreadPoolManagerImpl::s_pInstance = nullptr;
std::mutex ThreadPoolManagerImpl::s_pCreateMutex;
ThreadPoolManagerImpl::ThreadPoolManagerImpl()
{
    InitializeThreadpoolEnvironment(&m_callbackEnv);
    m_pThreadPool = CreateThreadpool(nullptr);
    m_pCleanupGroup = CreateThreadpoolCleanupGroup();
    SYSTEM_INFO sysInfo{};
    GetSystemInfo(&sysInfo);
    SetThreadpoolCallbackPool(&m_callbackEnv, m_pThreadPool);
    SetThreadpoolThreadMaximum(m_pThreadPool, sysInfo.dwNumberOfProcessors);
    SetThreadpoolThreadMinimum(m_pThreadPool, sysInfo.dwNumberOfProcessors);
    SetThreadpoolCallbackCleanupGroup(&m_callbackEnv, m_pCleanupGroup, nullptr);

}

ThreadPoolManagerImpl::~ThreadPoolManagerImpl()
{
    CloseThreadpoolCleanupGroupMembers(m_pCleanupGroup, false, nullptr);
    CloseThreadpoolCleanupGroup(m_pCleanupGroup);
    CloseThreadpool(m_pThreadPool);
}

auto ThreadPoolManagerImpl::Instance() -> std::shared_ptr<ThreadPoolManagerImpl>
{
    if (s_pInstance == nullptr)
    {
        std::lock_guard<std::mutex> lock(s_pCreateMutex);
        if (s_pInstance == nullptr)
        {
            s_pInstance = std::shared_ptr<ThreadPoolManagerImpl>(new ThreadPoolManagerImpl(), OnDelete);
        }
    }
    return s_pInstance;
}

auto ThreadPoolManagerImpl::QueueTask(std::function<void(Kumazuma::ThreadPool::TaskContext&)> process) -> std::shared_ptr<Kumazuma::ThreadPool::Task> 
{
    PTP_WORK pWork{ CreateThreadpoolWork(Worker, new TaskItem{process}, &m_callbackEnv) };
    SubmitThreadpoolWork(pWork);
    TaskImpl* newTask{ new TaskImpl{} };
    std::shared_ptr<Task> task{ std::shared_ptr < Task>{newTask,& OnDeleteWork} };
    newTask->work = pWork;
    return task;
}

auto ThreadPoolManagerImpl::DispatchTask() -> void 
{
    TaskContext context;
    while (m_completeTaskQueue.try_pop(context))
    {
        if (!context.GetNext())
        {
            context.GetNext()();
        }
    }
}

auto ThreadPoolManagerImpl::OnDelete(ThreadPoolManagerImpl* ptr) -> void
{
    delete ptr;
}

auto ThreadPoolManagerImpl::OnDeleteWork(TaskImpl* ptr) -> void
{
    CloseThreadpoolWork(ptr->work);
    delete ptr;
}

auto ThreadPoolManagerImpl::Worker(PTP_CALLBACK_INSTANCE instance, void* parameter, PTP_WORK work) -> void
{
    auto manager{ Instance() };
    TaskItem* pTask{ reinterpret_cast<TaskItem*>(parameter) };
    TaskContext context;
    pTask->process(context);
    auto& t = context.GetNext ();
    if (t)
    {
        manager->m_completeTaskQueue.push(context);
    }
}

auto TaskImpl::Wait() -> void 
{
    WaitForThreadpoolWorkCallbacks(work, false);
}
