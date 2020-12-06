#pragma once
#include "render_com.hpp"
#include "animation.hpp"
namespace Kumazuma
{
	namespace Game
	{
		template<typename Renderer>
		class AnimationObject :
			public RenderComponent<Renderer>::IDrawObject
		{
		public:
			typedef typename Renderer::Image Image;
			typedef typename Renderer::DrawContext DrawContext;
			
			AnimationObject(const _2D::Sprite<Image>& sprite);
			AnimationObject(const AnimationObject& other);
			void Render(DrawContext&, const Object& obj);
			void Render(DrawContext& context, const Kumazuma::Game::_2D::Vector2& center);
			//재생을 시작한다.
			void Play(const AnimationTag& tag, bool repeat = true);
			//애니메이션을 정지한다.
			void Pause();
			void Resume();
			void PostUpdate(float timeDelta) override;
			typename RenderComponent<Renderer>::IDrawObject* Clone() const;
		private:
			const typename _2D::Sprite<Image>::FrameList* GetNowFrameList()const;
			_2D::Frame GetNowFrame();
		protected:
			_2D::Sprite<Image> m_sprite;
			const typename _2D::Sprite<Image>::FrameList* m_pFrameList;
			const AnimationTag* m_pAnimTag;
			_2D::Frame m_nowFrame;
			bool m_isRepeat;
			bool m_isPlaying;
			int m_time;
		};
		template<typename Renderer>
		inline AnimationObject<Renderer>::AnimationObject(const _2D::Sprite<Image>& sprite) :
			m_sprite{ sprite },
			m_isRepeat{ false },
			m_isPlaying{ false },
			m_pFrameList{nullptr},
			m_time{ 0 }
		{

		}
		template<typename Renderer>
		inline AnimationObject<Renderer>::AnimationObject(const AnimationObject& other):
			m_sprite{other.m_sprite},
			m_isRepeat{ other.m_isRepeat },
			m_isPlaying{ other.m_isPlaying },
			m_time{ other.m_time },
			m_pAnimTag{ other.m_pAnimTag },
			m_pFrameList{ nullptr }
		{
			m_pFrameList = GetNowFrameList();
		}
		template<typename Renderer>
		inline void AnimationObject<Renderer>::Render(DrawContext& context, const Object& obj)
		{
			Render(context, obj.GetTransform());
		}
		template<typename Renderer>
		inline void AnimationObject<Renderer>::Render(DrawContext& context, const Kumazuma::Game::_2D::Vector2& center)
		{
			auto& image = m_sprite.GetImage();
			context.DrawImage(image, center, m_nowFrame.leftTop, m_nowFrame.rightBottom);
		}
		template<typename Renderer>
		inline void AnimationObject<Renderer>::Play(const AnimationTag& tag, bool repeat)
		{
			const auto& frames = m_sprite.GetFrames(tag);
			m_isRepeat = repeat;
			if (&frames != nullptr && &frames != m_pFrameList)
			{
				//유효한 애니메이션이여야 재생을 시작한다.
				m_pAnimTag = &tag;
				m_isPlaying = true;
				m_pFrameList = &frames;
				m_time = 0;
			}
		}
		template<typename Renderer>
		inline void AnimationObject<Renderer>::PostUpdate(float timeDelta)
		{
			if (m_isPlaying)
			{
				m_nowFrame = GetNowFrame();
				m_time += static_cast<int>(timeDelta * 1000);
			}
		}
		template<typename Renderer>
		inline typename RenderComponent<Renderer>::IDrawObject* AnimationObject<Renderer>::AnimationObject::Clone() const
		{
			return new AnimationObject(*this);
		}
		template<typename Renderer>
		inline const typename _2D::Sprite<typename Renderer::Image>::FrameList* AnimationObject<Renderer>::GetNowFrameList() const
		{
			const auto& frames = m_sprite.GetFrames(*m_pAnimTag);
			return &frames;
		}
		template<typename Renderer>
		inline _2D::Frame AnimationObject<Renderer>::GetNowFrame()
		{
			if (m_pFrameList == nullptr)
			{
				m_pFrameList = GetNowFrameList();
				if (m_pFrameList == nullptr)
				{
					return _2D::Frame{};
				}
			}
			if (m_time >= m_pFrameList->back().timestamp && m_isRepeat == false)
			{
				m_isPlaying = false;
			}
			m_time = m_time % m_pFrameList->back().timestamp;
			for (auto& it : *m_pFrameList)
			{	
				if (m_time < it.timestamp)
				{
					return it;
				}
			}
			return _2D::Frame{};
		}
		template<typename Renderer>
		inline void AnimationObject<Renderer>::AnimationObject::Pause()
		{
			m_isPlaying = false;
		}
		template<typename Renderer>
		inline void AnimationObject<Renderer>::Resume()
		{
			m_isPlaying = true;
		}
	}
}