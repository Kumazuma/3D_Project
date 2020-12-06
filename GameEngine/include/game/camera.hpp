#pragma once
#pragma once
#include "transform.hpp"
namespace Kumazuma
{
	namespace Game
	{
		namespace _2D
		{
			//Camera클래스
			class Camera
			{
			public:
				Camera(const Vector2& position = {});
				//마우스 좌표 따위를 현재 카메라 뷰로 매핑한다
				Vector2 Map(const POINT& pt)const;
				void Move(const Vector2& translate);
				void SetCenter(const Vector2& translate);
				Vector2 GetCenter()const { return m_center; }
				Matrix GetMatrix()const {
					auto center = m_center;
					center.x *= -1;
					return Matrix::Translate(-1 * center) * Matrix::Translate(m_size.x / 2.f, m_size.y / 2.f);
				}
				const Size& GetSize()const { return m_size; }
				void SetSize(const Size& size);
				bool TestInViewport(const Vector2& point)const;
				Vector2 ConvertToInViewport(const Vector2& point) const;
				RectF GetViewport() const;
			private:
				Vector2 m_center;
				Vector2 m_scale;
				Size m_size;
			};

			inline void Camera::Move(const Vector2& translate)
			{
				m_center = static_cast<Vector2>(Matrix::Translate(translate) * Matrix::Translate(m_center) );
			}
			inline void Camera::SetCenter(const Vector2& translate)
			{ 
				m_center = translate;
			}
			inline void Camera::SetSize(const Size& size)
			{
				m_size = size;
			}
			inline bool Camera::TestInViewport(const Vector2& point) const
			{
				Vector2 mappedPos{ ConvertToInViewport(point) };
				//Transform mapped { Matrix{ point } * Matrix{ m_size / -2.f } };
				//less then view width per 2 and less then view height per 2
				return fabsf(mappedPos.x) <=  m_size.x / 2.f && fabsf(mappedPos.y) <= m_size.y / 2.f;
			}
			inline Vector2 Camera::ConvertToInViewport(const Vector2& point) const
			{
				//카메라의 중심을 기준으로 하는 좌표계로 사상한다.
				//point = (4 , 4) , center = (12 , 12) mapped point = (-8, -8)
				//
				auto mat = Matrix::Translate(point);
				//Y축을 뒤집는다.
				auto t = Matrix::Scale(-1.f, -1.f);
				mat = mat * t;
				mat[0][0] *= -1.f;
				mat[1][1] *= -1.f;
				mat = mat * Matrix::Translate(-1 * m_center);
				return Vector2 { mat };
			}
			inline RectF Camera::GetViewport() const
			{
				return RectF{ -0.5f * m_size,  0.5f * m_size} ;
			}
			inline Camera::Camera(const Vector2& position)
			{
				m_center = position;
				m_scale = Vector2{ 1,1 };
			}
			inline Vector2 Camera::Map(const POINT& pt)const
			{
				auto mat = Matrix::Translate((float)pt.x, (float)pt.y);
				
				//화면의 정 중앙을 기준으로 한 좌표계로 사상한다.
				mat = mat * Matrix::Translate(m_size / -2.f);
				//Y축을 뒤집는다.
				auto t = Matrix::Identity();
				t[1][1] = -1.f;
				mat = mat * t;
				mat[1][1] = 1.f;
				//카메라의 중심을 기준으로 하는 좌표계로 사상한다.
				mat = mat * Matrix::Translate(m_center);
				return (Vector2)mat;
			}
		}
	}
}