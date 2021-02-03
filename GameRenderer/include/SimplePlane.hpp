#pragma once
#include<d3d9.h>
#include<memory>
#include<Common.h>
namespace Kumazuma::Graphics
{
	class DLL_CLASS SimplePlane
	{
	public:
		virtual ~SimplePlane() = default;
		static auto Create(IDirect3DDevice9* device)->std::unique_ptr<SimplePlane>;
		virtual auto Render(IDirect3DDevice9* device)->void = 0;

	};
}