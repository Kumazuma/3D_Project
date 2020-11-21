#pragma once
#include"Transform.h"
#include"Camera.h"
#include"TextureConverter.h"
struct IDirect3D9Ex;
struct IDirect3DDevice9Ex;
class RenderObject;
class RenderModule;

namespace MapToolRender {
	public enum class RenderGroup
	{
		PRIORITY = 0,
		NONALPHA = 1,
		ALPHA = 2
	};
	using namespace System;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	ref class RenderObject;
	ref class Texture;
	ref struct Transform;
	public ref class GraphicsDevice sealed
	{
	public:
		static auto Initialize(Control^ renderView, unsigned width, unsigned height)->void;
		static property GraphicsDevice^ Instance
		{
			GraphicsDevice^  get()
			{
				return s_instance;
			}

		}
		auto Render()->void;
		auto ClearRenderGroup()->void;
		auto Add(RenderGroup groupId, RenderObject^ obj)->void;
		auto Remove(RenderGroup groupId, RenderObject^ obj)->void;
		
		property RenderModule* Handle
		{
			auto get()->RenderModule* { return m_pRenderModule; }
		}
		property Camera^ CurrentCamera
		{
			auto get()->Camera^ { return m_currentCamera; }
			auto set(Camera^ value)->void { m_currentCamera = value; Render(); }
		}
	private:
		auto ApplyViewProjMatrix()->void;
		!GraphicsDevice();
		~GraphicsDevice() {}
		static GraphicsDevice^ s_instance;
		GraphicsDevice(Control^ renderView, unsigned width, unsigned height);
	private:
		Camera^ m_currentCamera;
		Dictionary<RenderGroup, HashSet<RenderObject^>^ > m_renderObjects;
		RenderModule* m_pRenderModule;
	};
	public ref class RenderObject abstract : public INotifyPropertyChanged
	{
	public:
		virtual event PropertyChangedEventHandler^ PropertyChanged;
		[CategoryAttribute("속성")]
		property MapToolRender::Transform^ Transform
		{
			auto get()->MapToolRender::Transform^;
			auto set(MapToolRender::Transform^ value)->void;
		}
		[Browsable(false)]
		property ::RenderObject* Handle
		{
			auto get()->::RenderObject*;
		}
		virtual auto Clone()->RenderObject^ = 0;

	protected:
		RenderObject();
		RenderObject(RenderObject^  obj);
		!RenderObject();
		~RenderObject();
		auto OnTransformChanged(Object^ obj, PropertyChangedEventArgs^ e)->void;
		auto OnPropertyChanged(String^ info)->void
		{
			PropertyChanged(this, gcnew PropertyChangedEventArgs(info));
		}
	protected:
		PropertyChangedEventHandler^ m_transformChangedHandler;
		MapToolRender::Transform^ m_transform;
		::RenderObject* m_pNativeObject;
	};
	public ref class TerrainObject : public RenderObject
	{
	public:
		TerrainObject(GraphicsDevice^ device, System::String^ filePath, float interval, float maxHeight);
		auto Clone()->RenderObject^ override;
		property MapToolRender::Texture^ DiffuseTexture
		{
			auto get()->MapToolRender::Texture^;
			auto set(MapToolRender::Texture^ texture)->void;
		}
		property float Interval
		{
			auto get()->float;
			auto set(float value)->void;
		}
		property float MaxHeight
		{
			auto get()->float;
			auto set(float value)->void;
		}
	protected:
		TerrainObject(TerrainObject ^ const& rhs);
		Texture^ m_texture;
	};
}
namespace MapTool {
	namespace Render {
		using namespace MapToolRender;
	}
}