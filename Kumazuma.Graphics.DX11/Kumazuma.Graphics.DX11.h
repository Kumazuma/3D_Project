#pragma once

using namespace System;
using namespace System::Windows::Forms;
namespace Kumazuma::DX11 {
	
	public ref class GraphicsDevice
	{
	private:
		static GraphicsDevice^ s_instance;
	public:
		static bool Initialize(Control^ defaultControl, unsigned backbufferWidth, unsigned backbufferHeight);

	};
}
