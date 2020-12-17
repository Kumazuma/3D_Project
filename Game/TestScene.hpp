#pragma once
#include"../framework.h"
#include<game/scene.hpp>
#include"RenderModule.h"
#include"common.hpp"
#include"typedef.hpp"
#include<thread>
#include<memory>
#include<mutex>
#include<atomic>
#include<vector>
#include"json.hpp"
class WowMapMeshObject;
class RenderObject;
namespace Kumazuma
{
	namespace Client
	{
		class TestLoadingScene : public Kumazuma::Game::Scene
		{
		public:
			enum class LOAD_STATE:u8 {
				NONE,
				PROGRESSING,
				COMPLETE,
				FAIL
			};
			~TestLoadingScene();
			auto Loaded()->void override;
			auto Update(f32 timeDelta)->void;
		protected:
			static auto __cdecl LoadProcess(
				TestLoadingScene* testScene,
				std::shared_ptr<std::mutex> mutex,
				std::shared_ptr<std::atomic<LOAD_STATE> > threadState,
				std::shared_ptr<std::wstring> errMsg,
				std::shared_ptr<bool> die)->void;
		private:
			std::shared_ptr<bool> m_die;
			std::shared_ptr<std::mutex> m_pMutex;
			std::shared_ptr<std::atomic<LOAD_STATE> > m_threadState;
			std::wstring m_msg;
			std::shared_ptr<std::wstring> m_threadMsg;
			std::thread m_loadingThread;
		};
		class TestScene : public Kumazuma::Game::Scene
		{
		public:
			TestScene(nlohmann::json file);
			~TestScene();
			auto Loaded()->void override;
			auto Update(f32 timeDelta)->void;
		private:
			nlohmann::json m_file;
			std::unique_ptr<IRenderer> m_pRenderer;
			std::unique_ptr<RenderObject> m_skybox;
			std::vector<std::unique_ptr<WowMapMeshObject> > m_staticMapMeshs;
			std::shared_ptr<Game::Object> m_pCameraObject;
		};
	}
}
