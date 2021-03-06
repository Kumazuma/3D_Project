#include"pch.h"
#include "Camera.h"
#pragma unmanaged
#pragma warning(push)
#pragma warning(disable: 4793)
void MoveForward(DirectX::XMFLOAT3* pPosition, DirectX::XMFLOAT3* pRotation,  float delta)
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
#pragma warning(pop)
#pragma managed
MapToolRender::Camera::Camera()
{
	m_pPosition = new DirectX::XMFLOAT3{ 0.f, 0.f, 0.f };
	m_pRotation = new DirectX::XMFLOAT3{ 0.f, 0.f, 0.f };
	m_position = MapToolCore::Position(0.f, 0.f, 0.f);
	m_rotation = MapToolCore::Rotation(0.f, 0.f, 0.f);
}

MapToolRender::Camera::~Camera()
{
	if (m_pPosition != nullptr)
	{
		delete m_pPosition;
	}
	if (m_pRotation != nullptr)
	{
		delete m_pRotation;
	}
}

MapToolRender::Camera::!Camera()
{
	this->~Camera();
}

auto MapToolRender::Camera::MoveUp(float delta) -> void
{
	::MoveUp(PositionPtr, RotationPtr, delta);
	m_position.X = m_pPosition->x;
	m_position.Y = m_pPosition->y;
	m_position.Z = m_pPosition->z;
}

auto MapToolRender::Camera::MoveForward(float delta) -> void
{
	::MoveForward(PositionPtr, RotationPtr, delta);
	m_position.X = m_pPosition->x;
	m_position.Y = m_pPosition->y;
	m_position.Z = m_pPosition->z;
}

auto MapToolRender::Camera::MoveRight(float delta) -> void
{
	::MoveRight(PositionPtr, RotationPtr, delta);
	m_position.X = m_pPosition->x;
	m_position.Y = m_pPosition->y;
	m_position.Z = m_pPosition->z;

}
auto MapToolRender::Camera::RotationX(float delta) -> void
{
	m_rotation.X += delta;
}
auto MapToolRender::Camera::RotationY(float delta) -> void
{
	m_rotation.Y += delta;
}
auto MapToolRender::Camera::Position::get()->MapToolCore::Position
{
	return m_position;
}
auto MapToolRender::Camera::Position::set(MapToolCore::Position value)->void
{
	m_position = value;
	m_pPosition->x = value.X;
	m_pPosition->y = value.Y;
	m_pPosition->z = value.Z;
}
auto MapToolRender::Camera::Rotation::get()->MapToolCore::Rotation
{
	return m_rotation;
}
auto MapToolRender::Camera::Rotation::set(MapToolCore::Rotation value)->void
{
	m_rotation = value;
	m_pRotation->x = value.X;
	m_pRotation->y = value.Y;
	m_pRotation->z = value.Z;
}
auto MapToolRender::Camera::RotationPtr::get()->DirectX::XMFLOAT3*
{
	m_pRotation->x = m_rotation.X;
	m_pRotation->y = m_rotation.Y;
	m_pRotation->z = m_rotation.Z;
	return m_pRotation;
}
auto MapToolRender::Camera::PositionPtr::get()->DirectX::XMFLOAT3*
{
	m_pPosition->x = m_position.X;
	m_pPosition->y = m_position.Y;
	m_pPosition->z = m_position.Z;
	return m_pPosition;
}