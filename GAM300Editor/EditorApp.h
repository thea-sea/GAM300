#ifndef APPLICATION
#define APPLICATION

#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <chrono>

#include "windowswindow.h"
#include "vulkanTools/vulkanInstance.h"
#include "vulkanTools/Descriptor.h"
#include "vulkanTools/Renderer.h"
#include "vulkanTools/Buffer.h"
#include "vulkanTools/RendererManager.h"
#include "camera/camera.h"
#include "AssetManagement/AssetManager.h"
#include "vulkanTools/PointLightSystem.h"
#include "imguiHelper/ImguiHelper.h"
#include "dotnet/ImportExport.h"
#include "dotnet/include/coreclrhost.h" 
#include "Rendering/Skybox.h"

#include "Rendering/GridRenderer.h"
#include "GridManager/GridManager.h"
namespace TDS
{
	class Application
	{
	public:

		Application(HINSTANCE hinstance, int& nCmdShow, const wchar_t* classname, WNDPROC wndproc);
		~Application();

		void   Initialize();
		void   Update();
		void   Render() {}
		void   Exit() {}
		/*!*************************************************************************
		* Function for initializing script engine
		***************************************************************************/
		void   Run();

		void handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		static void SetWindowHandle(HWND hWnd);
		static HWND GetWindowHandle();

	private:
		bool initImgui();
		Application(const Application&);

	//Rendering for ImGui
	private:
		VkPipelineCache  m_PipelineCache = nullptr;
		VkDescriptorPool m_ImGuiDescPool = nullptr;

	private:
		WindowsWin						 m_window;
		bool							 m_isRunning{ true };
		static inline HWND				 m_handler;

		std::chrono::time_point<std::chrono::high_resolution_clock>Clock{};
		//std::shared_ptr <VulkanInstance> m_pVKInst;

		//std::shared_ptr<Renderer>		 m_Renderer/*{ m_window, * m_pVKInst.get() }*/;
		TDSCamera m_camera{ -90.0f ,0.f };
		TDSCamera m_GameCamera{ -90.0f, 0.f, 0.1f, 100.f, {0.0f,0.f, 0.f} };
		//if u want to see top-down for pathfinding, can uncomment line below:
		//TDSCamera m_GameCamera{ -90.0f, -89.f, 0.1f, 100.f, {0.0f,2500.f, 50.f} };
		//std::shared_ptr<Model> models;
		//VkSampler sampling;
		struct UniformBufferObject
		{
			Mat4 model;
			Mat4 view;
			Mat4 proj;
		};
		
		SkyBoxRenderer skyboxrender{};
		//commented out as grid cannot render:
		//GridRenderer gridrender{};
		//GridManagerBase gridmanager{};
	private:

		/*!*************************************************************************
		* Helper functions to get CoreClr pointers
		***************************************************************************/
		template<typename FunctType>
		FunctType getCoreClrFuncPtr(const std::string& functionName)
		{
			auto fPtr = reinterpret_cast<FunctType>(GetProcAddress(coreClr, functionName.c_str()));
			if (!fPtr)
				throw std::runtime_error("Unable to get pointer to function.");
			return fPtr;
		}

		/*!*************************************************************************
		* Helper functions to get function pointers to C++/CLI functions
		***************************************************************************/
		template<typename FunctionType>
		FunctionType GetFunctionPtr(const std::string_view& assemblyName, const std::string_view& typeName, const std::string_view& functionName)
		{
			FunctionType managedDelegate = nullptr;
			int result = createManagedDelegate
			(
				hostHandle,
				domainId,
				assemblyName.data(),
				typeName.data(),
				functionName.data(),
				reinterpret_cast<void**>(&managedDelegate)
			);
			// Check if it failed
			if (result < 0)
			{
				std::ostringstream oss;
				oss << std::hex << std::setfill('0') << std::setw(8)
					<< "[DotNetRuntime] Failed to get pointer to function \""
					<< typeName << "." << functionName << "\" in assembly (" << assemblyName << "). "
					<< "Error 0x" << result << "\n";
				throw std::runtime_error(oss.str());
			}
			return managedDelegate;
		}

	private:
		/*!*************************************************************************
		* Function in initialize script engine
		***************************************************************************/
		void startScriptEngine();
		/*!*************************************************************************
		* Function to stop script engine
		***************************************************************************/
		void stopScriptEngine();
		/*!*************************************************************************
		* Function to build TPA list for C++/CLI to function
		***************************************************************************/
		std::string buildTpaList(const std::string& directory);
		void buildManagedScriptCsProj();
		void compileScriptAssembly();
		std::string getDotNetRuntimePath() const;
		/*!*************************************************************************
		* References to CoreCLR key components
		***************************************************************************/
		HMODULE coreClr = nullptr;
		void* hostHandle = nullptr;
		unsigned int domainId = 0;
		/*!*************************************************************************
		* Function Pointers to CoreCLR functions
		***************************************************************************/
		coreclr_initialize_ptr      initializeCoreClr = nullptr;
		coreclr_create_delegate_ptr createManagedDelegate = nullptr;
		coreclr_shutdown_ptr        shutdownCoreClr = nullptr;


};//class application



}//namespace TDS


#endif // !APPLICATION

