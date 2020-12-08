#pragma once
#include<memory>
#include<string>
#include<unordered_map>
#include<mutex>
#include<shared_mutex>
#include "WowMapMeshObject.h"

class WowMapMeshObject;
class RenderModule;
namespace Kumazuma
{
	namespace Client
	{
		class ResourceManager
		{
			
		protected:
			ResourceManager(std::shared_ptr<RenderModule> pRenderModule);
			ResourceManager(ResourceManager&& rhs) noexcept;
		public:
			~ResourceManager();
		public:
			static auto Initialize(std::shared_ptr<RenderModule> pRenderModule)->bool;
			static auto Instance()->std::shared_ptr<ResourceManager>;
			static auto Release()->void;
		public:
			auto LoadOBJMesh(std::wstring const& path)->std::unique_ptr<WowMapMeshObject>;
			auto Clear()->void;
		private:
			std::shared_ptr<RenderModule> m_pRenderModule;
			std::unordered_map < std::wstring, std::unique_ptr<WowMapMeshObject> >m_objMeshs;
			std::shared_mutex m_mutex;
			
		private:
			static std::shared_ptr<ResourceManager> s_instance;
			
		};
	}
}