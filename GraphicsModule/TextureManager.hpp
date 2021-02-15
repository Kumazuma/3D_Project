#pragma once
#include "common.hpp"
#include "typedef.hpp"
#include "Texture2D.hpp"
namespace Kumazuma
{
	//내부적으로 텍스처들을 관리하기 위한 클래스
	class DLL_CLASS TextureManager
	{
	public:
		virtual  ~TextureManager();
		virtual Texture2D* Load(wchar_t const* path) = 0;
	private:
	};
}