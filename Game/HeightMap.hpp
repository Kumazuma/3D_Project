#pragma once
#include<DirectXMath.h>
#include"BoundingSphere.hpp"
#include<memory>
#include<vector>
#include<optional>
#include"WowMapMeshObject.h"
namespace Kumazuma::Client
{
	class HeightMapBuilder;
	class HeightMap
	{
	public:
		using Builder = HeightMapBuilder;
	public:
		virtual ~HeightMap() = default;
		//auto __vectorcall RayPicking(DirectX::XMVECTOR rayAt, DirectX::XMVECTOR rayDirection, f32* pOut)->bool;
		virtual auto __vectorcall RayPicking(DirectX::XMVECTOR rayAt, DirectX::XMVECTOR rayDir)const->std::optional<f32> = 0;
		virtual auto __vectorcall IsOnTriangles(DirectX::XMVECTOR position, DirectX::XMVECTOR up)const->bool = 0;
		virtual auto GetBoundingSphere() const->BoundSphere = 0;
		virtual auto __vectorcall IsInBoundSheres(DirectX::XMVECTOR position) const->bool = 0;
	};

	class HeightMapContainer : public HeightMap
	{
	public:
		HeightMapContainer(std::vector<std::unique_ptr<HeightMap > >&& children)noexcept;
		auto __vectorcall RayPicking(DirectX::XMVECTOR rayAt, DirectX::XMVECTOR rayDir)const->std::optional<f32> override;
		auto __vectorcall IsOnTriangles(DirectX::XMVECTOR position, DirectX::XMVECTOR up)const->bool override;
		auto GetBoundingSphere() const->BoundSphere override;
		auto __vectorcall IsInBoundSheres(DirectX::XMVECTOR position) const->bool override;

	private:
		BoundSphere m_boundingSphere;
		std::vector<std::unique_ptr<HeightMap > > m_children;
	};
	class HeightMapSubMesh : public HeightMap
	{
	public:
		HeightMapSubMesh(std::vector<DirectX::XMUINT3>&& triangles, std::shared_ptr< std::vector<DirectX::XMFLOAT3A> > vertices) noexcept;
		HeightMapSubMesh(HeightMapSubMesh&& rhs)noexcept;

		virtual auto __vectorcall RayPicking(DirectX::XMVECTOR rayAt, DirectX::XMVECTOR rayDir)const->std::optional<f32> override;
		virtual auto __vectorcall IsOnTriangles(DirectX::XMVECTOR position, DirectX::XMVECTOR up)const->bool override;
		virtual auto GetBoundingSphere() const->BoundSphere override;
		virtual auto __vectorcall IsInBoundSheres(DirectX::XMVECTOR position) const->bool override;

	private:
		BoundSphere m_boundingSphere;
		std::shared_ptr< std::vector<DirectX::XMFLOAT3A> > m_vertices;
		std::vector<DirectX::XMUINT3> m_triangles;
		std::vector<DirectX::XMFLOAT4X4> m_normalVectors;
	};

}
#include"HeightMapBuilder.hpp"