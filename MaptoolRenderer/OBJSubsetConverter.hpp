#pragma once
namespace MaptoolRenderer
{
	using namespace System::ComponentModel;
	using namespace System;
	public ref class OBJSubsetConverter : public EnumConverter
	{
		Type^ enumType_;
	public:
		OBJSubsetConverter(Type^ type) :EnumConverter(type) { enumType_ = type; }

		virtual auto CanConvertFrom(ITypeDescriptorContext^ context, System::Type^ type)->bool override;
		virtual auto CanConvertTo(ITypeDescriptorContext^ context, System::Type^ type)->bool override;
		virtual auto CreateInstance(ITypeDescriptorContext^ context, System::Collections::IDictionary^ propertyValues)->System::Object^ override;
		virtual auto ConvertTo(ITypeDescriptorContext^ context, System::Globalization::CultureInfo^ culture, System::Object^ obj, System::Type^ type)->System::Object^ override;
		virtual auto ConvertFrom(ITypeDescriptorContext^ context, System::Globalization::CultureInfo^ culture, System::Object^ value)->System::Object^ override;
	};
}