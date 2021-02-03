#include "SoundManager_FMOD.hpp"
#include <fmod.hpp>
#include <fmod_errors.h>
#include <functional>
class EventTag
{
public:
	bool operator==(const EventTag& tag)const { return this == &tag; }
	bool operator!=(const EventTag& tag)const { return this != &tag; }
};
template<size_t count, typename ...Args>
class EventTagEx
{

};
template<typename Class, typename ...Args>
class Handler
{
public:
	using Method = void(Class::*)(Args...);
	Handler(Method method) :m_method{ method } {}
	void Call(Class* obj, const Args&... args) const
	{
		(obj->*m_method)(args...);
	}
	void operator()(Class* obj, const Args&... args) const
	{
		(obj->*m_method)(args...);
	}
private:
	Method m_method;
};
class EvtQueueItem
{

};
template<typename Class, typename ...Args>
class EventCallHandler
{
public:
	EventCallHandler(Args&& ...args)
	{
		auto test = [args...](Class* obj, Handler<Class, Args...> handler)
		{
			handler.Call(obj, args...);
		};
		m_closure = test;
	}
	void Call(Class* obj, Handler<Class, Args...> handler)
	{
		m_closure(obj, handler);
	}
	std::function<void(Class* obj, Handler<Class, Args...> handler)> m_closure;
};

void Kumazuma::SoundManagerFMODBase::Update()
{
	m_system->update();
	auto it{ m_playingChannels.begin() };
	auto const end{ m_playingChannels.end() };
	while (it != end)
	{
		auto* pChannel{ it->ch };
		bool isPlaying{};
		FMOD::Sound* pSound{};
		pChannel->getCurrentSound(&pSound);
		if (pSound == nullptr)
		{
			it = m_playingChannels.erase(it);
			m_idleChannels.push_back(pChannel);
			continue;
		}
		pChannel->isPlaying(&isPlaying);
		if (isPlaying == false)
		{
			it = m_playingChannels.erase(it);
			m_idleChannels.push_back(pChannel);
			
			continue;
		}

		++it;
	}
}

int Kumazuma::SoundManagerFMODBase::Play(FMODWrapper<FMOD::Sound>& sound)
{
	SoundManagerFMODBase::Update();
	FMOD_RESULT fr{};
	FMOD::Channel* pChannel{};
	FMOD::Sound* pCurrentSound{};
	int loopCount{};
	
	if (!m_idleChannels.empty())
	{
		pChannel = m_idleChannels.front();
		m_idleChannels.pop_front();
		fr = m_system->playSound(*&sound, nullptr, false, &pChannel);
		int id{ ++m_lastUid };
		m_playingChannels.push_back({ id, pChannel });
		m_system->update();
		return id;
	}
	//업데이트 했는데도 빈 채널이 없다면,
	auto it = m_playingChannels.begin();
	auto const end = m_playingChannels.end();
	//현재 플레이 중인 채널 중에서 가장 먼저 재생된 채널을 개장한다.
	//대신 반복 사운드(==BGM) 빼고
	while (end != it)
	{
		pChannel = it->ch;
		pChannel->getCurrentSound(&pCurrentSound);
		pCurrentSound->getLoopCount(&loopCount);
		if (loopCount != -1)
		{
			fr = m_system->playSound(*&sound, nullptr, false, &pChannel);
			m_system->update();
			m_playingChannels.erase(it);
			int id{ ++m_lastUid };
			m_playingChannels.push_back({ id,pChannel });
			return id;
		}
		++it;
	}
	return -1;
}

void Kumazuma::SoundManagerFMODBase::Stop(FMODWrapper<FMOD::Sound>& sound)
{
	auto it = m_playingChannels.begin();
	const auto end = m_playingChannels.end();
	while (it != end)
	{
		bool isPlaying{};
		auto* channel = it->ch;
		FMOD::Sound* currentSound{};
		do
		{
			channel->getCurrentSound(&currentSound);
			if (sound == currentSound)
			{
				channel->stop();
				m_system->update();
				m_playingChannels.erase(it);
				m_idleChannels.push_back(channel);
				return;
			}
		} while (false);
		++it;
	}
	return;
}

void Kumazuma::SoundManagerFMODBase::Stop(int uid)
{
	auto it = m_playingChannels.begin();
	const auto end = m_playingChannels.end();
	while (it != end)
	{
		bool isPlaying{};
		auto* channel = it->ch;
		if (uid == it->uid)
		{
			channel->stop();
			m_system->update();
			m_playingChannels.erase(it);
			m_idleChannels.push_back(channel);
			return;
		}
		++it;
	}
	return;
}

float Kumazuma::SoundManagerFMODBase::GetSeek(int uid)
{
	auto it = m_playingChannels.begin();
	const auto end = m_playingChannels.end();
	while (it != end)
	{
		bool isPlaying{};
		auto* channel = it->ch;
		if (uid == it->uid)
		{
			unsigned position{};
			unsigned length{};
			FMOD::Sound* currentSound{};

			channel->getPosition(&position, FMOD_TIMEUNIT_MS);
			channel->getCurrentSound(&currentSound);
			currentSound->getLength(&length, FMOD_TIMEUNIT_MS);
			return static_cast<float>(position)/ static_cast<float>(length);
		}
		++it;
	}
	return NAN;
}

Kumazuma::SoundManagerFMODBase::SoundManagerFMODBase(int channelCount)
{
	m_lastUid = -1;
	FMOD::System_Create(&m_system);
	void* unused{};
	m_system->init(channelCount, FMOD_INIT_NORMAL, unused);
	for (int i = 0; i < channelCount; ++i)
	{
		FMOD::Channel* pChannel{};
		m_system->getChannel(i, &pChannel);
		m_idleChannels.push_back(pChannel);
	}

}
