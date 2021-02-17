#include "pch.h"
#include "Camera.hpp"
#pragma unmanaged
void GenerateViewSpaceMatrix(DirectX::XMFLOAT3* position, DirectX::XMFLOAT3* rotation, DirectX::XMFLOAT4X4* out)
{
	using namespace DirectX;

	XMVECTOR vPos{ XMLoadFloat3(position) };
	XMVECTOR vDir{ XMVectorSet(0.f,0.f,1.f,0.f) };
	XMVECTOR vRot{ XMLoadFloat3(rotation) };
	
	vRot = vRot * (XM_2PI / 360.f);
	XMMATRIX mRot{ XMMatrixRotationRollPitchYawFromVector(vRot) };
	XMMATRIX mViewSpace{ XMMatrixLookAtLH(vPos, mRot.r[2] + vPos, mRot.r[1]) };
	XMStoreFloat4x4(out, mViewSpace);
}
void GeneratePerspProjMatrix(float angle, float width, float height, float nearZ, float farZ, DirectX::XMFLOAT4X4* out)
{
	using namespace DirectX;
	XMMATRIX mProjSpace{ XMMatrixPerspectiveFovLH(angle * XM_2PI/360.f, width / height , nearZ, farZ) };
	XMStoreFloat4x4(out, mProjSpace);
}
void GenerateOrthoProjMatrix(float aspect, float width, float height, float nearZ, float farZ, DirectX::XMFLOAT4X4* out)
{
	using namespace DirectX;
	XMMATRIX mProjSpace{ XMMatrixOrthographicLH(width , height , nearZ, farZ) };
	XMStoreFloat4x4(out, mProjSpace);
}
#pragma managed
void MoveForward(DirectX::XMFLOAT3* pPosition, DirectX::XMFLOAT3* pRotation, float delta)
{
	using namespace DirectX;
	XMVECTOR vPos{ XMLoadFloat3(pPosition) };
	XMVECTOR vDir{ XMVectorSet(0.f,0.f,1.f,0.f) };
	XMVECTOR vRot{ XMLoadFloat3(pRotation) };
	vRot = vRot * (XM_2PI / 360.f);
	XMMATRIX mWorldMat{ XMMatrixRotationRollPitchYawFromVector(vRot) };
	vDir = XMVector3TransformNormal(vDir, mWorldMat);
	vPos += vDir * delta;
	XMStoreFloat3(pPosition, vPos);
}
void MoveRight(DirectX::XMFLOAT3* pPosition, DirectX::XMFLOAT3* pRotation, float delta)
{
	using namespace DirectX;
	XMVECTOR vPos{ XMLoadFloat3(pPosition) };
	XMVECTOR vDir{ XMVectorSet(1.f,0.f,0.f,0.f) };
	XMVECTOR vRot{ XMLoadFloat3(pRotation) };
	vRot = vRot * (XM_2PI / 360.f);
	XMMATRIX mWorldMat{ XMMatrixRotationRollPitchYawFromVector(vRot) };
	vDir = XMVector3TransformNormal(vDir, mWorldMat);
	vPos += vDir * delta;
	XMStoreFloat3(pPosition, vPos);
}
void MoveUp(DirectX::XMFLOAT3* pPosition, DirectX::XMFLOAT3* pRotation, float delta)
{
	using namespace DirectX;
	XMVECTOR vPos{ XMLoadFloat3(pPosition) };
	XMVECTOR vDir{ XMVectorSet(0.f,1.f,0.f,0.f) };
	XMVECTOR vRot{ XMLoadFloat3(pRotation) };
	vRot = vRot * (XM_2PI / 360.f);
	XMMATRIX mWorldMat{ XMMatrixRotationRollPitchYawFromVector(vRot) };
	vDir = XMVector3TransformNormal(vDir, mWorldMat);
	vPos += vDir * delta;
	XMStoreFloat3(pPosition, vPos);
}

namespace MaptoolRenderer
{
	Camera::Camera()
	{
		m_position = MapToolCore::Position(0.f, 0.f, 0.f);
		m_rotation = MapToolCore::Rotation(0.f, 0.f, 0.f);
		near_ = 0.1f;
		far_ = 1000.f;
	}

