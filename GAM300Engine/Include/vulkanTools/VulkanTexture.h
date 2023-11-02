#pragma once
#include "../GraphicsResource/TextureInfo.h"
#include <vulkan/vulkan.h>
#include "GraphicsAllocator.h"
#include "dotnet/ImportExport.h"
namespace TDS
{
	class VulkanInstance;
	struct ImageMemoryLayoutInput
	{
		VkImage m_Image = nullptr;
		VkAccessFlags m_srcAccessFlags{ VK_ACCESS_TRANSFER_WRITE_BIT }, m_dstAccessFlags{ VK_ACCESS_TRANSFER_READ_BIT };
		VkImageLayout m_oldLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
		VkImageLayout m_NewLayout = VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		VkPipelineStageFlags m_srcStageFlags{ VK_PIPELINE_STAGE_ALL_COMMANDS_BIT }, m_dstStageFlags{ VK_PIPELINE_STAGE_ALL_COMMANDS_BIT };
		VkImageSubresourceRange m_subResourceRange;
	};

	struct CreateImageParams
	{
		VkFormat m_Format;
		VkImageTiling m_tiling;
		VkImageUsageFlags m_usage;

	};

	class VulkanTexture
	{
	public:
		DLL_API VulkanTexture();
		DLL_API ~VulkanTexture();

		void DLL_API						CreateTexture(TextureInfo& textureAssetData);

		static std::shared_ptr<VulkanTexture>	CreateDefaultTexture();

		static std::shared_ptr<VulkanTexture>	CreateDefaultCubeTexture(bool useAnistrophy = false);

		void DLL_API						CreateCubeMapTexture(TextureInfo& textureAssetData, TextureData& textureData);


		void DLL_API						CreateBasicTexture(TextureInfo& textureAssetData);
		void DLL_API						CreateSampler(TextureInfo& textureInfo);
		void DLL_API						CreateSamplerViaCuteMap(TextureInfo& textureInfo);
		void DLL_API						CreateImageViewViaCubeMap(TextureInfo& textureInfo);
		void DLL_API						CreateImageView(VkFormat m_Format);
		static void							SetImageMemoryBarrier(VkCommandBuffer& cmdbuffer, ImageMemoryLayoutInput& layout);


		static VkImage					CreateVulkanImage(VkExtent2D extents, std::int32_t mipLevels, VkSampleCountFlagBits samplecntFlag,
			CreateImageParams& imageCreateParams, VmaAllocation& allocation, std::uint32_t NumOfArrayLayers = 1);
		static VkImage					CreateCubeImage(VkExtent2D extents, std::int32_t mipLevels, VkSampleCountFlagBits samplecntFlag,
			CreateImageParams& imageCreateParams, VmaAllocation& allocation, std::uint32_t NumOfArrayLayers = 6);

		static void						setTextureLayout(VkCommandBuffer& cmdBuffer,
			ImageMemoryLayoutInput layoutParam);


		void DLL_API						Destroy();


		DLL_API VkExtent2D					GenMipsExtent(TextureData& data, std::uint32_t mip);

		DLL_API VkWriteDescriptorSet& getWriteSet();
		DLL_API VkDescriptorImageInfo& getInfo();
		DLL_API VkImage& GetImage();

	private:
		VkImage							m_ImageHdl = nullptr;
		VkImageView						m_BaseImageView = nullptr;
		VmaAllocation					m_Allocation = nullptr;
		VkSampler						m_Sampler = nullptr;
		VkImageLayout					m_ImageLayout;
		VkDescriptorImageInfo			m_DescriptorImageInfo;
		VkWriteDescriptorSet			m_WriteSet = { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
		


	};

	class DefaultTextures
	{
	public:
		DefaultTextures();
		~DefaultTextures();

		void DLL_API							Init();
		static							std::shared_ptr<VulkanTexture> GetDefaultTexture();
		static							std::shared_ptr<VulkanTexture> GetDefaultCubeTexture();
		static							std::shared_ptr<VulkanTexture> GetDefaultStorage();
		void DLL_API							DestroyDefaultTextures();
		static DefaultTextures& GetInstance();
	;		private:
		inline static					std::shared_ptr<DefaultTextures> m_Instance = nullptr;
		std::shared_ptr<VulkanTexture>	m_DefaultTexture = nullptr;
		std::shared_ptr<VulkanTexture>	m_DefaultCubeTexture = nullptr;
		std::shared_ptr<VulkanTexture>  m_DefaultStorage = nullptr;
	};
}