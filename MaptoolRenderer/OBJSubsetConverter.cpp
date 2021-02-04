#include "pch.h"
#include "OBJObject.hpp"
#include "OBJSubsetConverter.hpp"
using namespace System::Reflection;

auto MaptoolRenderer::OBJSubsetConverter::CanConvertFrom(ITypeDescriptorContext^ context, System::Type^ type) -> bool 
{
    return false;
}

auto MaptoolRenderer::OBJSubsetConverter::CanConvertTo(ITypeDescriptorContext^ context, System::Type^ type) -> bool 
{
    return false;
}

auto MaptoolRenderer::OBJSubsetConverter::CreateInstance(ITypeDescriptorContext^ context, System::Collections::IDictionary^ propertyValues) -> System::Object^ 
{
    return nullptr;

}

auto MaptoolRenderer::OBJSubsetConverter::ConvertTo(ITypeDescriptorContext^ context, System::Globalization::CultureInfo^ culture, System::Object^ obj, System::Type^ type) -> System::Object^ 
{
    //FieldInfo^ fi = enumType_->GetField(Enum::GetName(enumType_, obj));
    OBJSubset^ subset{safe_cast<OBJSubset^ >(obj) };
    return subset->RenderKind.ToString();
}

auto MaptoolRenderer::OBJSubsetConverter::ConvertFrom(ITypeDescriptorContext^ context, System::Globalization::CultureInfo^ culture, System::Object^ value) -> System::Object^ 
{
    //return System::Object ^ override();
    return nullptr;
}
