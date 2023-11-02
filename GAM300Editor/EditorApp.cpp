// Application.cpp
#include <shlwapi.h>                // GetModuleFileNameA(), PathRemoveFileSpecA()
#pragma comment(lib, "shlwapi.lib") // Needed for <shlwapi.h>
#include <vector>
#include <array>
#include <sstream>
#include <imgui/imgui.h>

#include "EditorApp.h"

#include "Logger/Logger.h"
#include "Input/Input.h"
#include "vulkanTools/Model.h"
#include "imguiHelper/ImguiHelper.h"
#include "vulkanTools/FrameInfo.h"
#include "Timestep/Timestep.h"
#include "sceneManager/sceneManager.h"
#include "Tools/ShaderReflector.h"
#include "Shader/ShaderLoader.h"
#include "Rendering/GraphicsManager.h"
#include "components/components.h"
#include "Rendering/RendererSystem.h"
#include "vulkanTools/VulkanHelper.h"
#include "EditorRenderer/ImguiLayer.h"
#include "vulkanTools/CommandManager.h"
#include "GraphicsResource/TextureInfo.h"
#include "vulkanTools/VulkanTexture.h"
#include "Rendering/renderPass.h"
#include "vulkanTools/FrameBuffer.h"
#include "Tools/DDSConverter.h"
#include "imguiHelper/ImguiScene.h"
bool isPlaying = false;

