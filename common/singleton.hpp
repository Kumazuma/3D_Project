#pragma once
#include<memory>
namespace Kumazuma
{
	template<typename T>
	class Singleton
	{
		template<typename ...Arg>
		static void Initialize(Arg&& ...args);
		static const std::shared_ptr<T>& Instance();
		static void Release();
	protected:

	private:
		static std::shared_ptr<T> s_instance;
	};
	template<typename T>
	inline std::shared_ptr<T> Singleton<T>::s_instance= nullptr;
	template<typename T>
	inline const std::shared_ptr<T>& Singleton<T>::Instance()
	{
		return s_instance;
	}
	template<typename T>
	template<typename ...Arg>
	inline void Singleton<T>::Initialize(Arg&& ...args)
	{
		if (s_instance != nullptr)
		{
			s_instance.reset(new T{ std::forward<Arg>(args)... });
		}
	}
	template<typename T>
	inline void Singleton<T>::Release()
	{
		if (s_instance != nullptr)
		{
			s_instance = nullptr;
		}
	}
}