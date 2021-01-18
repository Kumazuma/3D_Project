#include "CharacterMeta.hpp"
#include "ColliderAttribute.hpp"
#include <InitializeFailedException.h>
#include "UnicodeHelper.h"
Kumazuma::Client::CharacterMeta::CharacterMeta(nlohmann::json const& data) 
{
    if (auto const it = data.find(u8"mesh"); it != data.end())
    {
        if (it->is_string() == false)
        {
            throw InitializeFailedException(__FILEW__, __LINE__, L"'path' attribute's type is not string");
        }
        m_meshPath = ConvertUTF8ToWide(*it);
    }
    else throw InitializeFailedException(__FILEW__, __LINE__, L"has attribute 'mesh'");

    if (auto const it = data.find(u8"anim"); it != data.end())
    {
        if (it->is_object() == false)
        {
            throw InitializeFailedException(__FILEW__, __LINE__, L"'anim' attribute's type is not object");
        }
        auto const& jsonAnimTable{ *it };
        for (auto const& it : jsonAnimTable.items())
        {
            auto key{ ConvertUTF8ToWide(it.key()) };
            i32 id{ it.value() };
            m_animIdTable.emplace(std::move(key), id);
        }
    }
    else throw InitializeFailedException(__FILEW__, __LINE__, L"has attribute 'anim'");

    if (auto const it = data.find(u8"collider"); it != data.end())
    {
        auto const& jsonColliderList{ *it };
        for (auto const& it : jsonColliderList)
        {
            m_colliders.emplace_back(it);
        }
    }
    else throw InitializeFailedException(__FILEW__, __LINE__, L"has attribute 'collider'");
}

Kumazuma::Client::CharacterMeta::CharacterMeta(CharacterMeta&& rhs) noexcept:
    m_meshPath{ std::move( rhs.m_meshPath )},
    m_colliders{std::move(rhs.m_colliders)},
    m_animIdTable{std::move(rhs.m_animIdTable)}
{

}

Kumazuma::Client::CharacterMeta::CharacterMeta(CharacterMeta const& rhs):
    m_meshPath{ rhs.m_meshPath },
    m_colliders{rhs.m_colliders},
    m_animIdTable{rhs.m_animIdTable}
{

}

auto Kumazuma::Client::CharacterMeta::LoadFromJson(nlohmann::json const& data) -> CharacterMeta*
{
    try
    {
        CharacterMeta newObj{ data };
        return new CharacterMeta{ std::move(newObj) };
    }
    catch (InitializeFailedException ex)
    {
        DebugBreak();
    }
    return nullptr;
}

auto Kumazuma::Client::CharacterMeta::GetMeshPathRef() const -> std::wstring const&
{
    return m_meshPath;
}

auto Kumazuma::Client::CharacterMeta::GetAnimIndex(std::wstring const& id)const -> std::optional<i32>
{
    if (auto it{ this->m_animIdTable.find(id) }; it != this->m_animIdTable.end())
    {
        return it->second;
    }
    return std::nullopt;
}

auto Kumazuma::Client::CharacterMeta::GetCollidersRef() const -> std::vector<Collider> const&
{
    return m_colliders;
}
