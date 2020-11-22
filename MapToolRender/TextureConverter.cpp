#include "pch.h"
#include "TextureConverter.h"
#include "Texture.h"
#include "MapToolRender.h"
auto MapToolRender::TextureConverter::CanConvertFrom(ITypeDescriptorContext^ context, System::Type^ type) -> bool
{
    if (type == System::String::typeid)
    {
        return true;
    }
    if (type == MapToolRender::Texture::typeid)
    {
        return true;
    }
    return false;
}

auto MapToolRender::TextureConverter::CanConvertTo(ITypeDescriptorContext^ context, System::Type^ type) -> bool
{
    if (type == MapToolRender::Texture::typeid)
    {
        return true;
    }
    if (type == System::String::typeid)
    {
        return true;
    }
    return false;
}

auto MapToolRender::TextureConverter::CreateInstance(ITypeDescriptorContext^ context, System::Collections::IDictionary^ propertyValues) -> System::Object^ 
{
    System::String^ projectDirectory{ MapToolCore::Environment::Instance->ProjectDirectory };
    //System::String^ path{ MapToolCore::Utility::MakeRelativePath(projectDirectory, static_cast<System::String^>(value)) };
    return gcnew Texture();
}

auto MapToolRender::TextureConverter::ConvertTo(ITypeDescriptorContext^ context, Globalization::CultureInfo^ culture, System::Object^ obj, System::Type^ type) -> System::Object^
{
    if (obj == nullptr)
    {
        return nullptr;
    }
    System::String^ path = dynamic_cast<MapToolRender::Texture^>(obj)->FilePath;
    if (type == System::String::typeid)
    {
        return path;
    }
    return TypeConverter::ConvertTo(context, culture, obj, type);
}

auto MapToolRender::TextureConverter::ConvertFrom(
    ITypeDescriptorContext^ context,
    System::Globalization::CultureInfo^ culture,
    System::Object^ value) -> System::Object^ 
{
    if (value == nullptr)
    {
        return nullptr;
    }
    if (value->GetType() == MapToolRender::Texture::typeid)
    {
        return value;
    }
    if (value->GetType() == System::String::typeid)
    {
        System::String^ path{ static_cast<System::String^>(value) };
        return gcnew Texture(MapToolRender::GraphicsDevice::Instance, path);
    }
    return nullptr;
}

auto MapToolRender::TextureFileEditor::EditValue(ITypeDescriptorContext^ context, System::IServiceProvider^ provider, System::Object^ value) -> System::Object^ 
{
    System::Object^ text = FileNameEditor::EditValue(context, provider, value);
    if (text->GetType() == System::String::typeid)
    {
        return gcnew Texture(MapToolRender::GraphicsDevice::Instance,static_cast<System::String^>( text) );
    }
    return text;
}

auto MapToolRender::CubeTextureFileEditor::EditValue(ITypeDescriptorContext^ context, System::IServiceProvider^ provider, System::Object^ value) -> System::Object^ 
{
    System::Object^ text = FileNameEditor::EditValue(context, provider, value);
    if (text->GetType() == System::String::typeid)
    {
        return gcnew CubeTexture(MapToolRender::GraphicsDevice::Instance, static_cast<System::String^>(text));
    }
    return text;
}
