#include "imguiHelper/ImguiProfiler.h"
#include "vulkan/vulkan.h"

namespace TDS
{
	ImGui_ImplVulkan_InitInfo Profiler::m_vulkanInfo;
	Profiler::Profiler()
	{
		//selected = 0;
		//selectedFolder = -1;
		//renameCheck = false;

		flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse;
		panelTitle = "Profiler Window";
		windowPadding = ImVec2(0.f, 0.f);


		//insertEntities();
	}



	void Profiler::getVulkanInfo(ImGui_ImplVulkan_InitInfo& vulkanInfo)
	{
		m_vulkanInfo = vulkanInfo;
	}

	void Profiler::update()
	{
		vkGetPhysicalDeviceProperties(m_vulkanInfo.PhysicalDevice, &deviceProperties);
		ImGui::Text("Application: ", deviceProperties.deviceID);

		char* engineName = (char*)"TDS Engine";
		char* organizationName = (char*)"by Tear Drop Studio";

		ImVec4 yellow(1.0f, 0.8f, 0.0f, 1.0f); //We'll use yellow to print the results


		//GPU
		ImGui::Text("GPU: %s", deviceProperties.deviceName);


		//Brand
		uint32_t vulkanMajor = VK_VERSION_MAJOR(deviceProperties.apiVersion);
		uint32_t vulkanMinor = VK_VERSION_MINOR(deviceProperties.apiVersion);
		uint32_t vulkanPatch = VK_VERSION_PATCH(deviceProperties.apiVersion);

		ImGui::Text("API Version : %d.%d.%d", vulkanMajor, vulkanMinor, vulkanPatch);

		ImGui::Separator();

		//VRAM
		//float vram_budget = 0, vram_usage = 0, vram_available = 0, vram_reserved = 0;

		///*GetVramData(vram_budget, vram_usage, vram_available, vram_reserved);*/

		//ImGui::Text("VRAM Total: ");
		//ImGui::SameLine();
		//ImGui::TextColored(yellow, "%.1f Mb", vram_budget);

		ImGui::Text(engineName, 20, ImGuiInputTextFlags_ReadOnly);
		ImGui::Text(organizationName, 40, ImGuiInputTextFlags_ReadOnly);

		ImGui::Text("FPS: ");
		//FPS
		if (fps_log.size() > 100)
			fps_log.erase(fps_log.begin());
		fps_log.push_back(ImGui::GetIO().Framerate);
		char engine_name[25];
		sprintf_s(engine_name, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
		ImGui::PlotHistogram("##framerate", &fps_log[0], static_cast<int>(fps_log.size()), 0, engine_name, 0.0f, 100.0f, ImVec2(200, 70));

		//Milliseconds
		if (ms_log.size() > 100)
			ms_log.erase(ms_log.begin());
		ms_log.push_back(1 / (ImGui::GetIO().Framerate / 1000));
		sprintf_s(engine_name, 25, "Milliseconds %.1f", ms_log[ms_log.size() - 1]);
		ImGui::PlotHistogram("##framerate", &ms_log[0], static_cast<int>(ms_log.size()), 0, engine_name, 0.0f, 40.0f, ImVec2(200, 70));
	}

}