#pragma once
#include"Transform.h"
#include"Camera.h"
#include"TextureConverter.h"
struct IDirect3D9Ex;
struct IDirect3DDevice9Ex;
struct RenderObject;
struct IRenderer;
class RenderModule;
namespace std
{
	class ofstream;
	class streambuf;
}
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
	ref class StaticXMeshObj;
	ref class Ray;
	ref class SwapChain;

	public ref class GraphicsDevice sealed
	{
	public:
		static auto Initialize(Control^ renderView, unsigned width, unsigned height)->void;
		static auto GetOpenFilePath(System::Windows::Forms::Control^ owner, System::String^ filter)->System::String^;
		static auto GetSaveFilePath(System::Windows::Forms::Control^ owner, System::String^ filter)->System::String^;

		static property GraphicsDevice^ Instance
		{
			GraphicsDevice^  get()
			{
				return s_instance;
			}

		}
		auto Render(System::Windows::Forms::Control^, IEnumerable<RenderObject^>^ objs, Camera^ camera)->void;
		auto CreateMouseRay(Control^ drawPanel, Camera^ camera, System::Drawing::Point^ mousePt)->Ray^;
		property RenderModule* Handle
		{
			auto get()->RenderModule* { return m_pRenderModule; }
		}
	private:
		auto ApplyViewProjMatrix()->void;
		!GraphicsDevice();
		~GraphicsDevice() {}
		static GraphicsDevice^ s_instance;
		GraphicsDevice(Control^ renderView, unsigned width, unsigned height);
	private:
		std::ofstream* m_logFileStreamPtr;
		std::streambuf* m_streamBufPtr;
		RenderModule* m_pRenderModule;
		IRenderer* m_pRenderer;
	};
	public ref class MapObject abstract : public INotifyPropertyChanged
	{
	protected:
		MapObject() {}
		MapObject(MapObject^ rhs);
	public:
		virtual event PropertyChangedEventHandler^ PropertyChanged;
		[CategoryAttribute("속성")]
		property System::String^ Name
		{
			auto get()->System::String^;
			auto set(System::String^ value)->void;
		}
		virtual auto Update(int ms)->void {}
		virtual auto ToString()->System::String^ override;
	protected:
		auto BroadcastPropertyChanged(System::String^ propertyName)->void;
	protected:
		System::String^ m_name;
	};
	public ref class RenderObject abstract: public MapObject
	{
	public:
		[CategoryAttribute("속성")]
		property MapToolCore::Transform^ Transform
		{
			auto get()->MapToolCore::Transform^;
			auto set(MapToolCore::Transform^ value)->void;
		}
		[Browsable(false)]
		property ::RenderObject* Handle
		{
			auto get()->::RenderObject*;
		}
		property bool IsRayPick
		{
			auto get()->bool;
		}
		virtual auto Clone()->RenderObject^ = 0;
		auto RayPick(Ray^ ray)->System::Nullable<float>;
	protected:
		RenderObject();
		RenderObject(RenderObject^  obj);
		!RenderObject();
		~RenderObject();
		virtual auto UpdateTransform()->void;
		auto OnTransformChanged(Object^ obj, PropertyChangedEventArgs^ e)->void;
		auto OnPropertyChanged(String^ info)->void
		{
			PropertyChanged(this, gcnew PropertyChangedEventArgs(info));
		}
	protected:
		PropertyChangedEventHandler^ m_transformChangedHandler;
		MapToolCore::Transform^ m_transform;
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