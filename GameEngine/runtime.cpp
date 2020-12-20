#include"pch.h"
#include"include/game/runtime.hpp"
#include"include/game/comtag.hpp"
#include"include/game/object.hpp"
#include"include/game/objectfactory.hpp"
#include"include/game/ThreadPoolMgr.hpp"
#include<omp.h>
namespace Kumazuma
{
    namespace Game
    {
        std::shared_ptr<Runtime> Runtime::s_managerInner = nullptr;
        const EventTag<UpdateEvent> EVT_UPDATE{ "EVT_UPDATE" };
        UpdateEvent::UpdateEvent(float delta):
            Event{EVT_UPDATE},
            m_delta{delta}
        {

        }
        class EventProcessor
        {
        public:
            auto Process(Component* com, Event& event)->void
            {
                com->HandleEvent(event);
            }
        };
    }
}
using namespace Kumazuma::Game;
std::shared_ptr<Runtime> Runtime::Instance()
{
    return s_managerInner;
}

void Kumazuma::Game::Runtime::Initialize()
{
    s_managerInner.reset(new Runtime{});
}

void Kumazuma::Game::Runtime::Release()
{
    if (s_managerInner != nullptr)
    {
        auto tmp = s_managerInner;
        s_managerInner = nullptr;
    }
}

Kumazuma::Game::Runtime::Runtime()
{
}
Kumazuma::Game::Runtime::~Runtime()
{
}

void Runtime::Update(float delta)
{

    //TODO:스레드로 병렬화 가능
    using namespace Kumazuma::ThreadPool;
    UpdateEvent updateEvent{ delta };
    auto threadPoolManger{ Manager::Instance() };
    std::vector<std::shared_ptr<Task> > tasks;
    
    for (auto pair : m_tagComponentTable)
    {
        ComTagBase const* comtag{ pair.first };
        auto& rMutex{ m_tableLock[comtag] };
        std::shared_lock<std::shared_mutex> guard{ rMutex };
        auto& rList{ pair.second };
        for (auto it: rList)
        {
            auto task = threadPoolManger->QueueTask([comtag, it, delta](TaskContext& context) {
                EventProcessor process{};
                UpdateEvent updateEvent{ delta };
                process.Process( it, updateEvent);
                });
            tasks.emplace_back(std::move(task));
        }
    }
    for (auto& task : tasks)
    {
        task->Wait();
    }
    tasks.clear();
    GC();
    //TODO:스레드로 병렬화 가능
    while (m_eventQueue.empty() == false)
    {
        EventQueueItem item;
        if (m_eventQueue.try_pop(item))
        {
            auto task = threadPoolManger->QueueTask([item](TaskContext& context) {
                EventProcessor process{};
                auto ref = item.ptr.lock();
                if (ref == nullptr)return;
                process.Process(ref.get(), *item.event);
                });
            tasks.emplace_back(std::move(task));
        }
    }
    for (auto& task : tasks)
    {
        task->Wait();
    }
}

auto Kumazuma::Game::Runtime::OnDeleteComponent(Component* com) -> void
{
    auto runtime{ Runtime::Instance() };
    if (runtime == nullptr)
    {
        delete com;
        return;
    }
    auto comTag{ &com->GetTag() };
    auto& rLock = runtime->m_tableLock[comTag];
    std::lock_guard<std::shared_mutex> guard{ rLock };
    runtime->m_tagComponentTable[comTag].erase(com);
    delete com;
}



void Runtime::DoBroadcast(const ComTagBase& comTag, Event* event, Object const* const pObj)
{
    auto& rEventTag{ event->GetTag() };
    if (comTag == COM_ANY && pObj == nullptr)
    {
        delete event;
        return;
    }
    EventQueueItem item;
    item.event = std::shared_ptr<Event>(event);
    if (pObj != nullptr)
    {
        //std::shared_lock<std::shared_mutex> guard{ m_mutex };
        for (auto& row : m_tagComponentTable)
        {
            auto comtag{ row.first };
            auto& eventTag{ event->GetTag() };
            auto& rTableLock{ m_tableLock[comtag] };
            std::shared_lock<std::shared_mutex> guard{ rTableLock };
            auto& rList{ row.second };
            for (auto* com : rList)
            {
                if (com->IsHandled(eventTag) && (comTag == COM_ANY || comTag == com->GetTag()))
                {
                    item.ptr = com->weak_from_this();
                    m_eventQueue.emplace(item);
                }
            }
        }
    }
    else
    {
        //std::shared_lock<std::shared_mutex> guard{ m_mutex };
        auto& rTableLock{ m_tableLock[&comTag] };
        std::shared_lock<std::shared_mutex> guard{ rTableLock };
        const auto res = m_tagComponentTable.find(&comTag);
        if (res == m_tagComponentTable.end())return;
        auto& rList{ res->second };
        for (auto* com : rList)
        {
            item.ptr = com->weak_from_this();
            m_eventQueue.emplace(item);
        }
    }
}

auto Kumazuma::Game::Runtime::AddComponent(ComTagBase const* comtag, Component* ptr) -> std::shared_ptr<Component>
{
    std::shared_ptr<Component> res{ ptr, &OnDeleteComponent };
    auto& rTableLock{ m_tableLock[comtag] };
    std::lock_guard<std::shared_mutex> guard{ rTableLock };
    m_tagComponentTable[comtag].emplace(ptr);
    return res;
}

auto Kumazuma::Game::Runtime::GC() -> void
{

}
