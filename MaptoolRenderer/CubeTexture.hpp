#pragma once

struct IDirect3DCubeTexture9;
namespace MaptoolRenderer
{
	using namespace System::ComponentModel;
	ref class GraphicsDevice;
	ref class CubeTextureFileEditor;
	[EditorAttribute(
		CubeTextureFileEditor::typeid,
		System::Drawing::Design::UITypeEditor::typeid)]
	[TypeConverter(ExpandableObjectConverter::typeid)]
	public ref class CubeTexture
	{
	public:
		CubeTexture(GraphicsDevice^ device, System::String^ filePath);
		CubeTexture();
		~CubeTexture();
		!CubeTexture();

		[Browsable(true)]
		property System::String^ FilePath {auto get()->System::String^ { return filePath_; }}
		property int Width
		{
			auto get()->int { return width_; }
		}
		property int Height
		{
			auto get()->int { return height_; }
		}
		virtual auto ToString()->System::String^ override { return filePath_; }
	internal:
		property IDirect3DCubeTexture9* Handle {auto get()->IDirect3DCubeTexture9* { return texture_; }}
	private:
		System::String^ filePath_;
		IDirect3DCubeTexture9* texture_;
		int width_;
		int height_;
	};
}