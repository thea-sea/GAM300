#pragma once
#include <pch.h>
#include "dotnet/ImportExport.h"
#include "vulkan/vulkan.h"
#include "camera/camera.h"
#include "vulkanTools/PointLightSystem.h"
#include "camera/Camerasystem/CameraSystem.h"
#include "DebugRenderer.h"
#include "Rendering/ObjectPicking.h"
namespace TDS
{

	class WindowsWin;
	class VulkanInstance;
	class Renderer;
	class CommandManager;
	class RenderLayer;
	class RenderTarget;
	class RenderPass;
	class FrameBuffer;
	class PointLightSystem;
	class DLL_API GraphicsManager
	{
	private:
		std::shared_ptr<VulkanInstance>			m_MainVkContext;
		std::shared_ptr<Renderer>				m_SwapchainRenderer;
		std::shared_ptr<CommandManager>			m_CommandManager;
		std::shared_ptr<ObjectPick>				m_ObjectPicking = nullptr;
		std::vector<RenderLayer*>				m_RenderLayer;



		TDSCamera*								m_Camera = nullptr;
		VkCommandBuffer							currentCommand = nullptr;
		WindowsWin*								m_pWindow = nullptr;
		RenderTarget*							m_RenderingAttachment{ nullptr };
		RenderTarget*							m_RenderingDepthAttachment{ nullptr };
		RenderTarget*							m_PickAttachment{ nullptr };
		RenderPass*								m_Renderpass{ nullptr };
		FrameBuffer*							m_Framebuffer{ nullptr };
		int										m_LayerID = 0;
		bool									m_RenderAllLayer = true;
		bool									m_ViewingFrom2D = false;
		bool									m_FrameHasBegin = false;

		float									m_TabOffset{};
		Vec4									m_ViewportScreen{};
	public:
		inline static std::shared_ptr<GraphicsManager> m_Instance;
		GraphicsManager();
		~GraphicsManager();

		void								ToggleViewFrom2D(bool condition);
		bool								IsViewingFrom2D();
		inline bool							IfFrameHasBegin()
		{
			return m_FrameHasBegin;
		}
		void								AddRenderLayer(RenderLayer* layer);
		void								Init(WindowsWin* window);
		void								StartFrame();
		void								EndFrame();
		void								ShutDown();
		void								ResizeFrameBuffer(std::uint32_t width, std::uint32_t height);
		void								setCamera(TDSCamera& camera);
		std::shared_ptr<VulkanInstance>		getVkInstancePtr();

		WindowsWin*							GetWindow();
		
		void								SetLayerToRender(int ID);
		bool								RenderAllLayer();
		std::uint32_t						PickedObject();
		void								ToggleRenderAllLayer(bool condition);
		int									LayerToRender();
		

		TDSCamera& GetCamera();

		VkCommandBuffer& getCommandBuffer();
		Renderer& GetSwapchainRenderer();
		VulkanInstance& getVkInstance();
		CommandManager& getCommandManager();
		static GraphicsManager& getInstance();
		RenderTarget& getFinalImage() { return *m_RenderingAttachment; }
		RenderTarget& getPickImage() { return *m_PickAttachment; }
		RenderTarget& getFinalDepthAttachment() { return *m_RenderingDepthAttachment; }
		FrameBuffer& getFrameBuffer() { return *m_Framebuffer; }
		RenderPass& getRenderPass() { return *m_Renderpass; }
		std::unique_ptr<PointLightSystem> m_PointLightRenderer;
		std::unique_ptr<DebugRenderer> m_DebugRenderer;
		ObjectPick& getObjectPicker();


		WindowsWin* getWindow() const { return m_pWindow;}
		Vec4& getViewportScreen() { return m_ViewportScreen; }
		float& getOffset() { return m_TabOffset; }
	};


}