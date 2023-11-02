#include "AssetManagement/ResourceAllocator.h"
#include "Logger/Logger.h"
namespace TDS
{
	ResourceAllocator::ResourceAllocator()
	{
	}

	ResourceAllocator::~ResourceAllocator()
	{
	}

	std::set<std::string>& ResourceAllocator::GetLoadedAssetNames()
	{
		return m_CurrentAssets;
	}

	void ResourceAllocator::RemoveAssetNameFromList(std::string_view name)
	{
		if (m_CurrentAssets.find(name.data()) != m_CurrentAssets.end())
			m_CurrentAssets.erase(name.data());
	}

	void ResourceAllocator::FreeAllResources()
	{
		for (auto& alloc : m_InfoBuffer)
		{
			if (alloc.m_Destroyfunc)
			{
				alloc.m_Destroyfunc.value()();
				alloc.m_Destroyfunc.reset();
			}
			alloc.m_AssetName = "";
			alloc.ReferenceCnt = 0;
		}
		m_CurrentIndex = 0;
		m_ResourceInstance.clear();


	}

}