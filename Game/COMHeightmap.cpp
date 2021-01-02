#include "COMHeightmap.hpp"
#include "HeightMap.hpp"
namespace Kumazuma::Client
{
    Game::ComponentTag<COMHeightMap> const COMHeightMap::TAG{"COMHeightMap"};
    COMHeightMap::COMHeightMap(std::shared_ptr<HeightMap> heightMap) :
        Component{ TAG },
        m_heightmap{std::move(heightMap)}
    {

    }
    auto COMHeightMap::Clone() const -> Game::Component*
    {
        return new COMHeightMap{ *this };
    }
    auto COMHeightMap::GetHeightMap() const -> std::shared_ptr<HeightMap>
    {
        return m_heightmap;
    }
}

