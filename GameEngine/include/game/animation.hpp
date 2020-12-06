#pragma once
#include<vector>
#include<memory>
#include<map>
#include"transform.hpp"
#include<unordered_map>
namespace Kumazuma
{
	namespace Game
	{
		class AnimationTag
		{
		public:
			AnimationTag(const char* const animName) : m_animName{ animName } {}
			bool operator == (const AnimationTag& other) const { return &other == this; }
			bool operator != (const AnimationTag& other) const { return &other != this; }
			const char* const m_animName;
		};
		namespace _2D
		{
			struct Frame
			{
				uint32_t timestamp{};
				uint32_t x;
				uint32_t y;
				uint32_t width;
				uint32_t height;
			};
			template<typename _ImageT>
			class Sprite
			{
			public:
				using FrameList = std::vector<Frame>;
				Sprite Animation(const AnimationTag& anim, std::vector<Frame>&& frames)&&;
				Sprite& Animation(const AnimationTag& anim, std::vector<Frame>&& frames)&;
				void Initialize(const std::shared_ptr<_ImageT>& image) { m_image = image; }
				Sprite Initialize(const std::shared_ptr<_ImageT>& image) const
				{
					auto tmp{ *this };
					tmp.m_image = image;
					return tmp;
				}
				const FrameList& GetFrames(const AnimationTag& animTag) const;
				const _ImageT& GetImage()const { return *m_image; }
			private:
				std::shared_ptr<_ImageT> m_image;
				std::unordered_map<const AnimationTag*, FrameList >  m_frames;
			};
			template<typename _ImageT>
			inline Sprite<_ImageT>& Sprite<_ImageT>::Animation(const AnimationTag& anim, std::vector<Frame>&& frames)&
			{
				m_frames[&anim] = std::move(frames);
				return *this;
			}
			template<typename _ImageT>
			inline Sprite<_ImageT> Sprite<_ImageT>::Animation(const AnimationTag& anim, std::vector<Frame>&& frames)&&
			{
				this->Animation(anim, std::move(frames));
				return std::move(*this);
			}
			template<typename _ImageT>
			inline const std::vector<Frame>& Sprite<_ImageT>::GetFrames(const AnimationTag& animTag) const
			{
				const auto it = m_frames.find(&animTag);
				if (it == m_frames.end())
				{
					return *(std::vector<Frame>*)nullptr;
				}
				return it->second;
				// TODO: 여기에 return 문을 삽입합니다.
			}
			
		}
	}
}