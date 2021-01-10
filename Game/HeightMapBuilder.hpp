#pragma once
#include"HeightMap.hpp"

namespace Kumazuma::Client
{
	class HeightMapBuilder
	{
	public:
		HeightMapBuilder() = default;
		HeightMapBuilder(HeightMapBuilder&& rhs)noexcept;
		
		auto operator << (WavefrontOBJMesh const& rhs)&&->HeightMapBuilder;
		auto operator << (WavefrontOBJMesh const& rhs)& ->HeightMapBuilder&;

		auto Build()const->std::unique_ptr<HeightMap>;
	private:
		std::vector<std::unique_ptr<WavefrontOBJMesh> > m_meshs;
	};
}