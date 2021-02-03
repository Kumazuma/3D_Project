#pragma once
#include"object.hpp"
#include<memory>
#include<memory>
#include"object.hpp"
#include"objectfactory.hpp"
#include"list.hpp"
#include<unordered_map>
#include "LayerTag.hpp"
#include<shared_mutex>
#include "common.hpp"
namespace Kumazuma
{
	namespace Game
	{
		class DLL_CLASS Scene
		{
		public:
			using ObjList = LinkedList < std::shared_ptr<Object> >;
			Scene(Scene&& other) noexcept;
			virtual ~Scene() = default;
			virtual void Loaded() {}
			virtual void Unloaded() {}
			virtual void Update(float timeDelta) {}
			void AddObject(LayerTag const& tag, std::shared_ptr<Object>);
			void RemoveObject(LayerTag const& tag);
			void RemoveObject(LayerTag const& tag, Object& obj);
			void RemoveObject(Object& obj);
			ObjList const& GetListRef(LayerTag const& tag);

		protected:
			Scene() = default;
			std::unordered_map <LayerTag const*, ObjList > m_objects;
			std::shared_mutex m_mutex;

		private:
		};
	}
}
