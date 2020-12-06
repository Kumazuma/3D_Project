#pragma once
#include<map>
namespace Kumazuma
{
	using T = int;
	class Rwlock;
	
	class Rwlock
	{
	public:
		class WriteLock
		{
		public:
			T& operator *() const;
			T* operator ->()const;
		private:
		};
		class ReadLock
		{
		public:
			const T& operator *() const;
			const T* operator ->()const;
		};

	private:
		T m_value;
		thread_local static std::map<Rwlock*, int> s_writeLockCount;
	};
}