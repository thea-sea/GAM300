#pragma once
#include "AssetManagement/ModelFactory.h"
#include "TextureFactory.h"
#include "dotnet/ImportExport.h"
#include "ResourceAllocator.h"

namespace TDS
{

	template <typename T>
	inline constexpr bool wrong_factory_type = false;

	class AssetManager
	{
	public:
		DLL_API AssetManager();
		DLL_API ~AssetManager();
		void DLL_API Init();
		void DLL_API PreloadAssets();
		void DLL_API ShutDown();

		//Runtime load
		//template <typename T>
		//void LoadAsset(std::string_view FilePath, TypeReference<T>& ref)
		//{
		//	AssetFactory<T>::Load(FilePath, ref);
		//}
		template <typename T>
		void LoadAsset(std::string_view FilePath, TypeReference<T>& ref)
		{
			(FilePath);
			(ref);
			static_assert(wrong_factory_type<T>, "No such factory");
		}

		template <>
		void LoadAsset<AssetModel>(std::string_view FilePath, TypeReference<AssetModel>& ref)
		{
			AssetFactory<AssetModel>::Load(FilePath, ref, m_ModelFactory);
		}
		template <>
		void LoadAsset<Texture>(std::string_view FilePath, TypeReference<Texture>& ref)
		{
			AssetFactory<Texture>::Load(FilePath, ref, m_TextureFactory);
		}
		//template <typename T>
		//T* GetResource(TypeReference<T>& ref)
		//{
		//	return m_ResourceAllocator.GetResource(ref);
		//}

		AssetFactory<AssetModel>& GetModelFactory()
		{
			return m_ModelFactory;
		}

		AssetFactory<Texture>& GetTextureFactory()
		{
			return m_TextureFactory;
		}

		//inline ResourceAllocator& getResourceAllocator()
		//{
		//	return m_ResourceAllocator;
		//}
		static DLL_API std::shared_ptr<AssetManager> GetInstance();


	private:
		inline static std::shared_ptr<AssetManager> m_Instance = nullptr;
		/*ResourceManager m_ResourceManager;*/
		//ResourceAllocator m_ResourceAllocator;
		AssetFactory<AssetModel> m_ModelFactory;
		AssetFactory<Texture> m_TextureFactory;


	};
}