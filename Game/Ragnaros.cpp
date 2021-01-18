#include "Ragnaros.hpp"
#include<game/TransformComponent.hpp>
#include"COMRenderObjectContainer.hpp"
#include"COMRagnarosAI.hpp"
#include"COMCollider.hpp"
#include"ResourceManager.hpp"
constexpr wchar_t CHARACTER_MESH[]{ L"CHARACTER" };

using namespace DirectX;
auto Kumazuma::Client::SpawnRagnaros(f32x3 const& initPosition) -> std::shared_ptr<Game::Object>
{
	auto resourceMgr{ ResourceManager::Instance() };
	std::shared_ptr<Game::Object> ragnaros{ new Game::Object{} };
	ragnaros->AddComponent<Game::TransformComponent>();
	ragnaros->AddComponent<COMRenderObjectContainer>();
	ragnaros->AddComponent<COMRagnarosAI>();
	ragnaros->AddComponent<COMCollider>();
	ragnaros->GetComponent<Game::TransformComponent>()->SetPosition(initPosition);
	ragnaros->GetComponent<Game::TransformComponent>()->SetScale(XMFLOAT3{ 0.01f, 0.01f,0.01f });

	//pPlayerObj->AddComponent<COMHeightMap>(std::move(heightmap));
	auto renderObj{ resourceMgr->GetSkinnedMesh(L"ragnaros") };
	auto hammer{ resourceMgr->GetOBJMesh(L"sulfuras") };
	hammer->SetVisible(false);
	renderObj->SetAnimationSet(0);
	ragnaros->GetComponent<COMRenderObjectContainer>()->Insert(CHARACTER_MESH, std::move(renderObj));
	ragnaros->GetComponent<COMRenderObjectContainer>()->Insert(L"ARM", std::move(hammer));
}