	auto Camera::MoveUp(float delta) -> void
	{
		DirectX::XMFLOAT3 position{ m_position.X, m_position.Y,m_position.Z, };
		DirectX::XMFLOAT3 rotation{ m_rotation.X, m_rotation.Y,m_rotation.Z, };
		::MoveUp(&position, &rotation, delta);
		m_position.X = position.x;
		m_position.Y = position.y;
		m_position.Z = position.z;
		PropertyChanged(this, gcnew PropertyChangedEventArgs("Position"));
	}

	auto Camera::MoveForward(float delta) -> void
	{
		DirectX::XMFLOAT3 position{ m_position.X, m_position.Y,m_position.Z, };
		DirectX::XMFLOAT3 rotation{ m_rotation.X, m_rotation.Y,m_rotation.Z, };
		::MoveForward(&position, &rotation, delta);
		m_position.X = position.x;
		m_position.Y = position.y;
		m_position.Z = position.z;
		PropertyChanged(this, gcnew PropertyChangedEventArgs("Position"));
	}

	auto Camera::MoveRight(float delta) -> void
	{
		DirectX::XMFLOAT3 position{ m_position.X, m_position.Y,m_position.Z, };
		DirectX::XMFLOAT3 rotation{ m_rotation.X, m_rotation.Y,m_rotation.Z, };
		::MoveRight(&position, &rotation, delta);
		m_position.X = position.x;
		m_position.Y = position.y;
		m_position.Z = position.z;
		PropertyChanged(this, gcnew PropertyChangedEventArgs("Position"));
	}
	auto Camera::RotationX(float delta) -> void
	{
		m_rotation.X += delta;
		PropertyChanged(this, gcnew PropertyChangedEventArgs("Rotation"));
	}
	auto Camera::RotationY(float delta) -> void
	{
		m_rotation.Y += delta;
		PropertyChanged(this, gcnew PropertyChangedEventArgs("Rotation"));
	}
	auto Camera::GenerateMatrix(DirectX::XMFLOAT4X4* out) -> void
	{
		DirectX::XMFLOAT3 position{ m_position.X, m_position.Y,m_position.Z, };
		DirectX::XMFLOAT3 rotation{ m_rotation.X, m_rotation.Y,m_rotation.Z, };
		GenerateViewSpaceMatrix(&position, &rotation, out);
	}
	auto Camera::Position::get()->MapToolCore::Position
	{
		return m_position;
	}
	auto Camera::Position::set(MapToolCore::Position value)->void
	{
		m_position = value;
		PropertyChanged(this, gcnew PropertyChangedEventArgs("Position"));
	}
	auto Camera::Rotation::get()->MapToolCore::Rotation
	{
		return m_rotation;
	}
	auto Camera::Rotation::set(MapToolCore::Rotation value)->void
	{
		m_rotation = value;
		PropertyChanged(this, gcnew PropertyChangedEventArgs("Rotation"));
	}
	auto Camera::Far::get()->float
	{
		return far_;
	}
	auto Camera::Far::set(float value)->void
	{
		far_ = value;
		PropertyChanged(this, gcnew PropertyChangedEventArgs("Far"));
	}
	auto Camera::Near::get()->float
	{
		return near_;
	}
	auto Camera::Near::set(float value)->void
	{
		near_ = value;
		PropertyChanged(this, gcnew PropertyChangedEventArgs("Near"));
	}
	auto OrthoCamera::Aspect::get()->float
	{
		return aspect_;
	}
	auto OrthoCamera::Aspect::set(float value)->void
	{
		aspect_ = value;
		PropertyChanged(this, gcnew PropertyChangedEventArgs("Aspect"));
	}
	auto PersCamera::Angle::get()->float
	{
		return angle_;
	}
	auto PersCamera::Angle::set(float value)->void
	{
		angle_ = value;
		PropertyChanged(this, gcnew PropertyChangedEventArgs("Angle"));
	}
	auto OrthoCamera::GenerateProjMatrix(float width, float height, DirectX::XMFLOAT4X4* out) -> void
	{
		GenerateOrthoProjMatrix(aspect_, width, height, near_, far_, out);

	}
	auto PersCamera::GenerateProjMatrix(float width, float height, DirectX::XMFLOAT4X4* out) -> void
	{
		GeneratePerspProjMatrix(angle_, width, height, near_, far_, out);
	}
}