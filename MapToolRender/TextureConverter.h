#pragma once
namespace MapToolRender
{
	using namespace System::Windows::Forms;
	using namespace System::ComponentModel;
	public ref class TextureConverter: public ExpandableObjectConverter
	{
	public:
		virtual auto CanConvertFrom(ITypeDescriptorContext^ context, System::Type^ type)->bool override;
		virtual auto CanConvertTo(ITypeDescriptorContext^ context, System::Type^ type)->bool override;

		virtual auto CreateInstance(ITypeDescriptorContext^ context, System::Collections::IDictionary^ propertyValues)->System::Object^ override;
		virtual auto ConvertTo(ITypeDescriptorContext^ context, System::Globalization::CultureInfo^ culture, System::Object^ obj, System::Type^ type)->System::Object^ override;
		virtual auto ConvertFrom(ITypeDescriptorContext^ context, System::Globalization::CultureInfo^ culture, System::Object^ value)->System::Object^ override;
		//virtual auto GetProperties(ITypeDescriptorContext^ context, System::Object^ obj, array<System::Attribute^>^ attributes)->PropertyDescriptorCollection^ override;
	};

	public ref class TextureFileEditor : public System::Windows::Forms::Design::FileNameEditor
	{
	protected:
		virtual void InitializeDialog(OpenFileDialog^ openFileDialog) override
		{
			//FileNameEditor::InitializeDialog()
			FileNameEditor::InitializeDialog(openFileDialog);
			openFileDialog->Filter = "texture files (*.bmp;*.png;*.tga;*.dds)|*.bmp;*.png;*.tga;*.dds";
			
		}
	public:
		virtual auto EditValue(ITypeDescriptorContext^ context, System::IServiceProvider^ provider, System::Object^ value)->System::Object^ override;
	};
	public ref class CubeTextureFileEditor : public System::Windows::Forms::Design::FileNameEditor
	{
	protected:
		virtual void InitializeDialog(OpenFileDialog^ openFileDialog) override
		{
			//FileNameEditor::InitializeDialog()
			FileNameEditor::InitializeDialog(openFileDialog);
			openFileDialog->Filter = "texture files (*.dds)|*.dds";

		}
	public:
		virtual auto EditValue(ITypeDescriptorContext^ context, System::IServiceProvider^ provider, System::Object^ value)->System::Object^ override;
	};
}