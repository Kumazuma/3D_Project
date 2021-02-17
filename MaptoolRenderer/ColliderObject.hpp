#pragma once
#include "IRenderable.hpp"
#include "RenderObject.hpp"
namespace MapToolCore
{
	ref class Collider;
}

namespace MaptoolRenderer
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace MapToolCore;
	ref class Mesh;
	ref class SkinnedXMeshObject;
	public ref class ColliderObject :
		public IRenderable,
		public System::ComponentModel::INotifyPropertyChanged,
		public IRenderEntity,
		public IHasName
	{
	public:
		ColliderObject();
		property SkinnedXMeshObject^ ParentObject
		{
			auto get()->SkinnedXMeshObject^;
			auto set(SkinnedXMeshObject^ object)->void;
		}
		property String^ Name
		{
			virtual auto get()->String^;
			virtual auto set(String^ value)-> void;
		}
		[CategoryAttribute("Common")]
		property ColliderType Type
		{
			auto get()->ColliderType;
			auto set(ColliderType)->void;
		}
		[Browsable(false)]
		property cli::array<String^>^ FrameNames
		{
			auto get()->cli::array<String^>^;
		}
		[TypeConverter(FormatStringConverter::typeid)]
		property String^ FrameName
		{
			auto get()->String^;
			auto set(String^)->void;
		}
		property ColliderAttribute^ Attribute
		{
			auto get()->ColliderAttribute^;
		}
		// INotifyPropertyChanged을(를) 통해 상속됨
		virtual event System::ComponentModel::PropertyChangedEventHandler^ PropertyChanged;
	public:
		// IRenderable을(를) 통해 상속됨
		virtual auto PrepereRender(GraphicsDevice^ renderer) -> void override;
		// IRenderEntity을(를) 통해 상속됨
		virtual auto Render(GraphicsDevice^ renderer) -> void;
	public:

	internal:
		String^ name_;
		ColliderType type_ = ColliderType::None;
		ColliderAttribute^ attribute_;
		Offset offset_;
		Mesh^ mesh_;
		SkinnedXMeshObject^ parent_;
		String^ frameName_;
		void OnPropertyChanged(System::Object^ sender, System::ComponentModel::PropertyChangedEventArgs^ e);
	};
}