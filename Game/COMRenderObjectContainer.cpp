#include "COMRenderObjectContainer.hpp"
#include "RenderObject.h"
namespace Kumazuma::Client
{
    //IMPLEMENT_COMPONENT_CLASS(Kumazuma::Client::COMRenderObjectContainer)
    //COMRenderObjectContainer::COMRenderObjectContainer() :
    //    Component(TAG)
    //{
    //}

    //COMRenderObjectContainer::COMRenderObjectContainer(Self const& rhs):
    //    Component(rhs)
    //{
    //    std::lock_guard<SpinLock> guard(m_mutex);

    //    for (auto it : rhs.m_collection)
    //    {
    //        m_collection[it.first].reset(it.second->Clone());
    //    }
    //}

    //auto COMRenderObjectContainer::Remove(std::wstring const& key) -> void
    //{
    //    std::lock_guard<SpinLock> guard(m_mutex);

    //    m_collection.erase(key);
    //}

    //auto COMRenderObjectContainer::Insert(std::wstring const& key, std::shared_ptr<MeshObject> obj) -> void
    //{
    //    std::lock_guard<SpinLock> guard(m_mutex);

    //    m_collection[key] = std::move(obj);
    //}

    //auto COMRenderObjectContainer::Get(std::wstring const& key) -> std::shared_ptr<MeshObject>
    //{
    //    std::lock_guard<SpinLock> guard(m_mutex);

    //    return m_collection[key];
    //}

    //auto COMRenderObjectContainer::Clone() const -> Game::Component*
    //{
    //    return new COMRenderObjectContainer{ *this };
    //}

    //auto COMRenderObjectContainer::begin() const -> Collection::const_iterator
    //{
    //    return m_collection.begin();
    //}

    //auto COMRenderObjectContainer::end() const -> Collection::const_iterator
    //{
    //    return m_collection.end();
    //}

}
