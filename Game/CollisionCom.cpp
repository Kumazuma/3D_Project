#include "pch.h"
#include "CollisionCom.h"
namespace Kumazuma
{
    namespace MS2
    {
        const Kumazuma::Game::ComponentTag<CollisionCom> CollisionCom::TAG{ "CollisionCom" };
    }
}

Kumazuma::MS2::CollisionCom::CollisionCom()
    :   Kumazuma::Game::Component{TAG},
        m_radius{0.f}
{

}

float Kumazuma::MS2::CollisionCom::GetRadius() const
{
    return m_radius;
}

void Kumazuma::MS2::CollisionCom::SetRadius(float radius)
{
    m_radius = radius;
}

Kumazuma::Game::Component* Kumazuma::MS2::CollisionCom::Clone() const
{
    return new CollisionCom{ *this };
}
