#pragma once
#include "IRenderable.hpp"

namespace MaptoolRenderer
{
	public ref class MeshObject abstract :
		public IRenderable,
		public MapToolCore::IHasTransform,
		public System::ComponentModel::INotifyPropertyChanged
	{
	protected:
		MeshObject();
		~MeshObject();
		!MeshObject();
	public:
		// IRenderable을(를) 통해 상속됨
		virtual auto PrepereRender(GraphicsDevice^ renderer) -> void = 0;
		// IHasTransform을(를) 통해 상속됨
		property MapToolCore::Transform^ Transform {
			virtual auto get()->MapToolCore::Transform^ override;
			virtual auto set(MapToolCore::Transform^)->void override;
		}
		// INotifyPropertyChanged을(를) 통해 상속됨
		virtual event System::ComponentModel::PropertyChangedEventHandler^ PropertyChanged;
	internal:
		auto OnTransformChanged(Object^ obj, System::ComponentModel::PropertyChangedEventArgs^ e) -> void;
	internal:
		MapToolCore::Transform^ transform_;
		DirectX::XMFLOAT4X4* transformPtr_;
		System::ComponentModel::PropertyChangedEventHandler^ transformChangedHandler_;
	};
}