#pragma once
namespace MaptoolRenderer
{
	using namespace System::Windows::Forms::Design;
	using namespace System::ComponentModel;
	using namespace System::Windows::Forms;
	using namespace System;
	//using namespace 
	public ref class CubeTextureFileEditor:
		public FileNameEditor
	{
	protected:
		virtual void InitializeDialog(OpenFileDialog^ openFileDialog) override;
	public:
		virtual auto EditValue(
			ITypeDescriptorContext^ context,
			IServiceProvider^ provider,
			Object^ value)->Object^ override;
	};
}