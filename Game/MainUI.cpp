#include "MainUI.hpp"
#include "COMMainUIRender.hpp"
auto Kumazuma::Client::SpawnUI(
    std::shared_ptr<Graphics::GameRenderer> const& renderer,
    std::shared_ptr<Game::Object> const& object)
    -> std::shared_ptr<Game::Object>
{
    auto uiObject{ std::make_shared<Game::Object>() };
    uiObject->AddComponent<COMMainUIRender>(renderer, object);
    return uiObject;
}
