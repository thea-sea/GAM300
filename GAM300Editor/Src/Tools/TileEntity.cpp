/******
@author		Thea Sea
@brief		Tile Entity definitions
*/

#include "Tools/TileEntity.h"
#include "../Include/vulkanTools/VulkanPipeline.h"
#include <vulkan/vulkan.h>
	//typedef struct VkCommandBuffer_T* VkCommandBuffer;


namespace TDS
{

	TileEntity::TileEntity(const PrimitiveShape _primitiveShape) :
		m_Transform{},
		m_Shape(_primitiveShape),
		m_Color(Vec3(1.0f, 0.0f, 1.0f))
	{}

	TileEntity::~TileEntity()
	{}

	//void TileEntity::Draw(VkCommandBuffer _cmdBuffer, const uint32_t _numOfIndices) const
	//{
	//	vkCmdDrawIndexed(_cmdBuffer, _numOfIndices, 1, 0, 0, 0);
	//	//try rendering again in new rendering revamp
	//}



}

