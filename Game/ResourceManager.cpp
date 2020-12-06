#include "pch.h"
#include "ResourceManager.hpp"
#include "RenderModule.h"
#include <shared_mutex>
using namespace Kumazuma::Client;
std::shared_ptr<ResourceManager> ResourceManager::s_instance = nullptr;
Kumazuma::Client::ResourceManager::ResourceManager(std::shared_ptr<RenderModule> pRenderModule):
    m_pRenderModule{ pRenderModule }
{
}

Kumazuma::Client::ResourceManager::ResourceManager(ResourceManager&& rhs) noexcept:
    m_pRenderModule{std::move(rhs.m_pRenderModule)},
    m_objMeshs{std::move(rhs.m_objMeshs)}
{

}

Kumazuma::Client::ResourceManager::~ResourceManager()
{

}


auto Kumazuma::Client::ResourceManager::Initialize(std::shared_ptr<RenderModule> pRenderModule) -> bool
{
    if (s_instance == nullptr)
    {
        s_instance = std::shared_ptr<ResourceManager>(new ResourceManager{ pRenderModule });
        return true;
    }
    else
    {
        return false;
    }
}

auto Kumazuma::Client::ResourceManager::Instance() -> std::shared_ptr<ResourceManager>
{
    return s_instance;
}

auto Kumazuma::Client::ResourceManager::Release() -> void
{
    auto s = s_instance;
    s_instance = nullptr;
}

auto Kumazuma::Client::ResourceManager::LoadOBJMesh(std::wstring const& path) -> std::unique_ptr<WowMapMeshObject>
{
    std::lock_guard<decltype(m_mutex)> lockguard{ m_mutex };
    auto it = m_objMeshs.find(path);
    if (it == m_objMeshs.end())
    {
        WowMapMeshObject* pNewmesh{};
        WowMapMeshObject::Create(m_pRenderModule.get(), path, &pNewmesh);
        if (pNewmesh == nullptr)
        {
            return nullptr;
        }
        it = m_objMeshs.emplace(
            path,
            std::unique_ptr<WowMapMeshObject>{pNewmesh}
        ).first;
    }
    std::unique_ptr<WowMapMeshObject> res{ static_cast<WowMapMeshObject*>(it->second->Clone()) };
    return res;
}

auto Kumazuma::Client::ResourceManager::Clear() -> void
{
    std::lock_guard<decltype(m_mutex)> lockguard{ m_mutex };
    m_objMeshs.clear();
}