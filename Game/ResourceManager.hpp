#pragma once
#include<memory>
#include<string>
#include<unordered_map>
#include<mutex>
#include<shared_mutex>
#include "WowMapMeshObject.h"
#include "SkinnedXMeshObject.h"
class WowMapMeshObject;
class RenderModule;
namespace Kumazuma
{
	namespace Client
	{
		class CharacterMeta;
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
			auto LoadSkinnedMesh(std::wstring const& id, std::wstring const& path)->std::unique_ptr<SkinnedXMeshObject>;
			auto LoadCharacterMetaData(std::wstring const& id, std::wstring const& path)->void;
			auto GetCharacterMeta(std::wstring const& id)const->std::shared_ptr<CharacterMeta const>;
			auto GetSkinnedMesh(std::wstring const& id)const->std::unique_ptr<SkinnedXMeshObject>;
			auto Clear()->void;
		private:
			std::shared_ptr<RenderModule> m_pRenderModule;
			std::unordered_map < std::wstring, std::unique_ptr<WowMapMeshObject> >m_objMeshs;
			std::unordered_map < std::wstring, std::unique_ptr<SkinnedXMeshObject> > m_skinnedMeshs;
			std::unordered_map < std::wstring, std::shared_ptr<CharacterMeta> > m_characterMetas;
			std::unordered_map < std::wstring, std::wstring > m_idMeshPathTable;
			
			mutable std::shared_mutex m_mutex;
			
		private:
			static std::shared_ptr<ResourceManager> s_instance;
			
		};
	}
}