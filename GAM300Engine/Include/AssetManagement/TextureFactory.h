#pragma once
#include "FactoryBase.h"
#include "../GraphicsResource/TextureInfo.h"
#define MAX_NUM_PRELOAD_TEXTURE 10
#define TEXTURE_PATH "../assets/textures/"
namespace TDS
{

	template<>
	class AssetFactory<Texture>
	{

	public:
		std::array<Texture, 2000> m_TextureArray;
		std::unordered_map<std::string, std::uint32_t> m_TextureIndices;
		std::uint32_t m_CurrentIndex = 0;
		bool m_UpdateTextureArray = false;
		/*void Preload(ResourceAllocator& resourceMgr)
		{
			std::uint32_t NumOfLoadedTexture = 0;
			std::filesystem::path dir = TEXTURE_PATH;

			if (!std::filesystem::exists(dir) || !std::filesystem::is_directory(dir))
			{
				std::cout << "Invalid directory" << std::endl;
				return;
			}

			for (const auto& entry : std::filesystem::directory_iterator(dir))
			{
				if (NumOfLoadedTexture >= MAX_NUM_PRELOAD_TEXTURE)
					break;
				const std::filesystem::path& path = entry.path();
				if (path.extension() == ".dds")
				{
					TypeReference<Texture> TextureInstance{};

					std::string fileName = path.filename().string();

					TextureInstance.m_AssetName = fileName;

					auto newTexture = resourceMgr.LoadResource(TextureInstance);
					if (newTexture == nullptr)
					{
						std::cout << "Failed to get texture instance!" << std::endl;
						continue;
					}
					newTexture->LoadTexture(path.string());
				}
			}
		}*/
		std::array<Texture, 2000>& GetTextureArray()
		{
			return m_TextureArray;
		}
		void Preload()
		{
			std::uint32_t NumOfLoadedTexture = 0;
			std::filesystem::path dir = TEXTURE_PATH;

			if (!std::filesystem::exists(dir) || !std::filesystem::is_directory(dir))
			{
				std::cout << "Invalid directory" << std::endl;
				return;
			}

			for (const auto& entry : std::filesystem::directory_iterator(dir))
			{
				if (NumOfLoadedTexture >= MAX_NUM_PRELOAD_TEXTURE)
					break;
				const std::filesystem::path& path = entry.path();
				if (path.extension() == ".dds")
				{
					std::string fileName = path.filename().string();
					m_TextureArray[m_CurrentIndex].LoadTexture(path.string());
					m_TextureIndices[fileName] = m_CurrentIndex++;
				}
			}
			m_UpdateTextureArray = true;
		}
		//static void Load(std::string_view path, TypeReference<Texture>& textureRef, ResourceAllocator& resourceMgr)
		//{
		//	std::filesystem::path FilePath(path);
		//	std::string fileName = FilePath.filename().string();
		//	textureRef.m_AssetName = fileName;
		//	if (!resourceMgr.GetResource(textureRef))
		//	{
		//		if (!resourceMgr.LoadResource(textureRef))
		//		{
		//			TDS_WARN("Failed to load resource!");
		//			return;
		//		}
		//	}
		//	textureRef.m_ResourcePtr->LoadTexture(path);
		//}
		Texture* GetTexture(std::string_view textureName)
		{
			auto itr = m_TextureIndices.find(textureName.data());
			if (itr != m_TextureIndices.end())
			{
				return &m_TextureArray[itr->second];
			}
			return nullptr;
			TDS_WARN("Texture {} doesnt exist!", textureName.data());
		}
		int GetTextureIndex(std::string_view textureName)
		{
			auto itr = m_TextureIndices.find(textureName.data());
			if (itr != m_TextureIndices.end())
			{
				return (int)itr->second;
			}
			return -1;
		}
		static void Load(std::string_view path, TypeReference<Texture>& textureRef, AssetFactory<Texture>& textureFactory)
		{
			std::filesystem::path FilePath(path);
			std::string fileName = FilePath.filename().string();
			textureRef.m_AssetName = fileName;
			auto& textureArray = textureFactory.m_TextureArray;
			auto& textureIndices = textureFactory.m_TextureIndices;

			auto itr = textureIndices.find(fileName);
			if (itr != textureIndices.end())
			{
				TDS_INFO("texture {} is already loaded!", textureRef.m_AssetName);

				textureRef.m_ResourcePtr = &textureArray[itr->second];
				return;
			}
			std::uint32_t& newIndex = textureFactory.m_CurrentIndex;
			textureArray[newIndex].LoadTexture(path);
			textureRef.m_ResourcePtr = &textureArray[newIndex];
			textureFactory.m_UpdateTextureArray = true;
			textureIndices[fileName] = newIndex++;


		}
		void DestroyAllTextures()
		{
			for (auto& texture : m_TextureArray)
			{
				texture.Destroy();
			}
			m_TextureIndices.clear();
		}
	};



}