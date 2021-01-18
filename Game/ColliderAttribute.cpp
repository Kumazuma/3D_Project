#include "ColliderAttribute.hpp"
#include "BoxColliderAttribute.hpp"
#include "SphareColliderAttribute.hpp"
auto Kumazuma::Client::ColliderAttribute::FromType(ColliderType type) -> ColliderAttribute*
{
    switch (type)
    {
    case ColliderType::NONE:
        return nullptr;
    case ColliderType::BOX:
        return new BoxColliderAttribute{};
    case ColliderType::SPHERE:
        return new SphereColliderAttribute{};
    }
}

auto Kumazuma::Client::ColliderAttribute::LoadFromJson(nlohmann::json const& data) -> ColliderAttribute*
{
    nlohmann::json::const_iterator it;
    ColliderAttribute* attr{};
    if (it = data.find(u8"type"); it == data.end()) return nullptr;
    if (*it == u8"SPHARE") attr = new SphereColliderAttribute{};
    else if (*it == u8"BOX") attr = new BoxColliderAttribute{};
    if (attr != nullptr) attr->DoLoadFromJson(data);
    return attr;
}

