#pragma once
#include "ResourceManagement/ResourceRef.h"
#include "Logger/Logger.h"
namespace TDS
{
#define MAX_RESOURCE 8192
	class ResourceAllocator
	{
	private:
		std::array<AllocationInfo, MAX_RESOURCE> m_InfoBuffer;
		std::unordered_map<std::string, std::uint32_t> m_ResourceInstance;
		std::set<std::string> m_CurrentAssets;
		std::uint32_t m_CurrentIndex = 0;
	public:
		ResourceAllocator();
		~ResourceAllocator();
		std::set<std::string>& GetLoadedAssetNames();

		void RemoveAssetNameFromList(std::string_view name);

		void FreeAllResources();


		template <typename T>
		T* LoadResource(TypeReference<T>& reference)
		{
			auto itr = m_ResourceInstance.find(reference.m_AssetName);
			if (itr != m_ResourceInstance.end())
			{
				TDS_WARN("ASSET ALREADY LOADED!");
				reference.m_AssetName = itr->first;
				reference.m_ResourcePtr = reinterpret_cast<T*>(m_InfoBuffer[itr->second].m_pData);
				++m_InfoBuffer[itr->second].ReferenceCnt;
				return reinterpret_cast<T*>(reference.m_ResourcePtr);
			}
			reference.m_ResourcePtr = new T();

			m_ResourceInstance.insert(std::make_pair(reference.m_AssetName, m_CurrentIndex));
			if constexpr (std::is_member_function_pointer_v<decltype(&T::Destroy)>)
				m_InfoBuffer[m_CurrentIndex].m_Destroyfunc = std::bind(&T::Destroy, reference.m_ResourcePtr);


			m_InfoBuffer[m_CurrentIndex].m_pData = reference.m_ResourcePtr;
			m_InfoBuffer[m_CurrentIndex].m_AssetName = reference.m_AssetName;
			m_InfoBuffer[m_CurrentIndex].ReferenceCnt = 1;
			m_CurrentAssets.insert(reference.m_AssetName);

			m_CurrentIndex++;
			return reference.m_ResourcePtr;
		}


		template <typename T>
		T* GetResource(TypeReference<T>& reference)
		{
			auto itr = m_ResourceInstance.find(reference.m_AssetName);
			if (itr != m_ResourceInstance.end())
			{
				if (m_InfoBuffer[itr->second].m_pData == nullptr)
				{
					TDS_ERROR("Something is wrong here... Resources should be allocated the moment you loaded");
					return nullptr;
				}
				else
				{
					reference.m_ResourcePtr = reinterpret_cast<T*>(m_InfoBuffer[itr->second].m_pData);
					++m_InfoBuffer[itr->second].ReferenceCnt;
				}
				return reference.m_ResourcePtr;
			}
			TDS_WARN("Resource is not loaded!");
			return nullptr;
		}
		template <typename T>
		void RemoveReference(TypeReference<T>& reference)
		{
			if (reference.m_AssetName.empty() || reference.m_ResourcePtr == nullptr)
			{
				TDS_WARN("Nothing to remove!");
				return;
			}


			auto itr = m_ResourceInstance.find(reference.m_AssetName);
			if (itr != m_ResourceInstance.end())
			{
				reference.m_ResourcePtr = nullptr;
				reference.m_AssetName = "";
				--m_InfoBuffer[itr->second].ReferenceCnt;
				
			}
			else
			{
				TDS_WARN("Theres no such reference!");
			}

		}

	};

}