#pragma once
#include "IRenderable.hpp"

namespace MaptoolRenderer
{
	using namespace System;
	using namespace System::ComponentModel;
	ref class Mesh;
	public ref class MeshObject abstract :
		public IRenderable,
		public MapToolCore::IHasText,
		public MapToolCore::IHasTransform,
		public System::ComponentModel::INotifyPropertyChanged,
		public MapToolCore::IHasName
	{
	protected:
		MeshObject();
		~MeshObject();
		!MeshObject();
	public:
		static auto Create(Mesh^ mesh)->MeshObject^;
		// IRenderable을(를) 통해 상속됨
		virtual auto PrepereRender(GraphicsDevice^ renderer) -> void = 0;
		// IHasTransform을(를) 통해 상속됨
		property MapToolCore::Transform^ Transform {
			virtual auto get()->MapToolCore::Transform^ ;
			virtual auto set(MapToolCore::Transform^)->void ;
		}
		property String^ Name
		{
			virtual auto get()->String^;
			virtual auto set(String^)->void;
		}
		[BrowsableAttribute(false)]
		property String^ Text
		{
			virtual auto get()->String^;
		}
		String^ ToString() override;
		// INotifyPropertyChanged을(를) 통해 상속됨
		virtual event System::ComponentModel::PropertyChangedEventHandler^ PropertyChanged;
	internal:
		auto OnTransformChanged(Object^ obj, System::ComponentModel::PropertyChangedEventArgs^ e) -> void;
	internal:
		String^ name_;
		MapToolCore::Transform^ transform_;
		System::ComponentModel::PropertyChangedEventHandler^ transformChangedHandler_;
		DirectX::XMFLOAT4X4* transformPtr_;
	};
}