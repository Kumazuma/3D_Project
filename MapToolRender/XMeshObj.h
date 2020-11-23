#pragma once
#include "MapToolRender.h"
#include "TextureCollection.h"
namespace MapToolRender
{
	public ref class StaticXMeshObj : public RenderObject
	{
	public:
		StaticXMeshObj(GraphicsDevice^ device, System::String^ filePath);
		auto Clone()->RenderObject^ override;
		property TextureCollection^ Textures
		{
			auto get()->TextureCollection^;
		}
	protected:
		StaticXMeshObj(StaticXMeshObj^ const& rhs);
		TextureCollection^ m_textures;
		void OnCollectionChanged(System::Object^ sender, System::Collections::Specialized::NotifyCollectionChangedEventArgs^ e);
	};
}