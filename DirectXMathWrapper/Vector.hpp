#pragma once
namespace DirectXMathWrapper
{
	public value class Vector
	{
		float x;
		float y;
		float z;
		float w;
		Vector(float x, float y, float z, float w);
		property float X
		{
			auto get()->float;
			auto set(float val)->void;
		}
		property float Y
		{
			auto get()->float;
			auto set(float val)->void;
		}
		property float Z
		{
			auto get()->float;
			auto set(float val)->void;
		}
		property float W
		{
			auto get()->float;
			auto set(float val)->void;
		}
	internal:
		
	};
}