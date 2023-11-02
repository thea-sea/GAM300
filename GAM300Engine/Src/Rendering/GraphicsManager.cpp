#include "Rendering/GraphicsManager.h"
#include "vulkanTools/Renderer.h"
#include "vulkanTools/vulkanInstance.h"
#include "vulkanTools/CommandManager.h"
#include "vulkanTools/DirectFrameBuffer.h"
#include "Shader/ShaderLoader.h"
#include "Rendering/Renderer3D.h"
#include "vulkanTools/GlobalBufferPool.h"
#include "vulkanTools/VulkanTexture.h"
#include "vulkanTools/GlobalBufferPool.h"
#include "Rendering/RenderLayer.h"
#include "Rendering/Renderer3D.h"
#include "Rendering/RenderDataManager.h"
#include "Rendering/RenderTarget.h"
#include "Rendering/renderPass.h"
#include "vulkanTools/FrameBuffer.h"
namespace TDS
{
	GraphicsManager::GraphicsManager()
	{
	}
	GraphicsManager::~GraphicsManager()
	{
	}
	void GraphicsManager::Init(WindowsWin* window)
	{
		m_pWindow = window;
		m_MainVkContext = std::make_shared<VulkanInstance>(*m_pWindow);
		m_CommandManager = std::make_shared<CommandManager>();
		ShaderLoader::GetInstance()->DeserializeShaderReflection(REFLECTED_BIN);
		GraphicsAllocator::GetInstance().Init(*m_MainVkContext);
		m_CommandManager->Init();
		m_SwapchainRenderer = std::make_shared<Renderer>(*m_pWindow, *m_MainVkContext);
		DefaultTextures::GetInstance().Init();
	
		Vec3 size = { static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()), 1.f };
		std::vector<AttachmentInfo> attachmentInfos{};
		std::vector<RenderTarget*> attachments{};
		RenderTargetCI rendertargetCI{
			VK_FORMAT_R8G8B8A8_UNORM,
			VK_IMAGE_ASPECT_COLOR_BIT,
			VK_IMAGE_VIEW_TYPE_2D,
			size,
			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			RenderTargetType::COLOR,
			false,
			VK_SAMPLE_COUNT_1_BIT
		};
	
		m_RenderingAttachment = new RenderTarget(m_MainVkContext, rendertargetCI);
		attachmentInfos.push_back({ m_RenderingAttachment, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE });
		attachments.push_back(m_RenderingAttachment);
		
		m_Renderpass = new RenderPass(m_MainVkContext->getVkLogicalDevice(), attachmentInfos);
		m_Framebuffer = new FrameBuffer(m_MainVkContext->getVkLogicalDevice(), m_Renderpass->getRenderPass(), attachments);
		
		Renderer3D::Init();
		m_PointLightRenderer = std::make_unique<PointLightSystem>(*m_MainVkContext);
		for (auto& renderLayer : m_RenderLayer)
		{
			renderLayer->Setup(m_pWindow);
			renderLayer->Init();
		}


	}
	void GraphicsManager::StartFrame()
	{
		for (auto& renderLayer : m_RenderLayer)
		{
			renderLayer->StartFrame();
		}
		currentCommand = m_SwapchainRenderer->BeginFrame();

	}

	void GraphicsManager::AddRenderLayer(RenderLayer* layer)
	{
		m_RenderLayer.push_back(layer);
	}

	void GraphicsManager::EndFrame()
	{
		for (auto& renderLayer : m_RenderLayer)
		{
			renderLayer->StartFrame();
			renderLayer->Render();
		}
		m_SwapchainRenderer->EndFrame();


	}
	void GraphicsManager::ShutDown()
	{

		vkDeviceWaitIdle(m_MainVkContext->getVkLogicalDevice());

		for (auto& renderlayer : m_RenderLayer)
		{
			renderlayer->ShutDown();
		}
		Renderer3D::getInstance()->ShutDown();
		GlobalBufferPool::GetInstance()->Destroy();
		m_RenderingAttachment->~RenderTarget();
		m_Renderpass->~RenderPass();
		m_Framebuffer->~FrameBuffer();
		DefaultTextures::GetInstance().DestroyDefaultTextures();
		m_SwapchainRenderer->ShutDown();
		m_CommandManager->Shutdown();
		RendererDataManager::Destroy();
		GraphicsAllocator::GetInstance().ShutDown();

		m_MainVkContext->ShutDown();
	}
	void GraphicsManager::ResizeFrameBuffer(std::uint32_t width, std::uint32_t height)
	{
	}
	void GraphicsManager::setCamera(TDSCamera& camera)
	{
		m_Camera = &camera;
	}

	TDSCamera& GraphicsManager::GetCamera()
	{
		return *m_Camera;
	}
	//std::shared_ptr<DirectFrameBuffer> GraphicsManager::GetMainFrameBuffer()
	//{
	//	return m_MainFrameBuffer;
	//}
	VkCommandBuffer& GraphicsManager::getCommandBuffer()
	{
		return currentCommand;
	}
	Renderer& GraphicsManager::GetSwapchainRenderer()
	{
		return *m_SwapchainRenderer;
	}
	CommandManager& GraphicsManager::getCommandManager()
	{
		return *m_CommandManager;
	}
	VulkanInstance& GraphicsManager::getVkInstance()
	{
		return *m_MainVkContext;
	}
	GraphicsManager& GraphicsManager::getInstance()
	{
		if (m_Instance == nullptr)
		{
			m_Instance = std::make_shared<GraphicsManager>();
		}
		return *m_Instance;
	}
}