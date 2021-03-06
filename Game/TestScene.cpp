#include"pch.h"
#include"TestScene.hpp"
#include"COMPtr.hpp"
#include"app.h"
#include<game/objectfactory.hpp>
#include<game/TransformComponent.hpp>
#include"json.hpp"
#include<fstream>
#include"Env.hpp"
#include"CameraComponent.hpp"
#include"ResourceManager.hpp"
#include"UnicodeHelper.h"
#include"SkyBoxObject.h"
#include <future>
#include "Renderer.h"
#include "InGameRenderer.hpp"
#include"constvar.hpp"
#include <game/ThreadPoolMgr.hpp>
#include "CharacterMeta.hpp"
#include "COMRenderObjectContainer.hpp"
#include "Player.h"
#include "Ragnaros.hpp"
#include "HeightMapBuilder.hpp"
#include "PhysicsManager.hpp"
#include "COMRagnarosAI.hpp"
#include "LayerTags.hpp"
#include "SimpleTransform.hpp"
#include "COMCollider.hpp"
#include <GameRenderer.hpp>
#include"COMTerrainRender.hpp"
#include "MainUI.hpp"
using namespace Kumazuma::Client;
using namespace DirectX;
using Task = Kumazuma::ThreadPool::Task;
using TaskContext = Kumazuma::ThreadPool::TaskContext;
constexpr wchar_t CHARACTER_MESH[]{ L"CHARACTER" };

Kumazuma::Client::TestScene::TestScene(nlohmann::json file)
	
