#ifndef OBJECT_PICK
#define OBJECT_PICK

#include "MathCommon.h"
#include "vulkanTools/VulkanPipeline.h"
#include "renderPass.h"
#include "vulkanTools/FrameBuffer.h"
#include "vulkanTools/vulkanInstance.h"
#include "vulkanTools/Synchronization.h"
#include "dotnet/ImportExport.h"

namespace TDS {

	class DLL_API ObjectPick
	{
		struct PickStruct
		{
			Vec4 mouseWorldPos{};
			uint32_t objectID{};
		}pickStr;
		struct PickUBO
		{
			Mat4 Projection;
			Mat4 View;
		}pickubo;

		struct IDReadCompute {
			Vec2 mouseRelativeCoord;
		};
	public:
		ObjectPick(std::shared_ptr<VulkanInstance> inst,Vec3 resolution);
		~ObjectPick();
		void Shutdown();
		void Update(VkCommandBuffer commandBuffer, uint32_t frameIndex, Vec2 mousePosition);


		ObjectPick(const ObjectPick&) = delete;
		ObjectPick& operator=(const ObjectPick&) = delete;


		void resize(Vec3 newSize);

		

		void beginRenderPass(VkCommandBuffer commandbuffer);
		void endRenderPass(VkCommandBuffer commandbuffer);

		uint32_t getActiveObject()
		{
			uint32_t selectedObjs = pickStr.objectID;
			return selectedObjs;
		}

		Vec4 getWorldPosition()
		{
			Vec4 position = reinterpret_cast<PickStruct*>(m_PickBuffer->Map())->mouseWorldPos;
			return position;
		}

	private:

		void CreatePickObj(std::shared_ptr<VulkanInstance> inst,Vec3 resolution);


		std::shared_ptr<VulkanPipeline> m_PickPipeline = nullptr;
		std::shared_ptr<VMABuffer> m_PickBuffer = nullptr;
		RenderPass* m_PickRenderPass{};
		RenderTarget* m_PickRenderTarget{};
		RenderTarget* m_PickDepthAttachment{};
		FrameBuffer* m_PickFrameBuffer{};
		//Synchronization m_ReadSync{ SYNCTYPE::TRANSFER2, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		
		//Synchronization m_DepthSync{ SYNCTYPE::GRAPHIC2COMPUTE,VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT };

	};

}

#endif // !OBJECT_PICK

