#pragma once
#include<Windows.h>
#include<math.h>
namespace Kumazuma
{
	namespace Game
	{
		namespace _2D
		{
			template<typename T>
			struct XYPair
			{
				XYPair();
				constexpr XYPair(T x, T y);
				XYPair operator + (const XYPair& other) const;
				XYPair operator - (const XYPair& other) const;
				XYPair operator + (float delta) const;
				XYPair operator - (float delta) const;
				XYPair& operator += (const XYPair& other);
				XYPair& operator -= (const XYPair& other);
				XYPair& operator += (float delta);
				XYPair& operator -= (float delta);
				T x;
				T y;
			};
			template<typename T>
			inline XYPair<T>::XYPair() :XYPair(T{}, T{}) {}
			template<typename T>
			inline constexpr XYPair<T>::XYPair(T x, T y) : x{ x }, y{ y } {}
			template<typename T>
			inline XYPair<T> XYPair<T>::operator+(const XYPair& other) const
			{
				XYPair xyPair{ *this };
				return xyPair += other;
			}
			template<typename T>
			inline XYPair<T> XYPair<T>::operator-(const XYPair& other) const
			{
				XYPair xyPair{ *this };
				return xyPair -= other;
			}
			template<typename T>
			inline XYPair<T> XYPair<T>::operator+(float delta) const
			{
				XYPair xyPair{ *this };
				return xyPair += delta;
			}
			template<typename T>
			inline XYPair<T> XYPair<T>::operator-(float delta) const
			{
				XYPair xyPair{ *this };
				return xyPair -= delta;
			}
			template<typename T>
			inline  XYPair<T>& XYPair<T>::operator-=(const XYPair& other)
			{
				x -= other.x;
				y -= other.y;
				return *this;
			}
			template<typename T>
			inline  XYPair<T>& XYPair<T>::operator+=(const XYPair& other)
			{
				x += other.x;
				y += other.y;
				return *this;
			}

			template<typename T>
			inline  XYPair<T>& XYPair<T>::operator-=(float delta)
			{
				x -= delta;
				y -= delta;
				return *this;
			}
			template<typename T>
			inline  XYPair<T>& XYPair<T>::operator+=(float delta)
			{
				x += delta;
				y += delta;
				return *this;
			}
			template<typename T>
			struct Rect
			{
				constexpr Rect(T left, T top, T right, T bottom):
					leftTop{left, top},
					rightBottom{right, bottom}
				{

				}
				Rect() {}
				constexpr Rect(const XYPair<T>& leftTop, const XYPair<T>& rightBottom) :
					leftTop{ leftTop },
					rightBottom{ rightBottom }
				{

				}
				T Width()const { return rightBottom.x - leftTop.x; }
				T Height()const { return rightBottom.y - leftTop.y; }
				XYPair<T> Center()const { return {(rightBottom.x - leftTop.x)/(T)2, (rightBottom.y - leftTop.y) / (T)2}; }
				XYPair<T> leftTop;
				XYPair<T> rightBottom;
			};
			typedef XYPair<int> SizeU;
			typedef XYPair<int> ImageXY;
			typedef Rect<int> ImageRect;
			class Vector2
			{
			public:
				Vector2() :Vector2{ 0.f, 0.f } {}
				constexpr Vector2(float x, float y);
				Vector2(const Vector2& other) :Vector2{ other.x, other.y } {}
				Vector2(const POINT& pos);
				float Length()const;
				Vector2 operator + (const Vector2& other) const;
				Vector2 operator - (const Vector2& other) const;
				Vector2 operator / (const Vector2& other) const;
				Vector2 operator / (float other) const;
				Vector2 operator * (float other) const;
				Vector2& operator *= (float other);
				Vector2& operator += (const Vector2& other);
				Vector2& operator -= (const Vector2& other);
				Vector2& operator /= (const Vector2& other);
				Vector2& operator /= (float other) ;
				bool operator == (const Vector2& other) const;
				float x;
				float y;
			};
			using Size =  Vector2 ;
			template<typename _floatT>
			struct MatrixRow
			{
			public:
				MatrixRow(_floatT(&row)[3]) :row{ row } {}
				const _floatT& operator[](size_t index)const&&
				{
					return row[index];
				}
				_floatT& operator[](size_t index) &&
				{
					return row[index];
				}
				_floatT(&row)[3];
			};
			struct Matrix
			{
			public:
				static Matrix Identity();
				static Matrix Translate(float dx, float dy);
				static Matrix Translate(const Vector2& point) { return Translate(point.x, point.y); }
				static Matrix Rotate(float degree);
				static Matrix Rotate(const Vector2& vector) { return Rotate(atan2f(vector.y, vector.x)); }
				static Matrix Scale(float sx, float sy);
			public:
				Matrix();
				Matrix(const Matrix& other);
				
