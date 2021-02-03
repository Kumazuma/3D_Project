#include "pch.h"
#include "OBJMesh.hpp"
#include "MaptoolRenderer.h"
#include <RenderModule/WavefrontOBJMesh.h>
#include "msclr\marshal_cppstd.h"
using namespace msclr::interop;

MaptoolRenderer::OBJMesh::OBJMesh(System::String^ path)
{
	marshal_context ctx;
	std::wstring szFilePath{ ctx.marshal_as<std::wstring>(path) };
	auto instance{ GraphicsDevice::Instance };
	WavefrontOBJMesh* mesh{};
	WavefrontOBJMesh::Create(instance->RenderModuleHandle, szFilePath, &mesh);
	renderObject_ = mesh;
}

MaptoolRenderer::OBJMesh::OBJMesh(OBJMesh^ rhs)
{
	this->renderObject_ = rhs->renderObject_->Clone();
}

auto MaptoolRenderer::OBJMesh::Clone() -> Mesh^ 
{
	return gcnew OBJMesh{ this };
}
