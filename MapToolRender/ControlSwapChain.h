#pragma once
struct IDirect3DSwapChain9;
namespace MapToolRender
{
	ref class GraphicsDevice;
	public ref class SwapChain
	{
	public:
		enum class PixelFormat {
			NONE,
			R8G8B8A8
		};
		SwapChain(GraphicsDevice^ device, System::Windows::Forms::Control^ control, unsigned int width,unsigned int height, PixelFormat pixelFormat, bool fullscreen);
		!SwapChain();
		~SwapChain();
		property IDirect3DSwapChain9* Native {auto get()->IDirect3DSwapChain9* { return m_pSwapChain; } }
	protected:
		IDirect3DSwapChain9* m_pSwapChain;
	};

}