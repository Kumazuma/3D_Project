#pragma once
#include"object.hpp"
#include<memory>
#include<memory>
#include"object.hpp"
#include"objectfactory.hpp"
#include"list.hpp"
namespace Kumazuma
{
	namespace Game
	{
		class Scene
		{
		public:
			Scene(Scene&& other) noexcept;
			virtual ~Scene() = default;
			virtual void Loaded() {}
			virtual void Unloaded() {}
			virtual void Update(float timeDelta) {}
			std::shared_ptr<Object> NewObject(const ObjectFactory& factory);
			void RemoveObject(Object& obj);
		protected:
			Scene() = default;
			LinkedList<std::shared_ptr<Object>> m_objects;

		private:
		};
	}
}
