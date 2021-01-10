#pragma once
#include<DirectXMath.h>
#include"BoundingSphere.hpp"
#include<memory>
#include<vector>
#include<optional>
#include<array>
#include"WavefrontOBJMesh.h"
namespace Kumazuma::Client
{
	class HeightMapBuilder;
	class HeightMap
	{
	public:
		struct Triangle
		{
			DirectX::XMFLOAT3A vertices[3];
			DirectX::XMFLOAT3A normalVector;
		};
		using Builder = HeightMapBuilder;
	public:
		virtual ~HeightMap() = default;
		//auto __vectorcall RayPicking(DirectX::XMVECTOR rayAt, DirectX::XMVECTOR rayDirection, f32* pOut)->bool;
		virtual auto RayPicking(DirectX::XMVECTOR rayAt, DirectX::XMVECTOR rayDir)const->std::optional<f32> = 0;
		virtual auto GetTriangle(DirectX::XMVECTOR position, DirectX::XMVECTOR up) const->std::optional<Triangle> = 0;
		virtual auto IsInBoundSheres(DirectX::XMVECTOR position) const->bool = 0;
		virtual auto GetHeight(DirectX::XMVECTOR position) const->std::optional<f32> = 0;
		virtual auto GetBoundingSphere() const->BoundSphere = 0;
	};

	class HeightMapContainer : public HeightMap
	{
	public:
		HeightMapContainer(std::vector<std::unique_ptr<HeightMap > >&& children)noexcept;
		auto RayPicking(DirectX::XMVECTOR rayAt, DirectX::XMVECTOR rayDir)const->std::optional<f32> override;
		auto GetTriangle(DirectX::XMVECTOR position, DirectX::XMVECTOR up) const->std::optional<Triangle> override;
		auto IsInBoundSheres(DirectX::XMVECTOR position) const->bool override;
		auto GetHeight(DirectX::XMVECTOR position) const->std::optional<f32> override;
		auto GetBoundingSphere() const->BoundSphere override;

	private:
		BoundSphere m_boundingSphere;
		std::vector<std::unique_ptr<HeightMap > > m_children;
	};
	class HeightMapSubMesh : public HeightMap
	{
	public:
		HeightMapSubMesh(std::vector<DirectX::XMUINT3>&& triangles, std::shared_ptr< std::vector<DirectX::XMFLOAT3A> > vertices) noexcept;
		HeightMapSubMesh(HeightMapSubMesh&& rhs)noexcept;

		auto RayPicking(DirectX::XMVECTOR rayAt, DirectX::XMVECTOR rayDir)const->std::optional<f32> override;
		auto GetTriangle(DirectX::XMVECTOR position, DirectX::XMVECTOR up) const->std::optional<Triangle> override;
		auto GetHeight(DirectX::XMVECTOR position) const->std::optional<f32> override;
		auto IsInBoundSheres(DirectX::XMVECTOR position) const->bool override;
		auto GetBoundingSphere() const->BoundSphere override;

	private:
		BoundSphere m_boundingSphere;
		std::shared_ptr< std::vector<DirectX::XMFLOAT3A> > m_vertices;
		std::vector<DirectX::XMUINT3> m_triangles;
		std::vector<std::array<DirectX::XMFLOAT3A, 3> > m_sideNormls;
		std::vector<DirectX::XMFLOAT4> m_plains;
	};

}
#include"HeightMapBuilder.hpp"