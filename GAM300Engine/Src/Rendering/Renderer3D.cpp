#include "Rendering/Renderer3D.h"
#include "vulkanTools/DirectFrameBuffer.h"
#include "Rendering/GraphicsManager.h"
#include "vulkanTools/VulkanPipeline.h"
#include "Rendering/RenderDataManager.h"
#include "Identifier/Identifier.h"
#include "vulkanTools/vulkanSwapChain.h"
#include "GraphicsResource/VertexInfo.h"
#include "vulkanTools/GlobalBufferPool.h"
#include "vulkanTools/Model.h"
#include "vulkanTools/FrameInfo.h"
namespace TDS
{
	std::shared_ptr<Renderer3D> Renderer3D::getInstance()
	{
		if (m_Instance == nullptr)
			m_Instance = std::make_shared<Renderer3D>();
		return m_Instance;
	}
	void Renderer3D::Init()
	{
		Renderer3D& inst = *getInstance();

		/*inst.m_FrameBuffer = GraphicsManager::getInstance().GetMainFrameBuffer();*/
		PipelineCreateEntry entry{};
		/*entry.m_FBTarget = { inst.m_FrameBuffer };*/
		entry.m_NumDescriptorSets = 1;
		
		entry.m_ShaderInputs.m_Shaders.insert(std::make_pair(SHADER_FLAG::VERTEX, "../assets/shaders/shadervert.spv"));
		entry.m_ShaderInputs.m_Shaders.insert(std::make_pair(SHADER_FLAG::FRAGMENT, "../assets/shaders/shaderfrag.spv"));
		VertexLayout layout = 
		VertexLayout(
		{ 
		  VertexBufferElement(VAR_TYPE::VEC3, "vPosition"),
		  VertexBufferElement(VAR_TYPE::VEC3, "vColor"),
		  VertexBufferElement(VAR_TYPE::VEC2, "inTexCoord"),
		  VertexBufferElement(VAR_TYPE::VEC4, "vNormals"),
		  VertexBufferElement(VAR_TYPE::Float, "vFloat")
		});
		GlobalBufferPool::GetInstance()->AddToGlobalPool(sizeof(GlobalUBO), 0, VkBufferUsageFlagBits::VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, "PL");
		entry.m_ShaderInputs.m_InputVertex.push_back(VertexBufferInfo(false, layout, sizeof(VertexData)));
		inst.m_DefaultPipeline = std::make_shared<VulkanPipeline>();

		inst.m_DefaultPipeline->Create(entry);
		

	}
	std::shared_ptr<VulkanPipeline>& Renderer3D::getPipeline()
	{
		return m_Instance->m_DefaultPipeline;
	}

	void Renderer3D::ShutDown()
	{
		m_DefaultPipeline->ShutDown();
	}
}