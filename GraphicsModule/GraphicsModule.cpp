#include "pch.h"
#include "GraphicsModule.hpp"
#include "GraphicsModuleImpl.hpp"
#include <mutex>
namespace Kumazuma
{
	std::mutex graphicsMutex;
	GraphicsModule* GraphicsModule::Create( )
	{
		try
		{
			GraphicsModuleImpl module{  };
			return new GraphicsModuleImpl{std::move( module )};
		}
		catch (...)
		{

		}
		return nullptr;
	}

	void GraphicsModule::LockDevice()
	{
		graphicsMutex.lock();
	}

	void GraphicsModule::UnlockDevice()
	{
		graphicsMutex.unlock();
	}

	void GraphicsModule::lock()
	{
		graphicsMutex.lock();
	}

	void GraphicsModule::unlock()
	{
		graphicsMutex.unlock();
	}

}
