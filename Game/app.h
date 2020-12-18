#pragma once

#include "resource.h"
#include "framework.h"
#include <iostream>
#include <kumawin/kumawin.hpp>
#include <game/scene.hpp>
#include "common.hpp"
#include <atomic>
#include <fstream>
class RenderModule;
namespace Kumazuma
{
	class App
	{
	public:
		using Scene = Kumazuma::Game::Scene;
		using Window = Kumazuma::WindowSystem::Window;
		static fn_ Initialize(HINSTANCE)->bool;
		static fn_ Instance()->const std::shared_ptr<App>& { return s_instance; }
		static fn_ Release()->void;
	public:
		~App();
		fn_ Loop()->int;
		//현재 로딩한 신을 가져온다.
		fn_ GetScene()const->Scene& { return *m_pNowScene; }
		fn_ GetWindow()->Window& { return m_mainWindow; }
		template<typename _SceneT>
		fn_ LoadScene(_SceneT&& scene)->bool;
		template<typename _SceneT, typename ...Args>
		fn_ LoadScene(Args&& ...args)->bool;
		fn_ GetCursorPosition()const->POINT;
		//fn_ GetTextureManager()->std::shared_ptr<kzTextureManager> { return m_textureManager; }
		//fn_ GetGraphicsManager()->GameGraphics& {return *m_grapics;}
		//fn_ GetAnimationManager()->const std::unordered_map<std::string, std::shared_ptr<kzMetaAnimation>>& {return m_animations;}
		fn_ GetRenderModule()->std::shared_ptr<RenderModule> {
			return m_renderModule;
		}
		fn_ Exit()->void;
		fn_ GetClientWidth()const->u32;
		fn_ GetClientHeight()const->u32;
	private:
		App(HINSTANCE hInstance);
		static fn_ SetDPICheck()->void;
	private:
		fn_ OnMainWindowDestory(Kumazuma::WindowSystem::Event&)->void;
		fn_ OnMainWindowPaint(Kumazuma::WindowSystem::Event&)->void;
		fn_ OnKeyDown(Kumazuma::WindowSystem::WinMsgEvent&)->void;
		fn_ OnTimer(Kumazuma::WindowSystem::Event&)->void;
		fn_ OnSized(Kumazuma::WindowSystem::WinSizeEvent&)->void;
		fn_ LoadJSONData() -> void;
		fn_ _LoadScene(Scene* pScene)->bool;
	private:
		std::streambuf* m_orig;
		std::ofstream m_logFileStream;
		bool m_isRunning;

		uint32_t m_clientWidth;
		uint32_t m_clientHeight;
		std::shared_ptr<RenderModule> m_renderModule;
		//std::shared_ptr<kzTextureManager> m_textureManager;
		//std::unordered_map<std::string, std::shared_ptr<kzMetaAnimation>> m_animations;
		//std::unique_ptr<GameGraphics> m_grapics;
		//Renderer m_renderer;
		Scene* m_pNowScene;
		std::atomic <Scene*> m_pNextScene;
		int64_t m_preTick;
		Window m_mainWindow;
		HACCEL m_hAccelTable;
	private:
		static std::shared_ptr<App> s_instance;
	};
	template<typename _SceneT>
	inline auto App::LoadScene(_SceneT&& scene) ->bool
	{
		return _LoadScene(new _SceneT{ std::move(scene) });
	}
	template<typename _SceneT, typename ...Args>
	inline auto App::LoadScene(Args&& ...args)->bool
	{
		return _LoadScene(new _SceneT{ std::forward<Args>(args)... });
	}
}