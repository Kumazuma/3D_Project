#pragma once
#include<game/component.hpp>
#include<DirectXMath.h>
#include<optional>
#include"typedef.hpp"
#include "BoundingSphere.hpp"
#include <memory>
#include <vector>
namespace Kumazuma::Client
{
	class HeightMap;
	class COMHeightMap : public Game::Component
	{
	public:
		static const Game::ComponentTag<COMHeightMap> TAG;
		COMHeightMap(std::shared_ptr<HeightMap> heightMap);
		auto Clone()const->Game::Component* override;
		auto GetHeightMap()const->std::shared_ptr<HeightMap>;
	private:
		std::shared_ptr<HeightMap> m_heightmap;
	};
	
}