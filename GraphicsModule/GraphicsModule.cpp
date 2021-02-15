#include "pch.h"
#include "GraphicsModule.hpp"
#include "GraphicsModuleImpl.hpp"
#include <mutex>
namespace Kumazuma
{
	std::mutex graphicsMutex;
	GraphicsModule* GraphicsModule::Create(HWND hWindow, Size2D<u32> const& bufferSize, bool fullScreen)
	{
		try
		{
			GraphicsModuleImpl module{ hWindow, bufferSize, fullScreen };
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
