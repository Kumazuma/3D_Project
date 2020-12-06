#pragma once
#include"runtime.hpp"
#include"gdi_render.hpp"
namespace Kumazuma
{
	namespace Game
	{
		class Runtime : public RuntimeBase
		{
		public:
			using Renderer = Kumazuma::Game::gdi::Renderer;
			static void Initialize(Renderer&& renderer);
			static const std::shared_ptr<Runtime>& Instance();
		protected:
			Runtime(Renderer&& renderer);
		protected:
			Renderer m_renderer;
		private:
			static std::shared_ptr<Runtime> s_instance;
		};
		inline Runtime::Runtime(Renderer&& renderer):
			RuntimeBase{},
			m_renderer{std::move(renderer)}
		{

		}
		inline void Runtime::Initialize(Renderer&& renderer)
		{
			if (s_instance != nullptr)
			{
				s_instance.reset(new Runtime{ std::move(renderer) });
			}
		}
		inline const std::shared_ptr<Runtime>& Runtime::Instance()
		{
			return s_instance;
		}
	}
}