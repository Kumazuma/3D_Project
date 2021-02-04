#include "pch.h"
#include <RenderModule/WavefrontOBJMesh.h>
#include "OBJMesh.hpp"
#include "MaptoolRenderer.h"
#include "msclr\marshal_cppstd.h"
using namespace msclr::interop;
namespace MaptoolRenderer
{
	OBJMesh::OBJMesh(System::String^ path)
	{
		marshal_context ctx;
		std::wstring szFilePath{ ctx.marshal_as<std::wstring>(path) };
		auto instance{ GraphicsDevice::Instance };
		WavefrontOBJMesh* mesh{};
		WavefrontOBJMesh::Create(instance->RenderModuleHandle, szFilePath, &mesh);
		renderObject_ = mesh;
		auto const& subsets{ mesh->GetSubsets() };
		subsetNames_ = gcnew array<String^>(subsets.size());
		int i{ 0 };
		for (auto& it : subsets)
		{
			subsetNames_[i] = ctx.marshal_as<String^>(it.first);
			++i;
		}
	}
	OBJMesh::OBJMesh(OBJMesh^ rhs)
	{
		this->renderObject_ = rhs->renderObject_->Clone();
	}

	auto OBJMesh::Clone() -> Mesh^
	{
		return gcnew OBJMesh{ this };
	}

	auto OBJMesh::SubsetNames::get()->array<System::String^>^ 
	{
		return safe_cast<array<System::String^>^>(subsetNames_->Clone());
	}
}
