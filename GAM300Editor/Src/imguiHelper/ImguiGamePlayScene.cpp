#include "imguiHelper/ImguiGamePlayScene.h"
#include "Rendering/GraphicsManager.h"
#include "Rendering/RenderTarget.h"


TDS::GamePlayScene::GamePlayScene()
{
	flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse;
	panelTitle = "GamePlayScene";
	windowPadding = ImVec2(0.f, 0.f);

}

void TDS::GamePlayScene::init()
{

	m_GamePlayDesc = ImGui_ImplVulkan_AddTexture(GraphicsManager::getInstance().getFinalImage().getSampler(), GraphicsManager::getInstance().getFinalImage().getImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

void TDS::GamePlayScene::update()
{
	if (ImGui::BeginMenuBar())
	{
		if (isPlaying)
		{
			if (ImGui::BeginMenu("Game is Running..."))
			{
				ImGui::EndMenu();
			}
		}
		else
		{
			if (ImGui::BeginMenu("Game is Paused"))
			{
				ImGui::EndMenu();
			}
		}
		ImGui::EndMenuBar();
	}
	isFocus = ImGui::IsWindowFocused() && ImGui::IsItemVisible();
	ImVec2 vSize = ImGui::GetContentRegionAvail();


	ImGui::Image((ImTextureID)m_GamePlayDesc, vSize);

}

void TDS::GamePlayScene::Resize()
{
	if (m_GamePlayDesc)
	{
		ImGui_ImplVulkan_RemoveTexture(m_GamePlayDesc);
		m_GamePlayDesc = ImGui_ImplVulkan_AddTexture(GraphicsManager::getInstance().getFinalImage().getSampler(), GraphicsManager::getInstance().getFinalImage().getImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	}


}
