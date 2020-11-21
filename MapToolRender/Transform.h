#pragma once
namespace MapToolRender
{
	using namespace System;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::ComponentModel;
	using namespace System::ComponentModel;
	[TypeConverter(ExpandableObjectConverter::typeid)]
	public ref struct Position: public INotifyPropertyChanged, public ExpandableObjectConverter
	{
		virtual event PropertyChangedEventHandler^ PropertyChanged;
	public:
		Position();
		Position(float x, float y, float z);
		Position(Position^ rhs);
		[CategoryAttribute("Position"),
			DescriptionAttribute("X ��ǥ")]
		property float X
		{
			auto get()->float;
			auto set(float x)->void;
		}
		[CategoryAttribute("Position"),
			DescriptionAttribute("Y ��ǥ")]
		property float Y
		{
			auto get()->float;
			auto set(float value)->void;
		}
		[CategoryAttribute("Position"),
			DescriptionAttribute("Z ��ǥ")]
		property float Z
		{
			auto get()->float;
			auto set(float value)->void;
		}
		virtual auto ToString()->System::String^ override
		{
			return String::Format("({0}, {1}, {2})", X, Y, Z);
		}
	private:
		void OnPropertyChanged(String^ info)
		{
			PropertyChanged(this, gcnew PropertyChangedEventArgs(info));
		}
	private:
		float x;
		float y;
		float z;
	};
	//�÷��� �ִϸ��̼��� ���� �� �����ؾ� �Ѵ�.
	[TypeConverter(ExpandableObjectConverter::typeid)]
	public ref struct Scale : public INotifyPropertyChanged, public ExpandableObjectConverter
	{
		virtual event PropertyChangedEventHandler^ PropertyChanged;
	public:
		Scale();
		Scale(float x, float y, float z);
		Scale(Scale^  rhs);

		[CategoryAttribute("Scale"),
			DescriptionAttribute("�������� X���")]
		property float X
		{
			auto get()->float;
			auto set(float x)->void;
		}
		[CategoryAttribute("Scale"),
			DescriptionAttribute("�������� Y���")]
		property float Y
		{
			auto get()->float;
			auto set(float value)->void;
		}
		[CategoryAttribute("Scale"),
			DescriptionAttribute("�������� Z���")]
		property float Z
		{
			auto get()->float;
			auto set(float value)->void;
		}
		virtual auto ToString()->System::String^ override
		{
			return String::Format("({0}, {1}, {2})", X, Y, Z);
		}
	private:
		void OnPropertyChanged(String^ info)
		{
			PropertyChanged(this, gcnew PropertyChangedEventArgs(info));
		}
		float x;
		float y;
		float z;
	};
	[TypeConverter(ExpandableObjectConverter::typeid)]
	public ref struct Rotation : public INotifyPropertyChanged, public ExpandableObjectConverter
	{
		virtual event PropertyChangedEventHandler^ PropertyChanged;
	public:
		Rotation();
		Rotation(float x, float y, float z);
		Rotation(Rotation^  rhs);

		[CategoryAttribute("Position"),
			DescriptionAttribute("X �� ȸ����")]
		property float X
		{
			auto get()->float;
			auto set(float x)->void;
		}
		[CategoryAttribute("Position"),
			DescriptionAttribute("X �� ȸ����")]
		property float Y
		{
			auto get()->float;
			auto set(float x)->void;
		}
		[CategoryAttribute("Position"),
			DescriptionAttribute("X �� ȸ����")]
		property float Z
		{
			auto get()->float;
			auto set(float x)->void;
		}
		virtual auto ToString()->System::String^ override
		{
			return String::Format("({0}, {1}, {2})", X, Y, Z);
		}
	private:
		void OnPropertyChanged(String^ info)
		{
			PropertyChanged(this, gcnew PropertyChangedEventArgs(info));
		}
		float x;
		float y;
		float z;
	};
	[TypeConverter(ExpandableObjectConverter::typeid)]
	public ref struct Transform : public INotifyPropertyChanged, public ExpandableObjectConverter
	{
		virtual event PropertyChangedEventHandler^ PropertyChanged;
	public:
		Transform();
		Transform(Transform^  rhs);
		[CategoryAttribute("Transform"), Browsable(true)]
		property MapToolRender::Rotation^ Rotation
		{
			auto get()->MapToolRender::Rotation^;
			auto set(MapToolRender::Rotation^ value)->void;
		}
		[CategoryAttribute("Transform"), Browsable(true)]
		property MapToolRender::Position^ Position
		{
			auto get()->MapToolRender::Position^;
			auto set(MapToolRender::Position^ value)->void;
		}
		[CategoryAttribute("Transform"), Browsable(true)]
		property MapToolRender::Scale^ Scale
		{
			auto get()->MapToolRender::Scale^;
			auto set(MapToolRender::Scale^ value)->void;
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
		MapToolRender::Rotation^ rotation;
		MapToolRender::Scale^ scale;
		MapToolRender::Position^ position;
	};
}
