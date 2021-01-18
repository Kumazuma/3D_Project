#pragma once
#include<d3d11.h>
class Resource
{
public:
	enum class Kind{None, Mesh, Texture};
	virtual ~Resource() = default;
	virtual auto GetKind()const->Kind = 0;
};
class TextureResource : public Resource
{

};
class SubMesh
{
public:

private:

};
class SubMeshCollection
{
public:
	virtual ~SubMeshCollection() = default;
	virtual auto GetCount()->int = 0;
	
};

class MeshResource : public Resource
{
public:
	virtual auto GetSubMesh(SubMeshCollection** collection)->HRESULT = 0;

};
class SkinnedXMesh : public MeshResource
{

};
class WavefrontOBJMesh : public MeshResource
{

};
class MeshObject
{
public:

private:

};
class EffectObject
{
public:

private:

};