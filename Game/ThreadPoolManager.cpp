#include "ThreadPoolManager.h"
#ifdef NDEBUG
#pragma comment(lib, "tbb.lib")
#pragma comment(lib, "tbbbind.lib")
#pragma comment(lib, "tbbmalloc.lib")
#pragma comment(lib, "tbbmalloc_proxy.lib")
#pragma comment(lib, "tbbproxy.lib")
#else
#pragma comment(lib, "tbb_debug.lib")
#pragma comment(lib, "tbbbind_debug.lib")
#pragma comment(lib, "tbbmalloc_debug.lib")
#pragma comment(lib, "tbbmalloc_proxy_debug.lib")
#pragma comment(lib, "tbbproxy_debug.lib")
#endif
std::shared_ptr<Kumazuma::ThreadPoolManager> Kumazuma::ThreadPoolManager::s_pInstance = nullptr;
std::mutex Kumazuma::ThreadPoolManager::s_pCreateMutex;
Kumazuma::ThreadPoolManager::ThreadPoolManager()
{
    InitializeThreadpoolEnvironment(&m_callbackEnv);
    m_pThreadPool = CreateThreadpool(nullptr);
    m_pCleanupGroup =  CreateThreadpoolCleanupGroup();
    SYSTEM_INFO sysInfo{};
    GetSystemInfo(&sysInfo);
    SetThreadpoolThreadMaximum(m_pThreadPool, sysInfo.dwNumberOfProcessors);
    SetThreadpoolThreadMinimum(m_pThreadPool, sysInfo.dwNumberOfProcessors);
    SetThreadpoolCallbackPool(&m_callbackEnv, m_pThreadPool);
    SetThreadpoolCallbackCleanupGroup(&m_callbackEnv, m_pCleanupGroup, nullptr);
}

Kumazuma::ThreadPoolManager::~ThreadPoolManager()
{
    CloseThreadpoolCleanupGroupMembers(m_pCleanupGroup, false, nullptr);
    CloseThreadpoolCleanupGroup(m_pCleanupGroup);
    CloseThreadpool(m_pThreadPool);
}

auto Kumazuma::ThreadPoolManager::Instance() -> std::shared_ptr<ThreadPoolManager>
{
    if (s_pInstance == nullptr)
    {
        std::lock_guard<std::mutex> lock(s_pCreateMutex);
        if (s_pInstance == nullptr)
        {
            s_pInstance = std::shared_ptr<ThreadPoolManager>(new ThreadPoolManager(), OnDelete);
        }
    }
    return s_pInstance;
}

auto Kumazuma::ThreadPoolManager::QueueTask(std::function<void(TaskContext&)> process) -> std::shared_ptr<Task>
{
    PTP_WORK pWork{ CreateThreadpoolWork(Worker, new TaskItem{process}, &m_callbackEnv) };
    SubmitThreadpoolWork(pWork);
    std::shared_ptr<Task> task{ std::shared_ptr < Task>{new Task{},& OnDeleteWork} };
    task->m_pWork = pWork;
    return task;
}

auto Kumazuma::ThreadPoolManager::OnDelete(ThreadPoolManager* ptr)->void
{
    delete ptr;
}

auto Kumazuma::ThreadPoolManager::OnDeleteWork(Task* ptr) -> void
{
    CloseThreadpoolWork(ptr->m_pWork);
    delete ptr;
}

auto Kumazuma::ThreadPoolManager::Worker(PTP_CALLBACK_INSTANCE instance, void* parameter, PTP_WORK work)->void
{
    auto manager{ Instance() };
    TaskItem* pTask{ reinterpret_cast<TaskItem*>(parameter) };
    TaskContext context;
    pTask->process(context);
    auto& t = context.GetEnd();
    if(t){
        manager->m_completeTaskQueue.push(context);
    }
}

auto Kumazuma::Task::Wait() -> void
{
    WaitForThreadpoolWorkCallbacks(m_pWork, false);
}
