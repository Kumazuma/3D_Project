#pragma once
#include <game/Component.hpp>
#ifndef Func
#define Func auto
#endif
namespace Kumazuma
{
	namespace MS2
	{
		class CollisionCom : public Kumazuma::Game::Component
		{
		//�Լ�
		public:
			CollisionCom();

		public:
			Func GetRadius()const->float;
			Func SetRadius(float radius)->void;
			/*
			float		GetRadius() const;
			void		SetRadius(const float& fRadius);
			*/
		//����
		public:	
			static const Kumazuma::Game::ComponentTag<CollisionCom> TAG;

		private:
			float m_radius;
		//Ư��
		public:
			Kumazuma::Game::Component* Clone() const override;
		};
	}
}