namespace TDS
{
    Application::Application(HINSTANCE hinstance, int& nCmdShow, const wchar_t* classname, WNDPROC wndproc)
        :m_window(hinstance, nCmdShow, classname)
    {
        m_window.createWindow(wndproc, 1280, 800);

        //m_pVKInst = std::make_shared<VulkanInstance>(m_window);
        //m_Renderer = std::make_shared<Renderer>(m_window, *m_pVKInst.get());
        Log::Init();
        TDS_INFO("window width: {}, window height: {}", m_window.getWidth(), m_window.getHeight());

       /* models = Model::createModelFromFile(*m_pVKInst.get(), "Test.bin");*/
    }
    void  Application::handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        IMGUI_WIN32_WNDPROCHANDLER_FORWARD_DECLARATION;
        ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam); //for imgui implementation
        //can extern  some imgui wndproc handler | tbc

        switch (uMsg)
        {

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_PAINT:
            ValidateRect(m_window.getWindowHandler(), NULL);
            break;
        case WM_SIZE: //for resize of window may need it for fullscreen?
            m_window.setWidth(LOWORD(lParam));
            m_window.setHeight(HIWORD(lParam));
            m_window.WindowIsResizing(true);
            break;
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_XBUTTONDOWN:
        case WM_XBUTTONUP:
        {
            Input::processMouseInput(wParam, lParam);
        }break;

        case WM_MOUSEMOVE:
        {
            Input::updateMousePosition(lParam);
        }break;

        case WM_MOUSEWHEEL:
        {
            Input::processMouseScroll(wParam);
        }break;

        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        {
            uint32_t VKcode = static_cast<uint32_t>(wParam);
            WORD keyflags = HIWORD(lParam);
            if (!(keyflags & KF_REPEAT))
            {
                Input::keystatus = Input::KeyStatus::PRESSED;
            }
            else if (keyflags & KF_REPEAT)
                Input::keystatus = Input::KeyStatus::REPEATED;

            bool wasDown = (lParam & (1 << 30)) != 0;
            bool isDown = (static_cast<unsigned int>(lParam) & (1 << 31)) == 0;
            Input::processKeyboardInput(VKcode, wasDown, isDown);
        }break;
        case WM_KEYUP:
        {
            uint32_t VKcode = static_cast<uint32_t>(wParam);
            bool wasDown = (lParam & (1 << 30)) != 0;
            bool isDown = (static_cast<unsigned int>(lParam) & (1 << 31)) == 0;

            Input::processKeyboardInput(VKcode, wasDown, isDown);
            Input::keystatus = Input::KeyStatus::RELEASED;
            Input::keystatus = Input::KeyStatus::IDLE;
        }break;
        }
    }
    void Application::Initialize()
    {

        ShaderReflector::GetInstance()->Init(SHADER_DIRECTORY, REFLECTED_BIN);
        GraphicsManager::getInstance().Init(&m_window);
        AssetManager::GetInstance()->Init();
        AssetManager::GetInstance()->PreloadAssets();
        //Run();
    }

    void Application::Update()
    {
        DDSConverter::Init();
        auto executeUpdate = GetFunctionPtr<void(*)(void)>
            (
                "ScriptAPI",
                "ScriptAPI.EngineInterface",
                "ExecuteUpdate"
            );

        auto reloadScripts = GetFunctionPtr<void(*)(void)>
            (
                "ScriptAPI",
                "ScriptAPI.EngineInterface",
                "Reload"
            );

        auto addScript = GetFunctionPtr<bool(*)(int, const char*)>
            (
                "ScriptAPI",
                "ScriptAPI.EngineInterface",
                "AddScriptViaName"
            );
        auto toggleScript = GetFunctionPtr<bool(*)(int, const char*)>
            (
                "ScriptAPI",
                "ScriptAPI.EngineInterface",
                "ToggleScriptViaName"
            );

        initImgui();
        float lightx = 0.f;
    
      /*  Texture data{};
        data.LoadTexture("../../assets/textures/texture.dds");
        VulkanTexture vkTexture{};
        vkTexture.CreateBasicTexture(data.m_TextureInfo);
        
        vkTexture.m_DescSet = ImGui_ImplVulkan_AddTexture(vkTexture.getInfo().sampler, vkTexture.getInfo().imageView, vkTexture.getInfo().imageLayout);
       */

        VkDescriptorSet  m_DescSet{};
        GraphicsManager::getInstance().setCamera(m_camera);
        while (m_window.processInputEvent())
        {

            TimeStep::CalculateDeltaTime();
            float DeltaTime = TimeStep::GetDeltaTime();
            

            
            m_camera.UpdateCamera(DeltaTime);
            lightx = lightx < -1.f ? 1.f : lightx - 0.005f;
            RendererSystem::lightPosX = lightx;

            Vec3 m_windowdimension{ static_cast<float>(m_window.getWidth()), static_cast<float>(m_window.getHeight()), 1.f };
            if (GraphicsManager::getInstance().getFrameBuffer().getDimensions() != m_windowdimension)
            {
                GraphicsManager::getInstance().getFrameBuffer().resize(m_windowdimension, GraphicsManager::getInstance().getRenderPass().getRenderPass());
                std::shared_ptr<EditorScene> pScene = static_pointer_cast<EditorScene>(LevelEditorManager::GetInstance()->panels[SCENE]);
                pScene->Resize();
            }
            GraphicsManager::getInstance().StartFrame();
            VkCommandBuffer commandBuffer = GraphicsManager::getInstance().getCommandBuffer();
             GraphicsManager::getInstance().getRenderPass().beginRenderPass(commandBuffer, &GraphicsManager::getInstance().getFrameBuffer());

            if (isPlaying)
            {
                ecs.runSystems(1, DeltaTime);
            }

            ecs.runSystems(2, DeltaTime);
         
        
            
            imguiHelper::Update();
            GraphicsManager::getInstance().getRenderPass().endRenderPass(commandBuffer);
            GraphicsManager::getInstance().GetSwapchainRenderer().BeginSwapChainRenderPass(commandBuffer);


            imguiHelper::Draw(commandBuffer);

            GraphicsManager::getInstance().GetSwapchainRenderer().EndSwapChainRenderPass(commandBuffer);
            GraphicsManager::getInstance().EndFrame();
            
            // Reloading
            if (GetKeyState(VK_F5) & 0x8000)
            {
                compileScriptAssembly();
                SceneManager::GetInstance()->saveCurrentScene();
                reloadScripts();
                SceneManager::GetInstance()->loadScene(SceneManager::GetInstance()->getCurrentScene());
                //addScript(1, "Test");
            }

            if (GetKeyState(VK_SPACE) & 0x8000)
            {
                toggleScript(1, "Test");
            }
            executeUpdate();
            Input::scrollStop();
        }
        stopScriptEngine();
        /*vkDeviceWaitIdle(m_pVKInst.get()->getVkLogicalDevice());*/

        AssetManager::GetInstance()->ShutDown();
        vkDeviceWaitIdle(GraphicsManager::getInstance().getVkInstance().getVkLogicalDevice());
        if (m_ImGuiDescPool)
        {
            vkDestroyDescriptorPool(GraphicsManager::getInstance().getVkInstance().getVkLogicalDevice(), m_ImGuiDescPool, 0);
            m_ImGuiDescPool = nullptr;
        }
        imguiHelper::Exit();
        ecs.destroy();
        GraphicsManager::getInstance().ShutDown();
        DDSConverter::Destroy();
    }

    void Application::Run()
    {
        startScriptEngine();
        compileScriptAssembly();

        // Step 1: Get Functions
        auto init = GetFunctionPtr<void(*)(void)>
            (
                "ScriptAPI",
                "ScriptAPI.EngineInterface",
                "Init"
            );

        auto reloadScripts = GetFunctionPtr<void(*)(void)>
            (
                "ScriptAPI",
                "ScriptAPI.EngineInterface",
                "Reload"
            );


        SceneManager::GetInstance()->addScript = GetFunctionPtr<bool(*)(EntityID, std::string)>
            (
                "ScriptAPI",
                "ScriptAPI.EngineInterface",
                "AddScriptViaName"
            );

        auto awake = GetFunctionPtr<void(*)(void)>
            (
                "ScriptAPI",
                "ScriptAPI.EngineInterface",
                "ExecuteAwake"
            );

        // Step 2: Initialize
        init();

        SceneManager::GetInstance()->getScriptVariables = GetFunctionPtr<std::vector<ScriptValues>(*)(EntityID, std::string)>
            (
                "ScriptAPI",
                "ScriptAPI.EngineInterface",
                "GetScriptVariables"
            );

        SceneManager::GetInstance()->hasScript = GetFunctionPtr<bool(*)(EntityID, std::string)>
            (
                "ScriptAPI",
                "ScriptAPI.EngineInterface",
                "HasScriptViaName"
            );

        SceneManager::GetInstance()->getAllScripts = GetFunctionPtr<std::vector<std::string>(*)()>
            (
                "ScriptAPI",
                "ScriptAPI.EngineInterface",
                "GetAllScripts"
            );

        ecs.addScriptList = GetFunctionPtr<void(*)(EntityID)>
            (
                "ScriptAPI",
                "ScriptAPI.EngineInterface",
                "AddScriptList"
            );

        ecs.removeScriptList = GetFunctionPtr<void(*)(EntityID)>
            (
                "ScriptAPI",
                "ScriptAPI.EngineInterface",
                "RemoveEntity"
            );

        SceneManager::GetInstance()->setBool = GetFunctionPtr<void(*)(EntityID, std::string, std::string, bool)>
            (
                "ScriptAPI",
                "ScriptAPI.EngineInterface",
                "SetValueBool"
            );

        SceneManager::GetInstance()->setInt = GetFunctionPtr<void(*)(EntityID, std::string, std::string, int)>
            (
                "ScriptAPI",
                "ScriptAPI.EngineInterface",
                "SetValueInt"
            );

        SceneManager::GetInstance()->setDouble = GetFunctionPtr<void(*)(EntityID, std::string, std::string, double)>
            (
                "ScriptAPI",
                "ScriptAPI.EngineInterface",
                "SetValueDouble"
            );

        SceneManager::GetInstance()->setFloat = GetFunctionPtr<void(*)(EntityID, std::string, std::string, float)>
            (
                "ScriptAPI",
                "ScriptAPI.EngineInterface",
                "SetValueFloat"
            );

        SceneManager::GetInstance()->setString = GetFunctionPtr<void(*)(EntityID, std::string, std::string, std::string)>
            (
                "ScriptAPI",
                "ScriptAPI.EngineInterface",
                "SetValueString"
            );

        //SceneManager::GetInstance()->setChar = GetFunctionPtr<void(*)(EntityID, std::string, std::string, char)>
        //    (
        //        "ScriptAPI",
        //        "ScriptAPI.EngineInterface",
        //        "SetValueChar"
        //    );

        SceneManager::GetInstance()->setGameObject = GetFunctionPtr<void(*)(EntityID, std::string, std::string, EntityID)>
            (
                "ScriptAPI",
                "ScriptAPI.EngineInterface",
                "SetGameObject"
            );

        SceneManager::GetInstance()->setScriptReference = GetFunctionPtr<void(*)(EntityID, std::string, std::string, EntityID, std::string)>
            (
                "ScriptAPI",
                "ScriptAPI.EngineInterface",
                "SetScript"
            );

        SceneManager::GetInstance()->updateName = GetFunctionPtr<bool(*)(EntityID, std::string)>
            (
                "ScriptAPI",
                "ScriptAPI.EngineInterface",
                "UpdateGameObjectName"
            );

        SceneManager::GetInstance()->Init();
        ecs.initializeSystems(1);
        ecs.initializeSystems(2);

        awake();
    }

    Application::~Application()
    {
        m_window.~WindowsWin();
    }

    void Application::startScriptEngine()
    {
        // Step 1: Loading the coreclr.dll
        // Get the current executable directory so that we can find the coreclr.dll to load
        std::string runtimePath(MAX_PATH, '\0');
        GetModuleFileNameA(nullptr, runtimePath.data(), MAX_PATH);
        PathRemoveFileSpecA(runtimePath.data());
        // Since PathRemoveFileSpecA() removes from data(), the size is not updated, so we must manually update it
        runtimePath.resize(std::strlen(runtimePath.data()));

        std::filesystem::current_path(runtimePath);

        // Construct the CoreCLR path
        std::string coreClrPath(runtimePath); // Works
        coreClrPath += "\\coreclr.dll";

        // Load the CoreCLR DLL
        coreClr = LoadLibraryExA(coreClrPath.c_str(), nullptr, 0);
        if (!coreClr)
            throw std::runtime_error("Failed to load CoreCLR.");

        // Step 2: Get CoreCLR hosting functions
        initializeCoreClr = getCoreClrFuncPtr<coreclr_initialize_ptr>("coreclr_initialize");
        createManagedDelegate = getCoreClrFuncPtr<coreclr_create_delegate_ptr>("coreclr_create_delegate");
        shutdownCoreClr = getCoreClrFuncPtr<coreclr_shutdown_ptr>("coreclr_shutdown");

        // Step 3: Construct AppDomain properties used when starting the runtime
        std::string tpaList = buildTpaList(runtimePath);
        // Define CoreCLR properties
        std::array propertyKeys =
        {
            "TRUSTED_PLATFORM_ASSEMBLIES",      // Trusted assemblies (like the GAC)
            "APP_PATHS",                        // Directories to probe for application assemblies
        };
        std::array propertyValues =
        {
            tpaList.c_str(),
            runtimePath.c_str()
        };

        // Step 4: Start the CoreCLR runtime
        int result = initializeCoreClr
        (
            runtimePath.c_str(),     // AppDomain base path
            "SampleHost",            // AppDomain friendly name, this can be anything you want really
            static_cast<int>(propertyKeys.size()),     // Property count
            propertyKeys.data(),     // Property names
            propertyValues.data(),   // Property values
            &hostHandle,             // Host handle
            &domainId                // AppDomain ID
        );
        // Check if intiialization of CoreCLR failed
        if (result < 0)
        {
            std::ostringstream oss;
            oss << std::hex << std::setfill('0') << std::setw(8)
                << "Failed to initialize CoreCLR. Error 0x" << result << "\n";
            throw std::runtime_error(oss.str());
        }
    }

    void Application::stopScriptEngine()
    {
        // Shutdown CoreCLR
        const int RESULT = shutdownCoreClr(hostHandle, domainId);
        if (RESULT < 0)
        {
            std::stringstream oss;
            oss << std::hex << std::setfill('0') << std::setw(8)
                << "Failed to shut down CoreCLR. Error 0x" << RESULT << "\n";
            throw std::runtime_error(oss.str());
        }
    }

    std::string Application::buildTpaList(const std::string& directory)
    {
        // Constants
        static const std::string SEARCH_PATH = directory + "\\*.dll";
        static constexpr char PATH_DELIMITER = ';';
        // Create a osstream object to compile the string
        std::ostringstream tpaList;
        // Search the current directory for the TPAs (.DLLs)
        WIN32_FIND_DATAA findData;
        HANDLE fileHandle = FindFirstFileA(SEARCH_PATH.c_str(), &findData);
        if (fileHandle != INVALID_HANDLE_VALUE)
        {
            do
            {
                // Append the assembly to the list
                tpaList << directory << '\\' << findData.cFileName << PATH_DELIMITER;
            } while (FindNextFileA(fileHandle, &findData));
            FindClose(fileHandle);
        }
        return tpaList.str();
    }
    void Application::compileScriptAssembly()
    {
        //relative path to the script assembly project file
        const char* PROJ_PATH =
            "..\\..\\ManagedScripts\\ManagedScripts.csproj";

        std::wstring buildCmd = L" build \"" +
            std::filesystem::absolute(PROJ_PATH).wstring() +
            L"\" -c Debug --no-self-contained " +
            L"-o \"..\\..\\scriptDLL/\" -r \"win-x64\"";

        // Define the struct to config the compiler process call
        STARTUPINFOW startInfo;
        PROCESS_INFORMATION pi;
        ZeroMemory(&startInfo, sizeof(startInfo));
        ZeroMemory(&pi, sizeof(pi));
        startInfo.cb = sizeof(startInfo);

        // Start compiler process
        const auto SUCCESS = CreateProcess
        (
            L"C:\\Program Files\\dotnet\\dotnet.exe", buildCmd.data(),
            nullptr, nullptr, true, NULL, nullptr, nullptr,
            &startInfo, &pi
        );

        // Check that we launched the process
        if (!SUCCESS)
        {
            auto err = GetLastError();
            std::ostringstream oss;
            oss << "Failed to launch compiler. Error code: "
                << std::hex << err;
            throw std::runtime_error(oss.str());
        }

        // Wait for process to end
        DWORD exitCode{};
        while (true)
        {
            const auto EXEC_SUCCESS =
                GetExitCodeProcess(pi.hProcess, &exitCode);
            if (!EXEC_SUCCESS)
            {
                auto err = GetLastError();
                std::ostringstream oss;
                oss << "Failed to query process. Error code: "
                    << std::hex << err;
                throw std::runtime_error(oss.str());
            }
            if (exitCode != STILL_ACTIVE)
                break;
        }

        // Successful build
        if (exitCode == 0)
        {
            // Copy out files
            std::filesystem::copy_file
            (
                "..\\..\\scriptDLL\\ManagedScripts.dll",
                "ManagedScripts.dll",
                std::filesystem::copy_options::overwrite_existing
            );
        }
        // Failed build
        else
        {
            throw std::runtime_error("Failed to build managed scripts!");
        }
    }

    bool Application::initImgui()
    {
        VkDescriptorPoolSize pool_sizes[] =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };

        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
        pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;

        vkCreateDescriptorPool(GraphicsManager::getInstance().getVkInstance().getVkLogicalDevice(), &pool_info, nullptr
            , &m_ImGuiDescPool);

        ImGui_ImplVulkan_InitInfo initInfo{};

        initInfo.Instance = GraphicsManager::getInstance().getVkInstance().getInstance();
        initInfo.PhysicalDevice = GraphicsManager::getInstance().getVkInstance().getVkPhysicalDevice();
        initInfo.Device = GraphicsManager::getInstance().getVkInstance().getVkLogicalDevice();
        initInfo.QueueFamily = GraphicsManager::getInstance().getVkInstance().GetGraphicsQueueIndex();
        initInfo.Queue = GraphicsManager::getInstance().getVkInstance().getGraphicsQueue();
        initInfo.PipelineCache = VK_NULL_HANDLE;


        initInfo.DescriptorPool = m_ImGuiDescPool;
        initInfo.Subpass = 0;
        initInfo.MinImageCount = 2;
        initInfo.ImageCount = 2;
        initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        initInfo.Allocator = nullptr;
        initInfo.CheckVkResultFn = nullptr;

        imguiHelper::InitializeImgui(initInfo, GraphicsManager::getInstance().GetSwapchainRenderer().getSwapChainRenderPass(), m_window.getWindowHandler());

        if (VkCommandBuffer FCB{ GraphicsManager::getInstance().getVkInstance().beginSingleTimeCommands() }; FCB != nullptr)
        {
            imguiHelper::ImguiCreateFont(FCB);
            GraphicsManager::getInstance().getVkInstance().endSingleTimeCommands(FCB);
            ImGui_ImplVulkan_DestroyFontUploadObjects();
        }
        else
        {
            std::cerr << "failed to create command buffer for imgui font creation\n";
            return false;
        }

       
        return true;

    }

}// end TDS