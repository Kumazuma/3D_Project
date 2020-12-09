#pragma once
namespace DirectX
{
	struct XMFLOAT3;
}
namespace MapToolRender
{
	using namespace System;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::ComponentModel;

	[TypeConverter(ExpandableObjectConverter::typeid)]
	public ref struct Transform : public INotifyPropertyChanged
	{
		virtual event PropertyChangedEventHandler^ PropertyChanged;
	public:
		Transform();
		Transform(Transform^  rhs);
		auto Clone()->Transform^;
		[CategoryAttribute("Transform"), Browsable(true)]
		property MapToolCore::Rotation Rotation
		{
			auto get()->MapToolCore::Rotation;
			auto set(MapToolCore::Rotation value)->void;
		}
		[CategoryAttribute("Transform"), Browsable(true)]
		property MapToolCore::Position Position
		{
			auto get()->MapToolCore::Position;
			auto set(MapToolCore::Position value)->void;
		}
		[CategoryAttribute("Transform"), Browsable(true)]
		property MapToolCore::Scale Scale
		{
			auto get()->MapToolCore::Scale;
			auto set(MapToolCore::Scale value)->void;
		}

	private:
		PropertyChangedEventHandler^ m_rotationChangedHandler;
		PropertyChangedEventHandler^ m_scaleChangedHandler;
		PropertyChangedEventHandler^ m_positionChangedHandler;
		auto OnChangedRotation(Object^ obj, PropertyChangedEventArgs^ e)->void;
		auto OnChangedScale(Object^ obj, PropertyChangedEventArgs^ e)->void;
		auto OnChangedPosition(Object^ obj, PropertyChangedEventArgs^ e)->void;
		auto OnPropertyChanged(String^ info)
		{
			PropertyChanged(this, gcnew PropertyChangedEventArgs(info));
		}
		MapToolCore::Position position;
		MapToolCore::Rotation rotation;
		MapToolCore::Scale scale;
	};
}