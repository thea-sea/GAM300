#ifndef RENDERTARGET
#define	RENDERTARGET

#include "vulkanTools/vulkanInstance.h"
namespace TDS {

	enum class RenderTargetType
	{
		DEPTH = 0,
		DEPTHSTENCIL,
		COLOR,
		RESOLVE			// multisampling resolve
	};

	struct RenderTargetCI
	{
		VkFormat			  _format				{};
		VkImageAspectFlagBits _imageAspect			{};
		VkImageViewType		  _viewType				{};
		Vec3				  _dim					{};
		VkImageUsageFlags	  _imageUsage			{};
		VkImageLayout		  _imagelayout			{};
		RenderTargetType      _renderTargetType		{};
		bool				  _linearFiltering		{ true };
		VkSampleCountFlagBits _sampleCount			{ VK_SAMPLE_COUNT_1_BIT };
		bool				  _enablePCFSampler		{ false };									//Percentage Closer Filtering for shadow mapping
		uint32_t			  _layerCount			{ 1 };
		uint32_t			  _mipLevels			{ 1 };
		bool				  _overrideDirectLayer	{ false };									//for lowest layering override
		VkImage				  _inheritVkImage		{ VK_NULL_HANDLE };
	};



	class RenderTarget
	{
	public:
		RenderTarget(std::shared_ptr<VulkanInstance> inst, const RenderTargetCI& attachmentCI);
		~RenderTarget();

		//no move no copy ctor 
		RenderTarget(const RenderTarget&) = delete;
		RenderTarget& operator=(const RenderTarget&) = delete;
		RenderTarget(RenderTarget&&) = delete;
		RenderTarget& operator=(RenderTarget&&) = delete;

		//getters
		VkImage getImage() const { return m_Image; }
		VkImageSubresourceRange getImageSubResourceRange() { return m_SubResourceRange; }
		VkImageView getImageView() const { return m_ImageView; }
		VkImageView getImageViewSB() const { return m_ImageViewSB; }
		VkImageView getRenderImageView(uint32_t layer, uint32_t miplevel)
		{
			return m_RenderImageViews[layer][miplevel];
		}

		VkSampler getSampler() const { return m_Sampler; }
		VkImageLayout getImageLayout() const { return m_AttachmentDescription._imagelayout; }
		VkDeviceMemory getDevicMemory() { return m_ImageMem; }

		VkDescriptorImageInfo getImageInfoDescriptor()
		{
			return { m_Sampler, m_ImageView, m_AttachmentDescription._imagelayout };
		}


		VkDescriptorImageInfo getStencilBufferDescriptor()
		{
			return { m_Sampler,m_ImageViewSB, m_AttachmentDescription._imagelayout };
		}

		Vec3 getDimensions() { return m_Dimensions; }
		size_t getResourceSize() { return m_MemSize; }
		
		RenderTargetType getAttachmentType() { return m_AttachmentDescription._renderTargetType; }
		const RenderTargetCI& getAttachmentDescription() { return m_AttachmentDescription; }

		void resize(Vec3 newDim);
		

	private:

		void destroy(); //for resize or destroy
		void create(std::shared_ptr<VulkanInstance> inst, const RenderTargetCI& attachmentCI);

		VkImage					m_Image{};
		VkDeviceMemory			m_ImageMem{};
		VkSampler				m_Sampler{};
		VkImageSubresourceRange m_SubResourceRange{};
		VkImageView				m_ImageView{};
		VkImageView				m_ImageViewSB{};				//SB == stencil buffer, !sha bi
		size_t					m_MemSize{ 0 };
		
		std::vector<std::vector<VkImageView>> m_RenderImageViews{};
		std::vector<std::vector<VkImageSubresourceRange>> m_RenderSubRR{}; //subresource ranges

		RenderTargetCI m_AttachmentDescription;

		std::shared_ptr<VulkanInstance> m_Inst;

		bool m_InheritsVkImage{ false };

		Vec3 m_Dimensions{};

	public:


	}; //End RenderTarget



}//End TDS


#endif // !RENDERTARGET
