#pragma once
#include "IRenderable.hpp"
namespace DirectX
{
	struct XMFLOAT4X4;
}
class SkyBoxObject;
namespace MaptoolRenderer {
	using namespace System::ComponentModel;
	using namespace System::Collections::Generic;
	using namespace System;
	ref class CubeTexture;
	public ref class SkyBox :
		public IRenderable,
		public MapToolCore::IHasText,
		public IRenderEntity,
		public INotifyPropertyChanged
	{
	public:
		SkyBox();
		~SkyBox();
		!SkyBox();
	public:
		virtual event System::ComponentModel::PropertyChangedEventHandler^ PropertyChanged;
		virtual auto PrepereRender(GraphicsDevice^ renderer)->void override;
		// IRenderEntity��(��) ���� ��ӵ�
		virtual auto Render(GraphicsDevice^ renderer) -> void;
		property CubeTexture^ Texture
		{
			auto get()->CubeTexture^;
			auto set(CubeTexture^ )->void;
		}
		// IHasText��(��) ���� ��ӵ�
		[BrowsableAttribute(false)]
		virtual property System::String^ Text
		{
			System::String^ get();
		}
	internal:
	private:
		CubeTexture^ texture_;
		SkyBoxObject* mesh_;
		// INotifyPropertyChanged��(��) ���� ��ӵ�




	};
}