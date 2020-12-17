#pragma once
class Ray;
namespace DirectX
{
	struct XMFLOAT3;
}
namespace MapToolRender
{
	ref class GraphicsDevice;
	public ref class Ray
	{
	public:
		Ray(MapToolCore::Position rayStart, MapToolCore::Position rayAt);
		Ray(DirectX::XMFLOAT3* rayStart, DirectX::XMFLOAT3* rayAt);
		~Ray();
		!Ray();
		auto GetPosition(float t)->MapToolCore::Position;
	public:
		property ::Ray* Handle {auto get()->::Ray* { return m_pNative; }}
	private:
		::Ray* m_pNative;
	};
}