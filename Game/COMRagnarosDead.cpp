#include "COMRagnarosDead.hpp"
#include <game/object.hpp>
#include "COMPlayerRender.hpp"
#include "COMRenderer.hpp"
#include "LayerTags.hpp"
#include "Finale.hpp"
#include "app.h"
#include <sstream>
IMPLEMENT_COMPONENT_CLASS(Kumazuma::Client::COMRagnarosDeadController)
Kumazuma::Client::COMRagnarosDeadController::COMRagnarosDeadController():
    Component{ TAG }
{
    Bind(Game::EVT_UPDATE, &COMRagnarosDeadController::Update);
}

auto Kumazuma::Client::COMRagnarosDeadController::Clone() const -> Game::Component*
{
    return new COMRagnarosDeadController{ *this };
}

auto Kumazuma::Client::COMRagnarosDeadController::Update(Game::UpdateEvent const& evt) -> void
{
    auto object         { std::shared_ptr<Game::Object>{}           };
    auto comRender      { std::shared_ptr<COMSkinnedMeshRender>{}   };
    auto skinnedMesh    { std::shared_ptr<SkinnedXMeshObject>{}     };
    object = GetObj().lock();
    if (object == nullptr)
        return;
    comRender = object->GetComponent<COMSkinnedMeshRender>();
    if (comRender == nullptr)
        return;
    skinnedMesh = comRender->GetMesh();
    skinnedMesh->PlayAnimation(evt.GetDelta());
    auto const length{ skinnedMesh->GetCurrentAnimSetLength() };
    auto const seek{ skinnedMesh->GetCurrentSeek() };
    std::stringstream stream;
    stream << seek << "/" << length << " (" << (100.f * seek / length) << ")\n";
   
    OutputDebugStringA(stream.str().c_str());
    if (seek >= length * 0.85f)
    {
        auto& scene{ App::Instance()->GetScene() };
        scene.RemoveObject(LAYER_ETC, *object);
        auto rendererCom{ object->GetComponent<COMRenderer>() };
        scene.AddObject(LAYER_ETC, SpawnFinale(rendererCom->GetRenderer()));
    }
}