				void operator =(const Matrix& other);
				MatrixRow<float> operator[](size_t index)
				{
					return MatrixRow<float>{ item[index] };
				}
				const MatrixRow<const float> operator[](size_t index) const
				{
					return MatrixRow<const float>{ item[index] };
				}
				Matrix operator * (const Matrix& other)const;
				Matrix operator + (const Matrix& other)const;
				Matrix operator - (const Matrix& other)const;
				explicit operator Vector2() const;
#ifdef WIN32
				explicit operator XFORM()const;
#ifdef DCOMMON_H_INCLUDED
				explicit operator D2D1_MATRIX_3X2_F()const;
#endif
#endif
				float item[3][3];
			};
			inline Matrix operator * (float number, const Matrix& mat)
			{
				Matrix res = mat;
				for (size_t y = 0; y < 3; ++y)
				{
					res.item[y][0] *= number;
					res.item[y][1] *= number;
					res.item[y][2] *= number;
				}
				return res;
			}
			inline Matrix Matrix::Identity()
			{
				Matrix res{};
				res.item[0][0] = 1;
				res.item[1][1] = 1;
				res.item[2][2] = 1;
				return res;
			}

			inline Matrix Matrix::Translate(float dx, float dy)
			{
				Matrix res = Identity();
				res.item[2][0] = dx;
				res.item[2][1] = dy;
				return res;
			}

			inline Matrix Matrix::Rotate(float degree)
			{
				Matrix res{Identity()};
				res.item[0][0] = (float)cosf(degree);
				res.item[0][1] = (float)sinf(degree);
				res.item[1][0] = (float)-1 * res.item[1][0];
				res.item[1][1] = res.item[0][0];
				
				return res;
			}

			inline Matrix Matrix::Scale(float sx, float sy)
			{
				Matrix mat{ Identity() };
				mat.item[0][0] = sx;
				mat.item[1][1] = sy;
				return mat;
			}

			inline Matrix::Matrix() :
				item{  }
			{

			}
			inline Matrix::Matrix(const Matrix& other) :
				Matrix{}
			{
				memcpy(item, other.item, sizeof(item));
			}
			inline void Matrix::operator=(const Matrix& other)
			{
				memcpy(item, other.item, sizeof(item));
			}
			inline Matrix Matrix::operator*(const Matrix& other) const
			{
				Matrix res;
				for (size_t y = 0; y < 3; ++y)
				{
					for (size_t x = 0; x < 3; ++x)
					{
						float value = 0.f;
						value += item[y][0] * other.item[0][x];
						value += item[y][1] * other.item[1][x];
						value += item[y][2] * other.item[2][x];
						res.item[y][x] = value;
					}
				}
				return res;
			}
			inline Matrix Matrix::operator+(const Matrix& other) const
			{
				Matrix res;
				for (size_t x = 0; x < 3; ++x)
				{
					res.item[0][x] = item[0][x] + other.item[0][x];
					res.item[1][x] = item[1][x] + other.item[1][x];
					res.item[2][x] = item[2][x] + other.item[2][x];
				}
				return res;
			}
			inline Matrix Matrix::operator-(const Matrix& other) const
			{
				Matrix res;
				for (size_t x = 0; x < 3; ++x)
				{
					res.item[0][x] = item[0][x] - other.item[0][x];
					res.item[1][x] = item[1][x] - other.item[1][x];
					res.item[2][x] = item[2][x] - other.item[2][x];
				}
				return res;
			}
			inline Matrix::operator Vector2() const
			{
				Vector2 res;
				res.x = item[2][0];
				res.y = item[2][1];
				return res;
			}
#ifdef WIN32
			inline Matrix::operator XFORM() const
			{
				XFORM res{};
				res.eDx = item[2][0];
				res.eDy = item[2][1];
				res.eM11 = item[0][0];
				res.eM12 = item[0][1];
				res.eM21 = item[1][0];
				res.eM22 = item[1][1];
				return res;
			}
#ifdef DCOMMON_H_INCLUDED
			inline Matrix::operator D2D1_MATRIX_3X2_F() const
			{
				D2D1_MATRIX_3X2_F res{};
				res.dx = item[2][0];
				res.dy = item[2][1];
				res.m11 = item[0][0];
				res.m12 = item[0][1];
				res.m21 = item[1][0];
				res.m22 = item[1][1];
				return res;
			}
#endif

#endif
			struct RectF
			{
				using Transform = _2D::Vector2;
			public:
				RectF(float left = 0.f, float top = 0.f, float right = 0.f, float bottom = 0.f) :leftTop{ left, top }, rightBottom{ right, bottom }{}
				RectF(const Transform& leftTop, const Transform& rightBottom) : leftTop{ leftTop }, rightBottom{ rightBottom }{}
				RectF operator + (const _2D::Vector2& xy) const
				{
					return { leftTop + xy , rightBottom + xy };
				}
				RectF operator - (const Transform& xy) const
				{
					return { leftTop + xy , rightBottom + xy };
				}
				Transform leftTop;
				Transform rightBottom;
			};
		}
		namespace _3D
		{
			class Vector3 :public _2D::Vector2
			{
			public:
				Vector3(const _2D::Vector2&);
				Vector3(float x = 0.f, float y = 0.f, float z = 0.f);
				Vector3(const Vector3& other) :Vector3{ other.x, other.y, other.z } {}
				operator _2D::Vector2()const;
				Vector3 operator + (const Vector3& other) const;
				Vector3 operator - (const Vector3& other) const;
				Vector3& operator += (const Vector3& other);
				Vector3& operator -= (const Vector3& other);

