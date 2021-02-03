#include "Finale.hpp"
#include <GameRenderer.hpp>
#include "COMFinale.hpp"
auto Kumazuma::Client::SpawnFinale(std::shared_ptr<Graphics::GameRenderer> const& renderer) -> std::shared_ptr<Game::Object>
{
    auto object{ std::make_shared<Game::Object>() };
    object->AddComponent<COMFinaleRender>(renderer);
    return object;
}
