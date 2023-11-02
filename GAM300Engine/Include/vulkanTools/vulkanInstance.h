/*!*************************************************************************
****
\file vulkanInstance.h
\author Ng Zuo Xian Amadeus
\par DP email: ng.z@digipen.edu
\par Course: CSD3400
\par Section: a
\date 22-9-2023
\brief  Contains the vulkanInstance Class and its member functions
****************************************************************************
***/
#ifndef VULKAN_CREATE_INSTANCE
#define VULKAN_CREATE_INSTANCE

#ifndef NOMINMAX
#define NOMINMAX
#endif//NOMINMAX
#define	VK_USE_PLATFORM_WIN32_KHR

#include <vulkan/vulkan.h>
#include <vector>
//#include <string>
//#include <iostream>
//#include <set>
//#include <limits>
//#include <algorithm>
//#include <fstream>
#include <optional>
#include "GraphicsAllocator.h"
//#include <array>

//assimp to be moved
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
//assimp
#include "windowswindow.h"
#include "vulkanTools/vulkanDebugger.h"
#include "dotnet/ImportExport.h"
//#include "vulkanTools/vulkanDevice.h"
//#include "TDSMath.h"
////#include "vulkanSwapChain.h"
//#include "Vector2.h"
//#include "dotnet/ImportExport.h"
//#include "camera/camera.h"
namespace TDS
{

	class DLL_API VulkanInstance
	{
	public:
		struct QueueFamilyIndices
		{

			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t>	presentFamily;
			std::optional<uint32_t> computeFamily;
			bool isComplete()
			{
				return graphicsFamily.has_value() &&
					presentFamily.has_value() && computeFamily.has_value();
			}

		};

		struct SwapChainSupportDetails
		{
			VkSurfaceCapabilitiesKHR capabilities{};
			std::vector<VkSurfaceFormatKHR> formats{};
			std::vector<VkPresentModeKHR> presentModes{};
		};

	public:
		//constructor and destructor
		VulkanInstance(const WindowsWin& enableWindows);
		~VulkanInstance();

		//no copyable or movable
		VulkanInstance(const VulkanInstance&) = delete;
		VulkanInstance(VulkanInstance&&) = delete;
		VulkanInstance& operator=(const VulkanInstance&) = delete;
		VulkanInstance& operator=(VulkanInstance&&) = delete;


		//getters
		VkInstance       getInstance() const { return m_VKhandler; }
		VkPhysicalDevice getVkPhysicalDevice()const { return m_PhysDeviceHandle; }
		VkDevice		 getVkLogicalDevice()const { return m_logicalDevice; }
		/*VkCommandPool	 getCommandPool() const { return m_CommandPool; }*/
		VkQueue          getGraphicsQueue() { return  m_graphicQueue; }
		VkQueue          getPresentQueue() { return m_presentQueue; }
		VkQueue			 getComputeQueue() { return m_ComputeQueue; }
		VkSurfaceKHR     getSurface() { return m_Surface; }
		bool			 getVsync()const { return vsync; }
		void			 ShutDown();

		//helpers
		SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(m_PhysDeviceHandle); }
		QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(m_PhysDeviceHandle); }
		uint32_t findMemoryType(const uint32_t& typeFiler, VkMemoryPropertyFlags properties);
		VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

		//TODO: Revamped
		void createBuffers(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer,
			VkDeviceMemory& buffermemory);
		VkCommandBuffer	beginSingleTimeCommands();
		void endSingleTimeCommands(VkCommandBuffer commandBuffer);
		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);
		void createImageWithInfo(const VkImageCreateInfo& imageInfo, VkMemoryPropertyFlags properties,
			VkImage& image, VkDeviceMemory& imageMemory);


		inline std::uint32_t GetGraphicsQueueIndex()
		{
			return m_SelectedIndices.graphicsFamily.value();
		}
		inline std::uint32_t GetComputeQueueIndex()
		{
			return m_SelectedIndices.computeFamily.value();
		}
		inline std::uint32_t GetPresentQueueIndex()
		{
			return m_SelectedIndices.presentFamily.value();
		}


	private:

		VkInstance			m_VKhandler{ nullptr };
		VkSurfaceKHR		m_Surface{ nullptr };

		VkPhysicalDevice	m_PhysDeviceHandle{ VK_NULL_HANDLE }; //where selected graphic card is stored

		VkDevice			m_logicalDevice{ nullptr };
		VkQueue				m_graphicQueue{ nullptr };
		VkQueue				m_presentQueue{ nullptr };
		VkQueue				m_ComputeQueue{ nullptr };

		QueueFamilyIndices	m_SelectedIndices{};

		/*VkCommandPool		m_CommandPool{ nullptr };*/

		VkPhysicalDeviceProperties m_Properties;

		std::vector<std::string> supportedInstanceExtensions{};
		std::vector<const char*> enabledInstanceExtensions{};

		const std::vector<const char*> validationLayers{ "VK_LAYER_KHRONOS_validation" };
		//const std::vector<const char*> deviceExtensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		//	VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
		//	VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
		//	VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
		//	VK_KHR_SPIRV_1_4_EXTENSION_NAME,
		//	VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME};
		const std::vector<const char*> deviceExtensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME,
			VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME};

		static constexpr decltype(VkApplicationInfo::apiVersion) apiVersion{ VK_API_VERSION_1_3 };

		bool		  vsync{ false };
		bool		  enableValidate{ false };



	private:

		VkResult createInstance(bool enableValidation);
		void CreateSurface(const WindowsWin& _Windows);
		void ChoosePhysicalDevice();
		void CreateLogicalDevice(const WindowsWin& _Windows);
		//void CreateCommandPool();


		//helper functions
		bool checkValidationLayerSupport();
		bool isDeviceSuitable(VkPhysicalDevice device);
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		SwapChainSupportDetails  querySwapChainSupport(const VkPhysicalDevice& device);

	};


}//namespace TDS



#endif//!VULKAN_CREATE_INSTANCE