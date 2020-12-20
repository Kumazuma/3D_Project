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
#include "MapToolRenderer.h"
#include"constvar.hpp"
#include <game/ThreadPoolMgr.hpp>
using namespace Kumazuma::Client;
using namespace DirectX;
using Task = Kumazuma::ThreadPool::Task;
using TaskContext = Kumazuma::ThreadPool::TaskContext;
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
	MapToolRenderer* pRenderer{};
	MapToolRenderer::Create(renderObj.get(), renderObj->GetWidth(), renderObj->GetHeight(), &pRenderer);
	if (pRenderer == nullptr)
	{
		App::Instance()->Exit();
		return;
	}
	m_pRenderer.reset(pRenderer);

	auto resourceMgr{ ResourceManager::Instance() };
	auto base_dir{ Enviroment::GetValue<std::wstring>(Enviroment::BASE_DIR) };

	SkyBoxObject* skyboxObj{};
	COMPtr<IDirect3DCubeTexture9> pCubeTexture{};
	SkyBoxObject::Create(renderObj.get(), &skyboxObj);
	renderObj->CreateCubeTexture((base_dir + ConvertUTF8ToWide(m_file[u8"skybox"])).c_str(), &pCubeTexture);
	skyboxObj->SetDiffuseTexture(pCubeTexture.Get());
	m_skybox.reset(skyboxObj);
	std::unordered_map<std::wstring, XMFLOAT3> targets;
	for (auto it : m_file[u8"objects"])
	{

		if (it[u8"type"] == u8"OBJ_MESH")
		{
			std::wstring path{ ConvertUTF8ToWide(it[u8"path"]) };
			auto meshObj = resourceMgr->LoadOBJMesh(base_dir + path);
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
			XMFLOAT4X4 transformMat{};
			XMStoreFloat4x4(
				&transformMat,
				XMMatrixScaling				(scale.x, scale.y, scale.z) *
				XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) *
				XMMatrixTranslation			(pos.x, pos.y, pos.z)
			);
			meshObj->SetTransform(transformMat);
			m_staticMapMeshs.emplace_back(std::move(meshObj));
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
	m_pCameraObject.reset(new Game::Object{});
	m_pCameraObject->AddComponent<CameraComponent>();
	m_pCameraObject->AddComponent<Game::TransformComponent>();
	m_pCameraObject->GetComponent<Game::TransformComponent>()->SetPosition(targets[L"PLAYER_SPAWN_POSITION"]);
	XMFLOAT4X4 projMatrix;
	renderObj->GenerateProjPerspective(30.f, static_cast<f32>(WINDOW_WIDTH) / static_cast<f32>(WINDOW_HEIGHT), 0.01f, 4000.f, &projMatrix);
	m_pRenderer->SetProjMatrix(projMatrix);
}
auto TestScene::Update(f32 timeDelta) -> void
{
	Kumazuma::Game::Runtime::Instance()->Update(timeDelta);
	auto renderModule{ App::Instance()->GetRenderModule() };
	XMFLOAT3 rotation;
	XMFLOAT4X4 viewMatrix;
	auto transformCompoentn{ m_pCameraObject->GetComponent< Game::TransformComponent>() };
	XMStoreFloat3(&rotation, XMLoadFloat3(&transformCompoentn->GetRotation()) * (360.f / XM_2PI));
	renderModule->GenerateViewMatrix(transformCompoentn->GetPosition(), rotation, &viewMatrix);
	m_pRenderer->SetViewMatrix(viewMatrix);

	m_skybox->PrepareRender(m_pRenderer.get());
	for (auto& mapMash : m_staticMapMeshs)
	{
		mapMash->PrepareRender(m_pRenderer.get());
	}
	
	m_pRenderer->Render(renderModule.get());
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
		m_pMutex->unlock();
	}
	//TODO: display loading message
}

auto __cdecl Kumazuma::Client::TestLoadingScene::LoadProcess(
	TestLoadingScene* testScene,
	std::shared_ptr<std::mutex> mutex,
	std::shared_ptr<std::atomic<LOAD_STATE>> threadState, std::shared_ptr<std::wstring> msg, std::shared_ptr<bool> die) -> void
{
	*threadState = LOAD_STATE::PROGRESSING;
	std::vector<std::shared_ptr<Task> > loaders;
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
					auto meshObj = resourceMgr->LoadOBJMesh(base_dir + path);
					});
				loaders.push_back(pWork);
				
			}
		}
		for (auto& it : loaders)
		{
			it->Wait();
		}
		for (auto& it : loaders)
		{
			it->Wait();
		}

		App::Instance()->LoadScene<TestScene>(file);
		*threadState = LOAD_STATE::COMPLETE;
	}
	catch (std::wstring errMsg)
	{
		*threadState = LOAD_STATE::FAIL;
		*msg = errMsg;
	}
}
