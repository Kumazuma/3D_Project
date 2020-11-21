#pragma once

struct IDirect3D9Ex;
struct IDirect3DDevice9Ex;
class RenderObject;
class RenderModule;
struct IDirect3DTexture9;
#include "TextureConverter.h"
namespace MapToolRender {
	using namespace System::ComponentModel;
	ref class GraphicsDevice;
	[EditorAttribute(
		TextureFileEditor::typeid,
		System::Drawing::Design::UITypeEditor::typeid)]
	[TypeConverter(TextureConverter::typeid)]
	public ref class Texture: public TextureConverter
	{
	public:
		Texture(GraphicsDevice^ device, System::String^ filePath);
		Texture();
		~Texture();
		!Texture();
		[Browsable(false)]
		property IDirect3DTexture9* Handle {auto get()->IDirect3DTexture9* { return m_pTexture; }}
		[Browsable(true)]
		property System::String^ FilePath {auto get()->System::String^ { return m_filePath; }}
		property int Width
		{
			auto get()->int { return m_width; }
		}
		property int Height
		{
			auto get()->int { return m_height; }
		}
	private:
		System::String^ m_filePath;
		IDirect3DTexture9* m_pTexture;
		int m_width;
		int m_height;
	};
}
