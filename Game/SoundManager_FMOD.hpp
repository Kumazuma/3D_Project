#pragma once
#include<memory>
#include"fmod.hpp"
#include<functional>
#include<unordered_map>
#include"UnicodeHelper.h"
#include<string>
#pragma comment(lib, "fmod_vc.lib")

namespace Kumazuma
{
	template<typename EnumT>
	struct TableItem
	{
		EnumT id;
		std::wstring filePath;
		bool repeat;
	};
	
	template<typename T>
	class FMODWrapper
	{
	public:
		FMODWrapper() :m_ptr{ nullptr } {}
		FMODWrapper(FMODWrapper&& other)noexcept :m_ptr{ std::move(other.m_ptr) } { other.m_ptr = nullptr; }
		FMODWrapper(const FMODWrapper&) = delete;
		void operator=(FMODWrapper&& other)noexcept
		{
			FMODWrapper tmp{ std::move(other) };
			this->~FMODWrapper();
			new(this) FMODWrapper{ std::move(tmp) };
		}
		~FMODWrapper()
		{
			if (m_ptr != nullptr)
			{
				m_ptr->release();
				m_ptr = nullptr;
			}
		}
		T* operator->()const
		{
			return m_ptr;
		}
		T** operator&()
		{
			return &m_ptr;
		}
		explicit operator T* ()const
		{
			return m_ptr;
		}
		bool operator==(const T* const ptr)const
		{
			return m_ptr == ptr;
		}
		bool operator!=(const T* const ptr)const
		{
			return m_ptr != ptr;
		}
	private:
		T* m_ptr;
	};
	class SoundManagerFMODBase
	{
		struct Chnannel
		{
			int uid;
			FMOD::Channel* ch;
		};
	public:
		void Update();
	protected:
		int Play(FMODWrapper<FMOD::Sound>& sound);
		void Stop(FMODWrapper<FMOD::Sound>& sound);
		void Stop(int uid);
		float GetSeek(int uid);
		SoundManagerFMODBase(int channelCOunt);
		virtual ~SoundManagerFMODBase()
		{

		}
		int m_lastUid;
		FMODWrapper<FMOD::System> m_system;
		//현재 재생 중인 채널들
		std::list < Chnannel> m_playingChannels;
		std::list < FMOD::Channel*> m_idleChannels;
	};
	template<typename EnumT>
	class SoundManagerFMOD : public SoundManagerFMODBase
	{
	public:
		//사운드 파일을 로딩은 시작할 때 다 로딩한다.
		static void Initialize(const std::initializer_list<TableItem<EnumT>>& tables);
		static void Release();
		static SoundManagerFMOD& Instance();
		//사운드를 재생하는 메소드
		int Play(EnumT id);
		void Stop(EnumT id);
		void Stop(int uid);
		float GetSeek(int uid);
	private:
		SoundManagerFMOD(int channelCount);
		~SoundManagerFMOD()
		{
			m_sounds.clear();
			if (m_system != nullptr)
			{
				m_system->close();
			}
		}
	private:
		std::unordered_map<EnumT, FMODWrapper<FMOD::Sound> > m_sounds;
	private:
		static SoundManagerFMOD* s_instance;
	};
	template<typename EnumT>
	SoundManagerFMOD<EnumT>* SoundManagerFMOD<EnumT>::s_instance{ nullptr };
	template<typename EnumT>
	inline SoundManagerFMOD<EnumT>::SoundManagerFMOD(int channelCount):
		SoundManagerFMODBase{channelCount}
	{

	}
	template<typename EnumT>
	inline void SoundManagerFMOD<EnumT>::Initialize(const std::initializer_list<TableItem<EnumT>>& tables)
	{
		if (s_instance != nullptr)
		{
			return;
		}
		void* unused{};
		s_instance = new SoundManagerFMOD{128};
		auto& system = s_instance->m_system;
		
		char tmp[4096]{};

		for (auto& item : tables)
		{
			FMODWrapper<FMOD::Sound> sound;
			std::string filePath{ ConvertWideToUTF8(item.filePath) };
			FMOD_RESULT res = FMOD_OK;
			FMOD_MODE mode = FMOD_DEFAULT;
			if (item.repeat)
			{
				mode = FMOD_LOOP_NORMAL;
			}
			res = system->createSound(filePath.c_str(), mode, nullptr, &sound);
			
			if (res != FMOD_OK)
			{
				delete s_instance;
				s_instance = nullptr;
				throw std::exception("failed create sound!");
			}
			if (item.repeat)
			{
				sound->setLoopCount(-1);
			}
			s_instance->m_sounds.emplace(item.id, std::move(sound));
		}
	}
	template<typename EnumT>
	inline void SoundManagerFMOD<EnumT>::Release()
	{
		if (s_instance != nullptr)
		{
			delete s_instance;
			s_instance = nullptr;
		}
	}
	template<typename EnumT>
	inline SoundManagerFMOD<EnumT>& SoundManagerFMOD<EnumT>::Instance()
	{
		return *s_instance;
	}
	template<typename EnumT>
	inline int SoundManagerFMOD<EnumT>::Play(EnumT id)
	{
		return SoundManagerFMODBase::Play(m_sounds.at(id));
	}
	template<typename EnumT>
	inline void SoundManagerFMOD<EnumT>::Stop(EnumT id)
	{
		return SoundManagerFMODBase::Stop(m_sounds.at(id));
	}
	template<typename EnumT>
	inline void SoundManagerFMOD<EnumT>::Stop(int uid)
	{
		return SoundManagerFMODBase::Stop(uid);
	}

	template<typename EnumT>
	inline float SoundManagerFMOD<EnumT>::GetSeek(int uid)
	{
		return SoundManagerFMODBase::GetSeek(uid);
	}

}