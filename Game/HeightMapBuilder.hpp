#pragma once
#include"HeightMap.hpp"

namespace Kumazuma::Client
{
	class HeightMapBuilder
	{
	public:
		HeightMapBuilder() = default;
		HeightMapBuilder(HeightMapBuilder&& rhs)noexcept;
		
		auto operator << (WowMapMeshObject const& rhs)&&->HeightMapBuilder;
		auto operator << (WowMapMeshObject const& rhs)& ->HeightMapBuilder&;

		auto Build()const->std::unique_ptr<HeightMap>;
	private:
		std::vector<std::unique_ptr<WowMapMeshObject> > m_meshs;
	};
}