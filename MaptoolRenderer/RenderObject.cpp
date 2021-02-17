#include "pch.h"
#include "MaptoolRenderer.h"
#include "RenderObject.hpp"
#include "SkinnedXMesh.hpp"
#include "XSkinnedMeshObject.hpp"
#include "OBJMesh.hpp"
#include "OBJObject.hpp"
MaptoolRenderer::MeshObject::MeshObject()
{
	transformChangedHandler_ = gcnew System::ComponentModel::PropertyChangedEventHandler(this, &MeshObject::OnTransformChanged);
	transform_ = gcnew MapToolCore::Transform{};
	transform_->PropertyChanged += transformChangedHandler_;
	transformPtr_ = new DirectX::XMFLOAT4X4{};
	DirectX::XMFLOAT3 position{ transform_->Position.X, transform_->Position.Y, transform_->Position.Z };
	DirectX::XMFLOAT3 rotation{ transform_->Rotation.X, transform_->Rotation.Y, transform_->Rotation.Z };
	GenerateTransform(&position, &rotation, transform_->Scale, transformPtr_);
	name_ = "";
}

MaptoolRenderer::MeshObject::~MeshObject()
{
	if (transformPtr_ != nullptr)
	{
		delete transformPtr_;
		transformPtr_ = nullptr;
	}
}

MaptoolRenderer::MeshObject::!MeshObject()
{
	this->~MeshObject();
}
auto MaptoolRenderer::MeshObject::Create(Mesh^ mesh) -> MeshObject^
{
	if (mesh == nullptr)
		return nullptr;
	if (auto objMesh{ dynamic_cast<SkinnedXMesh^>(mesh) }; objMesh != nullptr)
	{
		return gcnew SkinnedXMeshObject(objMesh);
	}
	if (auto objMesh{ dynamic_cast<OBJMesh^>(mesh) }; objMesh != nullptr)
	{
		return gcnew OBJObject(objMesh);
	}
	return nullptr;
}
auto MaptoolRenderer::MeshObject::Transform::get()->MapToolCore::Transform^
{
    return transform_;
}
auto MaptoolRenderer::MeshObject::Transform::set(MapToolCore::Transform^ value)->void
{
    transform_->PropertyChanged -= transformChangedHandler_;
    transform_ = value;
    transform_->PropertyChanged += transformChangedHandler_;
    OnTransformChanged(transform_, nullptr);
}
auto MaptoolRenderer::MeshObject::Name::get()->String^
{
	return name_;
}
auto MaptoolRenderer::MeshObject::Name::set(String^ value)->void
{
	name_ = value;
	this->PropertyChanged(this, gcnew System::ComponentModel::PropertyChangedEventArgs{ "Name" });
}
auto MaptoolRenderer::MeshObject::OnTransformChanged(Object^ obj, System::ComponentModel::PropertyChangedEventArgs^ e) -> void
{
	DirectX::XMFLOAT3 position{ transform_->Position.X, transform_->Position.Y, transform_->Position.Z };
	DirectX::XMFLOAT3 rotation{ transform_->Rotation.X, transform_->Rotation.Y, transform_->Rotation.Z };
	GenerateTransform(&position, &rotation, transform_->Scale, transformPtr_);
	this->PropertyChanged(this, gcnew System::ComponentModel::PropertyChangedEventArgs{ "Transform" });
}
