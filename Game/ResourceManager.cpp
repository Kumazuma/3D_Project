#include "pch.h"
#include "ResourceManager.hpp"
#include "RenderModule.h"
#include "CharacterMeta.hpp"
#include "ColliderAttribute.hpp"
#include <shared_mutex>
#include <fstream>
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
    m_mutex.lock_shared();
    auto it = m_objMeshs.find(path);
    auto const isExist{ it != m_objMeshs.end() };
    m_mutex.unlock_shared();

    if (isExist == false)
    {
        WowMapMeshObject* pNewmesh{};
        WowMapMeshObject::Create(m_pRenderModule.get(), path, &pNewmesh);
        if (pNewmesh == nullptr)
        {
            return nullptr;
        }
        m_mutex.lock();
        it = m_objMeshs.emplace(
            path,
            std::unique_ptr<WowMapMeshObject>{pNewmesh}
        ).first;
        m_mutex.unlock();
    }
    std::unique_ptr<WowMapMeshObject> res{ static_cast<WowMapMeshObject*>(it->second->Clone()) };
    return res;
}

auto Kumazuma::Client::ResourceManager::LoadSkinnedMesh(std::wstring const& id, std::wstring const& path) -> std::unique_ptr<SkinnedXMeshObject>
{
    
    m_mutex.lock_shared();
    if (auto idIt = m_idMeshPathTable.find(id); idIt != m_idMeshPathTable.end())
    {
        auto it = m_skinnedMeshs.find(idIt->second);
        m_mutex.unlock_shared();
        return std::unique_ptr<SkinnedXMeshObject>{static_cast<SkinnedXMeshObject*>(it->second->Clone())};
    }
    auto it = m_skinnedMeshs.find(path);
    auto const isExist{ it != m_skinnedMeshs.end() };
    m_mutex.unlock_shared();
    if (isExist == false)
    {
        SkinnedXMeshObject* pNewmesh{};
        SkinnedXMeshObject::Create(m_pRenderModule.get(), path, &pNewmesh);
        if (pNewmesh == nullptr)
        {
            return nullptr;
        }
        m_mutex.lock();
        it = m_skinnedMeshs.emplace(
            path,
            std::unique_ptr<SkinnedXMeshObject>{pNewmesh}
        ).first;
        m_mutex.unlock();
    }
    m_mutex.lock();
    m_idMeshPathTable.emplace(id, path);
    m_mutex.unlock();

    std::unique_ptr<SkinnedXMeshObject> res{ static_cast<SkinnedXMeshObject*>(it->second->Clone()) };
    return res;
}

auto Kumazuma::Client::ResourceManager::LoadCharacterMetaData(std::wstring const& id, std::wstring const& path) -> void
{
    std::ifstream istream;
    istream.open(path);
    nlohmann::json file{ nlohmann::json::parse(istream) };
    std::shared_ptr<CharacterMeta>  ptr{ CharacterMeta::LoadFromJson(file) };
    if (ptr != nullptr)
    {
        std::lock_guard<std::shared_mutex> guard{ m_mutex };
        m_characterMetas[id] = std::move(ptr);
    }
}

auto Kumazuma::Client::ResourceManager::GetCharacterMeta(std::wstring const& id)const -> std::shared_ptr<CharacterMeta const>
{
    std::shared_lock<std::shared_mutex> guard{ m_mutex };
    if (auto it = m_characterMetas.find(id); it != m_characterMetas.end())
    {
        return it->second;
    }
    return nullptr;
}

auto Kumazuma::Client::ResourceManager::GetSkinnedMesh(std::wstring const& id) const -> std::unique_ptr<SkinnedXMeshObject>
{
    std::shared_lock<std::shared_mutex> guard{ m_mutex };
    if (auto idIt = m_idMeshPathTable.find(id); idIt != m_idMeshPathTable.end())
    {
        auto it = m_skinnedMeshs.find(idIt->second);
        return std::unique_ptr<SkinnedXMeshObject>{static_cast<SkinnedXMeshObject*>(it->second->Clone())};
    }
    return nullptr;
}

auto Kumazuma::Client::ResourceManager::Clear() -> void
{
    std::lock_guard<decltype(m_mutex)> lockguard{ m_mutex };
    m_objMeshs.clear();
}