{
	m_file = file;
}
Kumazuma::Client::TestScene::~TestScene()
{

}
void TestScene::Loaded()
{
	auto renderObj{ App::Instance()->GetRenderModule() };
	Graphics::GameRenderer* pRenderer{};
	Graphics::GameRenderer::Create(renderObj.get(), &pRenderer);
	COMPtr<IDirect3DDevice9> pDevice;
	renderObj->GetDevice(&pDevice);
	pDevice->ShowCursor(false);
	if (pRenderer == nullptr)
	{
		App::Instance()->Exit();
		return;
	}
	renderer_.reset(pRenderer);
	
	auto resourceMgr{ ResourceManager::Instance() };
	auto base_dir{ Enviroment::GetValue<std::wstring>(Enviroment::BASE_DIR) };

	//SkyBoxObject* skyboxObj{};
	COMPtr<IDirect3DCubeTexture9> pCubeTexture{};
	//SkyBoxObject::Create(renderObj.get(), &skyboxObj);
	renderObj->CreateCubeTexture((base_dir + ConvertUTF8ToWide(m_file[u8"skybox"])).c_str(), &pCubeTexture);
	pRenderer->SetSkyBox(pCubeTexture.Get());
	//skyboxObj->SetDiffuseTexture(pCubeTexture.Get());
	//m_skybox.reset(skyboxObj);
	std::unordered_map<std::wstring, XMFLOAT3> targets;
	//std::vector<std::unique_ptr<WavefrontOBJMesh> > mapMeshs;
	//std::vector<SimpleTransform> transforms;
	for (auto it : m_file[u8"objects"])
	{
		if (it[u8"type"] == u8"OBJ_MESH")
		{
			std::wstring path{ ConvertUTF8ToWide(it[u8"path"]) };

			auto meshObj{ std::make_shared<Game::Object>() };
			meshObj->AddComponent<Game::TransformComponent>();
			auto transform{ meshObj->GetComponent<Game::TransformComponent>() };
			auto mesh{ resourceMgr->GetOBJMesh(base_dir + path) };
			auto const& jTransform{ it[u8"transform"] };
			auto const& jPosition{ jTransform[u8"position"] };
			auto const& jScale{ jTransform[u8"scale"] };
			auto const& jRotation{ jTransform[u8"rotation"] };
			XMFLOAT3 pos{
				jPosition[u8"x"],
				jPosition[u8"y"],
				jPosition[u8"z"]
			};
			float scale{ 1.f };
			if (jScale.is_object())
			{
				scale = jScale[u8"x"];
			}
			else if(jScale.is_number())
			{
				scale = jScale;
			}
			XMFLOAT3 rotation{
				jRotation[u8"x"],
				jRotation[u8"y"],
				jRotation[u8"z"]
			};
			transform->SetPosition(pos);
			transform->SetRotation(rotation);
			transform->SetScale(scale);

			std::string usage{ it[u8"usage"] };
			if (usage == u8"TERRAIN")
			{
				meshObj->AddComponent<COMTerrainRender>(renderer_, std::shared_ptr< WavefrontOBJMesh>{ static_cast<WavefrontOBJMesh*>(mesh->Clone()) });
				m_objects[&LAYER_TERRAIN].push_back(std::move(meshObj));
				//SimpleTransform st{};
				//st.scale = scale;
				//st.rotation = rotation;
				//st.position = pos;
				//transforms.emplace_back(st);
				//mapMeshs.emplace_back(std::move(mesh));
			}
			else
			{
				//TODO:
				//m_staticMapMeshs.emplace_back(std::move(meshObj));
			}
		}
		else if (it[u8"type"] == u8"TARGET")
		{
			std::wstring path{ ConvertUTF8ToWide(it[u8"name"]) };
			XMFLOAT3 pos{
				it[u8"transform"][u8"position"][u8"x"],
				it[u8"transform"][u8"position"][u8"y"],
				it[u8"transform"][u8"position"][u8"z"]
			};
			targets.emplace(path, pos);
		}
	}
	auto physicsManager{ Client::PhysicsManager::Instance() };
	//physicsManager->SetMap(mapMeshs, transforms);
	physicsManager->SetCharacterColliderCapsule(L"PLAYER_CAPSULE", 0.2f, 8.f);

	auto position{ targets[L"PLAYER_SPAWN_POSITION"] };

	m_pPlayerObject = SpawnPlayer(renderer_, position);
	m_pPlayerObject->GetComponent<Game::TransformComponent>()->SetPosition(position);
	m_pPlayerObject->GetComponent<Game::TransformComponent>()->SetScale(0.1f);
	m_objects[&LAYER_PLAYER].push_back(m_pPlayerObject);

	m_pCameraObject.reset(new Game::Object{});
	m_pCameraObject->AddComponent<CameraComponent>(m_pPlayerObject);
	m_pCameraObject->AddComponent<Game::TransformComponent>();
	m_pCameraObject->GetComponent<Game::TransformComponent>()->SetPosition(position);

	XMFLOAT4X4 projMatrix;
	renderObj->GenerateProjPerspective(30.f, static_cast<f32>(WINDOW_WIDTH) / static_cast<f32>(WINDOW_HEIGHT), 0.1f, 1000.f, &projMatrix);
	renderer_->SetProjMatrix(projMatrix);
	renderer_->SetNearFar(0.f, 500.f);
	AddObject(LAYER_ETC, SpawnUI(renderer_, m_pPlayerObject));
	{
		auto resourceMgr{ ResourceManager::Instance() };

		for (auto i = 0; i < 1; ++i)
		{
			f32x3 ragnarosPosition{ -1187.73f,  16.1408f, 575.176f };
			auto ragnaros{ SpawnRagnaros(renderer_ , ragnarosPosition) };

			m_objects[&LAYER_MONSTER].push_back(ragnaros);
		}
	}
}
auto TestScene::Update(f32 timeDelta) -> void
{
	auto runtime{ Kumazuma::Game::Runtime::Instance() };
	auto physicsManager{ PhysicsManager::Instance() };
	runtime->Update(timeDelta);
	physicsManager->Update(timeDelta);
	runtime->DispatchEvent();
	auto renderModule{ App::Instance()->GetRenderModule() };
	XMFLOAT3 rotation;
	XMFLOAT4X4 viewMatrix;

	D3DLIGHT9 light{};
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = D3DCOLORVALUE{ 0.1f, 0.1f, 0.1f, 0.1f };
	light.Diffuse = D3DCOLORVALUE{ 1.0f, 1.0f, 1.0f, 1.0f };
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&light.Direction), XMVector3Normalize(XMVectorSet(1.f, -2.f, 0.f, 0.f)));
	renderer_->AddLight(light);

	renderer_->SetViewMatrix(m_pCameraObject->GetComponent<CameraComponent>()->GetViewMatrix());
	renderer_->Render();

	COMPtr<IDirect3DDevice9> pDevice;
	renderModule->GetDevice(&pDevice);
	pDevice->Present(nullptr, nullptr, nullptr, nullptr);
}
TestLoadingScene::~TestLoadingScene()
{
	if (m_die != nullptr)
	{
		*m_die = true;
	}
	if (m_loadingThread.joinable())
	{
		m_loadingThread.join();
	}
}

