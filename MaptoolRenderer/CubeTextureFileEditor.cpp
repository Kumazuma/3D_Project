#include "pch.h"
#include "CubeTexture.hpp"
#include "CubeTextureFileEditor.hpp"
#include "MaptoolRenderer.h"
void MaptoolRenderer::CubeTextureFileEditor::InitializeDialog(OpenFileDialog^ openFileDialog)
{
	//FileNameEditor::InitializeDialog()
	FileNameEditor::InitializeDialog(openFileDialog);
	openFileDialog->Filter = "texture files (*.dds)|*.dds";

}

auto MaptoolRenderer::CubeTextureFileEditor::EditValue(
    ITypeDescriptorContext^ context,
    IServiceProvider^ provider,
    Object^ value) -> Object^ 
{
    System::Object^ text = FileNameEditor::EditValue(context, provider, value);
    if (text == nullptr)
    {
        return text;
    }
    if (text->GetType() == System::String::typeid)
    {
        return gcnew CubeTexture(MaptoolRenderer::GraphicsDevice::Instance,
            static_cast<System::String^>(text));
    }
    return text;
}
