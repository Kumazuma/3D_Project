#include "COMRenderer.hpp"
IMPLEMENT_COMPONENT_CLASS(Kumazuma::Client::COMRenderer)
Kumazuma::Client::COMRenderer::COMRenderer(std::shared_ptr<Graphics::GameRenderer> const& renderer):
    Component{ TAG }
{
    renderer_ = renderer;
}

auto Kumazuma::Client::COMRenderer::Clone() const -> Game::Component*
{
    return nullptr;
}

auto Kumazuma::Client::COMRenderer::GetRenderer() -> std::shared_ptr<Graphics::GameRenderer>
{
    return renderer_;
}
