#include "COMRenderObjectContainer.hpp"
#include <RenderModule/RenderObject.h>
namespace Kumazuma::Client
{
    Game::ComponentTag<COMRenderObjectContainer> COMRenderObjectContainer::TAG{ "COMRenderObjectContainer" };
    COMRenderObjectContainer::COMRenderObjectContainer() :
        Component(TAG)
    {
    }

    COMRenderObjectContainer::COMRenderObjectContainer(Self const& rhs):
        Component(rhs)
    {
        std::shared_lock<std::shared_mutex> guard(rhs.m_mutex);
        for (auto it : rhs.m_collection)
        {
            m_collection[it.first].reset(it.second->Clone());
        }
    }

    auto COMRenderObjectContainer::Remove(std::wstring const& key) -> void
    {
        std::lock_guard<std::shared_mutex> guard(m_mutex);
        m_collection.erase(key);
    }

    auto COMRenderObjectContainer::Insert(std::wstring const& key, std::shared_ptr<RenderObject> obj) -> void
    {
        std::lock_guard<std::shared_mutex> guard(m_mutex);
        m_collection[key] = std::move(obj);
    }

    auto COMRenderObjectContainer::Get(std::wstring const& key) -> std::shared_ptr<RenderObject>
    {
        std::shared_lock<std::shared_mutex> guard(m_mutex);
        return m_collection[key];
    }

    auto COMRenderObjectContainer::Clone() const -> Game::Component*
    {
        return new COMRenderObjectContainer{ *this };
    }

}
