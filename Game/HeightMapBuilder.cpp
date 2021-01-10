#include "HeightMapBuilder.hpp"
using namespace DirectX;
Kumazuma::Client::HeightMapBuilder::HeightMapBuilder(HeightMapBuilder&& rhs) noexcept:
	m_meshs{std::move(rhs.m_meshs)}
{
}
auto Kumazuma::Client::HeightMapBuilder::operator<<(WavefrontOBJMesh const& rhs) & -> HeightMapBuilder&
{
	m_meshs.emplace_back(static_cast<WavefrontOBJMesh*>(rhs.Clone()));
	return *this;
}

auto Kumazuma::Client::HeightMapBuilder::Build() const -> std::unique_ptr<HeightMap>
{
	std::vector<std::unique_ptr<HeightMap> > maps;
	for (auto const& it : m_meshs)
	{
		auto const& subsets{ it->GetSubsets() };
		auto const vertices{ it->GetVertices() };
		auto newVertices{ std::make_shared < std::vector<XMFLOAT3A> >() };
		auto mWorldTransform{ XMLoadFloat4x4(&it->GetTransform()) };
		newVertices->assign(vertices->size(), {});
		XMVector3TransformCoordStream(
			newVertices->data(),
			sizeof(XMFLOAT3A),
			vertices->data(),
			sizeof(XMFLOAT3A),
			vertices->size(),
			mWorldTransform);
		std::vector<std::unique_ptr<HeightMap> > subsetHeightMaps;

		for (auto& subset : subsets)
		{
			std::vector<XMUINT3> triangles;
			triangles.reserve(subset.second->GetTriangles().size());
			for (auto& triagle : subset.second->GetTriangles())
			{
				triangles.emplace_back(XMUINT3{ triagle[0],triagle[1], triagle[2] });
			}
			std::unique_ptr<HeightMap> submesh{ new HeightMapSubMesh(std::move(triangles), newVertices) };
			subsetHeightMaps.emplace_back(std::move(submesh));
		}
		maps.emplace_back(std::unique_ptr<HeightMap>{new HeightMapContainer{ std::move(subsetHeightMaps) }});
	}
	return std::unique_ptr<HeightMap>{new HeightMapContainer{ std::move(maps) }};
}

auto Kumazuma::Client::HeightMapBuilder::operator<<(WavefrontOBJMesh const& rhs) && -> HeightMapBuilder
{
	*this << rhs;
	return std::move(*this);
}
