#include "COMPlayerInput.hpp"
#include <Game/object.hpp>
#include <Game/TransformComponent.hpp>
#include "COMRenderObjectContainer.hpp"

namespace Kumazuma::Client
{
	using namespace Game;
	using namespace DirectX;
	const Game::ComponentTag<COMPlayerInput> COMPlayerInput::TAG{"COM_PLAYER_INPUT"};
	COMPlayerInput::COMPlayerInput():
		Component{ TAG }
	{
		Bind(EVT_UPDATE, &COMPlayerInput::Update);
	}
	auto COMPlayerInput::Clone() const -> Game::Component*
	{
		return new COMPlayerInput{ *this };
	}
	auto COMPlayerInput::Update(Game::UpdateEvent const& event) -> void
	{
		if (auto obj = GetObj().lock(); obj != nullptr)
		{
			auto transform = obj->GetComponent<Game::TransformComponent>();
			auto renderObjContainer{ obj->GetComponent<COMRenderObjectContainer>() };
			auto renderObj{ renderObjContainer->Get(L"") };
			if (renderObj)
			{
				auto skinnedMesh{ std::static_pointer_cast<SkinnedXMeshObject>(renderObj) };
				XMFLOAT4X4 tranform;
				transform->GenerateTransformMatrix(&tranform);
				renderObj->SetTransform(tranform);
				skinnedMesh->PlayAnimation(event.GetDelta());
			}
		}
	}
}
