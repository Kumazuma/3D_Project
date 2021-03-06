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
class WavefrontOBJMesh;
class RenderObject;
class InGameRenderer;
namespace Kumazuma
{
	namespace Graphics
	{
		class GameRenderer;
	}
	namespace Client
	{
		class HeightMap;
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
			COMPtr<ID3DXSprite> m_dxSprite;
			COMPtr<ID3DXFont>	m_dxFont;
			std::shared_ptr<bool> m_die;
			std::shared_ptr<std::mutex> m_pMutex;
			std::shared_ptr<std::atomic<LOAD_STATE> > m_threadState;
			std::wstring m_msg;
			nlohmann::json m_file;
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
			std::shared_ptr<Graphics::GameRenderer> renderer_;
			std::unique_ptr<RenderObject> m_skybox;

			std::shared_ptr<Game::Object> m_pCameraObject;
			std::shared_ptr<Game::Object> m_pPlayerObject;
			std::shared_ptr<HeightMap> m_heightMap;

		};
	}
}
