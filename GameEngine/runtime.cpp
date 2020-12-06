#include"pch.h"
#include"include/game/runtime.hpp"
#include"include/game/comtag.hpp"
#include"include/game/object.hpp"
#include"include/game/objectfactory.hpp"
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
        tmp->m_tagNComs.clear();
        tmp->m_objectNComs.clear();
        tmp->m_objectPool.clear();
    }
}

Kumazuma::Game::Runtime::~Runtime()
{
    m_tagNComs.clear();
    m_objectNComs.clear();
}
void Runtime::Update(float delta)
{
    //TODO:스레드로 병렬화 가능
    UpdateEvent updateEvent{ delta };

    for (auto pair : m_objectNComs)
    {
        for (auto& com : pair.second)
        {
            com->HandleEvent(&updateEvent);
        }
    }
    for (auto& mod: m_modules)
    {
        mod->PostUpdate(*this);
    }
    //TODO:스레드로 병렬화 가능
    decltype(m_queue) queue;
    while (m_queue.empty() == false)
    {
        queue = std::move(m_queue);
        while (queue.empty() == false)
        {   
            auto item = std::move(queue.front());
            queue.erase(queue.begin());
            for (const auto& weak_ref : item.handledCompnent)
            {
                auto ref = weak_ref.lock();
                ref->HandleEvent(item.event);
            }
            delete item.event;
        }
    }
    GC();
}

size_t Kumazuma::Game::Runtime::GetObjUid(const Object& obj)
{
    return obj.m_id;
}

std::shared_ptr<Object> Runtime::CreateObject(const ObjectFactory& factory)
{
    m_objectPool.push_back(Object{ factory.m_tags.begin(), factory.m_tags.end() });
    
    std::shared_ptr<Object> obj{ &*(--m_objectPool.end()) , &Runtime::DeleteObject};
    obj->m_runtime = Runtime::Instance();
    obj->m_id = ++m_lastGameObjectIndex;
    for (auto& com : factory.m_components)
    {
        Component* clonedCom = com.second->Create();
        auto com = std::shared_ptr<Component>{ clonedCom };
        m_tagNComs[&com->GetTag()].push_back(com);
        m_objectNComs[obj->m_id].push_back(com);
        com->SetObject(obj);
    }
    return obj;
}

Kumazuma::Game::Runtime::Runtime():
    m_lastGameObjectIndex{ 0 }
{
}

void Runtime::DoBroadcast(const ComTagBase& comTag, Event* event, size_t objId /*= 0*/)
{
    if (comTag == COM_ANY && objId == 0)
    {
        delete event;
        return;
    }
    Kumazuma::LinkedList<std::weak_ptr<Component> > componentList;
    if (objId != 0)
    {
        for (auto& com : m_objectNComs[objId])
        {
            if (com->IsHandled(event->GetTag()) && (comTag == COM_ANY || comTag == com->GetTag()) )
            {
                componentList.push_back(com);
            }
        }
    }
    else 
    {
        const auto res = m_tagNComs.find(&comTag);
        if (res != m_tagNComs.end())
        {
            componentList = res->second;
        }
    }
    if (componentList.empty())
    {
        delete event;
        return;
    }
    m_queue.push_back(EventQueueItem{ event,std::move( componentList )});
}
void Runtime::DeleteObject(Object* ptr)
{
    auto instance = Instance();
    if (instance != nullptr)
    {
        instance->m_readyToDestoryObjectQueue.push_back(ptr->m_id);
        for (auto& com : instance->m_objectNComs[ptr->m_id])
        {
            com->UnbindAll();
        }
    }
    else
    {
        
    }
}
void Kumazuma::Game::Runtime::GC()
{
    //죽을 준비가 된 오브젝트들을 정리한다.
    bool r = m_readyToDestoryObjectQueue.empty() == false;
    while (m_readyToDestoryObjectQueue.empty() == false)
    {
        auto uid = *m_readyToDestoryObjectQueue.begin();
        m_readyToDestoryObjectQueue.pop_front();
        m_objectNComs.erase(uid);
        auto end = m_objectPool.end();
        for (auto it = m_objectPool.begin(); it != end; ++it)
        {
            if (it->m_id == uid)
            {
                m_objectPool.erase(it);
                break;
            }
        }
    }
    if (r)
    {
        for (auto& it : m_tagNComs)
        {
            auto com = it.second.begin();
            while (com != it.second.end())
            {
                if (com->expired())
                {
                    com = it.second.erase(com);
                }
                else
                {
                    ++com;
                }
            }
        }
    }
}