auto TestLoadingScene::Loaded() -> void 
{
	m_die = std::make_shared<bool>(false);
	m_threadMsg = std::make_shared<std::wstring>();
	m_pMutex = std::make_shared<std::mutex>();
	m_threadState = std::make_shared<std::atomic<LOAD_STATE> >();
	m_loadingThread = std::thread(&TestLoadingScene::LoadProcess, this,  m_pMutex, m_threadState, m_threadMsg, m_die);
}

auto TestLoadingScene::Update(f32 timeDelta) -> void
{
	if (m_pMutex->try_lock())
	{
		m_msg = *m_threadMsg;
		LOAD_STATE currentState{ *m_threadState };
		auto renderModule{ App::Instance()->GetRenderModule() };
		switch (currentState)
		{
		case LOAD_STATE::COMPLETE:
			App::Instance()->LoadScene<TestScene>(m_file);
			break;
		case LOAD_STATE::FAIL:
			MessageBoxW(nullptr, m_msg.c_str(), nullptr, MB_ICONERROR);
			App::Instance()->Exit();
			break;
		}

		m_pMutex->unlock();
	}
	//TODO: display loading message
}

auto __cdecl Kumazuma::Client::TestLoadingScene::LoadProcess(
	TestLoadingScene* testScene,
	std::shared_ptr<std::mutex> mutex,
	std::shared_ptr<std::atomic<LOAD_STATE>> threadState,
	std::shared_ptr<std::wstring> msg, std::shared_ptr<bool> die) -> void
{
	*threadState = LOAD_STATE::PROGRESSING;
	std::unordered_map<std::wstring, std::shared_ptr<Task> > loaders;
	auto pThreadPoolMgr{ Kumazuma::ThreadPool::Manager::Instance() };
	try
	{
		auto resourceMgr{ ResourceManager::Instance() };
		std::ifstream istream;
		auto base_dir{ Enviroment::GetValue<std::wstring>(Enviroment::BASE_DIR) };
		auto mapFilePath{ base_dir + L"/json/map1.json" };
		istream.open(mapFilePath);
		if (!istream.is_open())
		{
			throw L"Can not Open Map File!";
		}
		nlohmann::json file{ nlohmann::json::parse(istream) };
		for (auto it : file[u8"objects"])
		{
			if (it[u8"type"] == u8"OBJ_MESH")
			{
				std::wstring path{ ConvertUTF8ToWide(it[u8"path"]) };
				//auto meshObj = resourceMgr->LoadOBJMesh(base_dir + path);

				std::shared_ptr<Task> pWork = nullptr;
				pWork = 
				pThreadPoolMgr->QueueTask([base_dir, path](TaskContext& context) {
					auto resourceMgr{ ResourceManager::Instance() };
					resourceMgr->LoadOBJMesh(base_dir + path);
					});
				if (loaders.find(path) == loaders.end())
				{
					loaders.emplace(path, std::move(pWork));
				}
			}
		}
		//skinnedmesh폴더의 json파일들을 순회하면서 메시파일을 파싱합니다.
		//HFILE 
		WIN32_FIND_DATAW ffd{};
		HANDLE hFind = INVALID_HANDLE_VALUE;
		std::wstring animDir{ base_dir + L"/json/skinnedmesh/" };
	
		hFind = FindFirstFileW((animDir + L"*.json").c_str(), &ffd);
		do
		{
			if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				continue;
			std::wstring name{ ffd.cFileName };
			if (auto index = name.find_last_of(L'.'); index != std::wstring::npos)
			{
				std::wstring nameWithoutExt{ name.substr(0, index) };
				index += 1;
				std::wstring_view view(name.c_str() + index, name.size() - index);
				if (view != L"json")continue;
				resourceMgr->LoadCharacterMetaData(nameWithoutExt, animDir + name);
				auto meta{ resourceMgr->GetCharacterMeta(nameWithoutExt) };
				auto meshPath{ base_dir + meta->GetMeshPathRef() };
				std::shared_ptr<Task> pWork = nullptr;
				pWork =
					pThreadPoolMgr->QueueTask([nameWithoutExt,  meshPath](TaskContext& context) {
					auto resourceMgr{ ResourceManager::Instance() };
					auto meshObj = resourceMgr->LoadSkinnedMesh(nameWithoutExt, meshPath);
						});
				if (loaders.find(meshPath) == loaders.end())
				{
					loaders.emplace(meshPath, std::move(pWork));
				}
			}
		} while (FindNextFileW(hFind, &ffd));
		//CloseHandle(hFind);
		std::ifstream itemMeshMetaFileIStream;
		itemMeshMetaFileIStream.open(base_dir + L"/json/item_mesh.json");
		nlohmann::json itemMeshMeta{ nlohmann::json::parse( itemMeshMetaFileIStream) };
		for (auto const& it : itemMeshMeta.items())
		{
			std::wstring key{ ConvertUTF8ToWide(it.key()) };
			std::wstring path{ ConvertUTF8ToWide(it.value()) };
			std::shared_ptr<Task> pWork = nullptr;
			pWork =
				pThreadPoolMgr->QueueTask([base_dir, key, path](TaskContext& context) {
				auto resourceMgr{ ResourceManager::Instance() };
				resourceMgr->LoadOBJMesh(base_dir + path, key);
					});
			if (loaders.find(path) == loaders.end())
			{
				loaders.emplace(path, std::move(pWork));
			}
		}
		for (auto& it : loaders)
		{
			it.second->Wait();
		}

		std::vector<std::unique_ptr<WavefrontOBJMesh> > mapMeshs;
		std::vector<SimpleTransform> transforms;
		for (auto it : file[u8"objects"])
		{
			if (it[u8"type"] == u8"OBJ_MESH")
			{
				std::wstring path{ ConvertUTF8ToWide(it[u8"path"]) };

				auto meshObj{ std::make_shared<Game::Object>() };
				meshObj->AddComponent<Game::TransformComponent>();
				auto transform{ meshObj->GetComponent<Game::TransformComponent>() };
				auto mesh{ resourceMgr->GetOBJMesh(base_dir + path) };
				XMFLOAT3 pos{
					it[u8"transform"][u8"position"][u8"x"],
					it[u8"transform"][u8"position"][u8"y"],
					it[u8"transform"][u8"position"][u8"z"]
				};
				XMFLOAT3 scale{
					it[u8"transform"][u8"scale"][u8"x"],
					it[u8"transform"][u8"scale"][u8"y"],
					it[u8"transform"][u8"scale"][u8"z"]
				};
				XMFLOAT3 rotation{
					it[u8"transform"][u8"rotation"][u8"x"],
					it[u8"transform"][u8"rotation"][u8"y"],
					it[u8"transform"][u8"rotation"][u8"z"]
				};
				transform->SetPosition(pos);
				transform->SetRotation(rotation);
				transform->SetScale(scale.x);

				std::string usage{ it[u8"usage"] };
				if (usage == u8"TERRAIN")
				{
					//meshObj->AddComponent<COMTerrainRender>(renderer_, std::shared_ptr< WavefrontOBJMesh>{ static_cast<WavefrontOBJMesh*>(mesh->Clone()) });
					//m_objects[&LAYER_TERRAIN].push_back(std::move(meshObj));
					SimpleTransform st{};
					st.scale = scale.x;
					st.rotation = rotation;
					st.position = pos;
					transforms.emplace_back(st);
					mapMeshs.emplace_back(std::move(mesh));
				}
				else
				{
					//TODO:
					//m_staticMapMeshs.emplace_back(std::move(meshObj));
				}
			}
		}
		auto physicsManager{ Client::PhysicsManager::Instance() };
		physicsManager->SetMap(mapMeshs, transforms);

		testScene->m_file = file;
		*threadState = LOAD_STATE::COMPLETE;
	}
	catch (std::wstring errMsg)
	{
		*threadState = LOAD_STATE::FAIL;
		*msg = errMsg;
	}
}
