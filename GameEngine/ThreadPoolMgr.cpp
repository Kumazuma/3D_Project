#include "pch.h"
#include "include/game/ThreadPoolMgr.hpp"
#include "ThreadPoolMgrImpl.hpp"

auto Kumazuma::ThreadPool::Manager::Instance() -> std::shared_ptr<Manager>
{
    return ThreadPoolManagerImpl::Instance();
}
