#include "pch.h"
#include"./include/kumawin/messagehandler.hpp"
namespace Kumazuma
{
	namespace WindowSystem
	{
		const WinMsgID<WinMsgEvent> EVT_Destroy{};
		const WinMsgID<WinMsgEvent> EVT_Paint{};
		const WinMsgID<WinMsgEvent> EVT_Timer{};
		const WinMsgID<WinMsgEvent> EVT_KeyDown{};
		const WinMsgID<WinMsgEvent> EVT_KeyUp{};
		const WinMsgID<WinSizeEvent> EVT_Size{};
	}
}

