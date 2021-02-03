#include "Vector.hpp"

DirectXMathWrapper::Vector::Vector(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}
auto DirectXMathWrapper::Vector::X::get()->float
{
	return x;
}
auto DirectXMathWrapper::Vector::X::set(float value)->void
{
	x = value;
}
auto DirectXMathWrapper::Vector::Y::get()->float
{
	return y;
}
auto DirectXMathWrapper::Vector::Y::set(float value)->void
{
	y = value;
}
auto DirectXMathWrapper::Vector::Z::get()->float
{
	return z;
}
auto DirectXMathWrapper::Vector::Z::set(float value)->void
{
	z = value;
}
auto DirectXMathWrapper::Vector::W::get()->float
{
	return w;
}
auto DirectXMathWrapper::Vector::W::set(float value)->void
{
	w = value;
}