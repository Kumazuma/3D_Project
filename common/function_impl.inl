#include"function.hpp"
template<typename T>
inline bool IsNULL(T* const ptr)
{
	return ptr == nullptr;
}
template<typename T>
inline bool IsNotNULL(T* const ptr)
{
	return ptr != nullptr;
}