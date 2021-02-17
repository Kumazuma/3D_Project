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
	ref class Mesh;
	ref class SkinnedXMeshObject;
	public ref class ColliderObject :
		public IRenderable,
		public System::ComponentModel::INotifyPropertyChanged,
		public IRenderEntity
	{
	public:
		ColliderObject();
		property SkinnedXMeshObject^ ParentObject
		{
			auto get()->SkinnedXMeshObject^;
			auto set(SkinnedXMeshObject^ object)->void;
		}
		property cli::array<String^>^ FrameNames
		{
			auto get()->cli::array<String^>^;
		}
		[TypeConverter(MapToolCore::FormatStringConverter::typeid)]
		property String^ FrameName
		{
			auto get()->String^;
			auto set(String^)->void;
		}
		property MapToolCore::Collider^ Collider
		{
			auto get()->MapToolCore::Collider^;
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
		MapToolCore::Collider^ collider_;
		Mesh^ mesh_;
		SkinnedXMeshObject^ parent_;
		String^ frameName_;
		void OnPropertyChanged(System::Object^ sender, System::ComponentModel::PropertyChangedEventArgs^ e);
	};
}