				float z;
			};
		}
		namespace _3D
		{
			inline Vector3::Vector3(const _2D::Vector2& other) :
				Vector3{ other.x, other.y }
			{

			}
			inline Vector3::Vector3(float x, float y, float z) :
				_2D::Vector2{ x,y },
				z{ z }
			{

			}
			inline Vector3::operator _2D::Vector2()const
			{
				return { x, y };
			}
			inline Vector3 Vector3::operator + (const Vector3& other) const
			{
				return { x + other.x, y + other.y, z + other.z };
			}
			inline Vector3 Vector3::operator - (const Vector3& other) const
			{
				return { x - other.x, y - other.y, z - other.z };
			}
			inline Vector3& Vector3::operator -= (const Vector3& other)
			{
				x -= other.x;
				y -= other.y;
				z -= other.z;
				return *this;
			}
			inline Vector3& Vector3::operator += (const Vector3& other)
			{
				x += other.x;
				y += other.y;
				z += other.z;
				return *this;
			}
		}
		namespace _2D
		{
			inline Vector2::Vector2(const POINT& pos) :Vector2{ (float)pos.x, (float)pos.y } {}
			inline constexpr Vector2::Vector2(float x, float y) :
				x{ x },
				y{ y }
			{

			}
			inline Vector2 Vector2::operator + (const Vector2& other) const
			{
				return { x + other.x, y + other.y };
			}
			inline Vector2 Vector2::operator - (const Vector2& other) const
			{
				return { x - other.x, y - other.y };
			}
			inline Vector2 Vector2::operator/(const Vector2& other) const
			{
				return Vector2{ x / other.x, y / other.y };
			}
			inline Vector2 Vector2::operator/(float other) const
			{
				auto tmp{ *this };
				return tmp /= other;
			}
			inline Vector2 Vector2::operator*(float other) const
			{
				auto res = *this;
				res *= other;
				return res;
			}
			inline Vector2& Vector2::operator*=(float other)
			{
				x *= other;
				y *= other;
				return *this;
			}
			inline Vector2& Vector2::operator -= (const Vector2& other)
			{
				x -= other.x;
				y -= other.y;
				return *this;
			}

			inline Vector2& Vector2::operator/=(const Vector2& other)
			{
				x /= other.x;
				y /= other.y;
				return *this;
			}

			inline Vector2& Vector2::operator/=(float other) 
			{
				x /= other;
				y /= other;
				return *this;
			}

			inline Vector2& Vector2::operator += (const Vector2& other)
			{
				x += other.x;
				y += other.y;
				return *this;
			}
			inline Vector2 operator * (float op1, const Vector2& op2)
			{
				return op2 * op1;
			}
			inline bool Vector2::operator == (const Vector2& other) const
			{
				return x == other.x && y == other.y;
			}
			inline float Vector2::Length() const
			{
				//길이는 피타고라스의 정리에 의해서 sqrtf(x^2 + y^2)
				return sqrtf(powf(x, 2) + powf(y, 2));
			}
		}
	}
	
}