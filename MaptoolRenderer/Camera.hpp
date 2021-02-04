#pragma once
namespace DirectX
{
	struct XMFLOAT4X4;
}
namespace MaptoolRenderer {
	public ref class Camera abstract
	{
	public:
		Camera();
		auto MoveUp(float delta)->void;
		auto MoveForward(float delta)->void;
		auto MoveRight(float delta)->void;
		auto RotationX(float delta)->void;
		auto RotationY(float delta)->void;
		property MapToolCore::Position Position
		{
			auto get()->MapToolCore::Position;
			auto set(MapToolCore::Position value)->void;
		}
		property MapToolCore::Rotation Rotation
		{
			auto get()->MapToolCore::Rotation;
			auto set(MapToolCore::Rotation value)->void;
		}
		property float Far
		{
			auto get()->float;
			auto set(float	)->void;
		}
		property float Near
		{
			auto get()->float;
			auto set(float)->void;
		}
	internal:
		virtual auto GenerateProjMatrix(float width, float height, DirectX::XMFLOAT4X4* out)->void = 0;
		auto GenerateMatrix(DirectX::XMFLOAT4X4* out)->void;
	private:
		MapToolCore::Position m_position;
		MapToolCore::Rotation m_rotation;
	protected:
		float near_;
		float far_;
	};
	public ref class OrthoCamera : public Camera
	{
	public:
		property float Aspect
		{
			auto get()->float;
			auto set(float)->void;
		}
	internal:
		auto GenerateProjMatrix(float width, float height, DirectX::XMFLOAT4X4* out)->void override;
	private:
		float aspect_;
	};
	public ref class PersCamera : public Camera
	{
	public:
		property float Angle
		{
			auto get()->float;
			auto set(float)->void;
		}
	internal:
		auto GenerateProjMatrix(float width, float height, DirectX::XMFLOAT4X4* out)->void override;
	protected:
		float angle_;
	};

}