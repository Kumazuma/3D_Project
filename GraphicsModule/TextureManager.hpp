#pragma once
#include "common.hpp"
#include "typedef.hpp"
#include "Texture2D.hpp"
namespace Kumazuma
{
	//���������� �ؽ�ó���� �����ϱ� ���� Ŭ����
	class DLL_CLASS TextureManager
	{
	public:
		virtual  ~TextureManager();
		virtual Texture2D* Load(wchar_t const* path) = 0;
	private:
	};
}