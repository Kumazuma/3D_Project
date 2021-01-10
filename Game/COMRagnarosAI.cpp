#include "COMRagnarosAI.hpp"
#include <DirectXMath.h>
#include <game/runtime.hpp>
#include <game/object.hpp>
#include <game/TransformComponent.hpp>
#include "COMRenderObjectContainer.hpp"
#include <SkinnedXMeshObject.h>
constexpr StringLiteral<wchar_t> CHARACTER_MESH{ L"CHARACTER" };
namespace Kumazuma::Client
{
	using namespace DirectX;
	const Game::ComponentTag<COMRagnarosAI> COMRagnarosAI::TAG{ "COMRagnarosAI" };
	COMRagnarosAI::COMRagnarosAI():
		Component(TAG)
	{
		Bind(Game::EVT_UPDATE, &COMRagnarosAI::Update);

	}

	auto COMRagnarosAI::Clone() const -> Game::Component*
	{
		return new COMRagnarosAI{ *this };
	}

	auto COMRagnarosAI::Update(Game::UpdateEvent const& event) -> void
	{
		if (auto obj{ GetObj().lock() }; obj != nullptr)
		{
			f32x44 transformMatrix{};
			auto transform = obj->GetComponent<Game::TransformComponent>();
			auto renderObjContainer{ obj->GetComponent<COMRenderObjectContainer>() };
			auto renderObj{ renderObjContainer->Get(CHARACTER_MESH) };
			if (!renderObj)return;
			auto skinnedMesh{ std::static_pointer_cast<SkinnedXMeshObject>(renderObj) };
			transform->GenerateTransformMatrix(&transformMatrix);
			renderObj->SetTransform(transformMatrix);
			skinnedMesh->PlayAnimation(event.GetDelta());
		}
	}
}
