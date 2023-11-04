/*!*************************************************************************
****
\file ImguiScriptBrowser.cpp
\author Go Ruo Yan
\par DP email: ruoyan.go@digipen.edu
\date 20-10-2023
\brief  This program defines the functions in the Level Editor Script
		Browser panel
****************************************************************************
***/

#include "imguiHelper/ImguiScriptBrowser.h"
#include "imguiHelper/ImguiHierarchy.h"


namespace TDS
{
	/*!*************************************************************************
	This function initializes the Scene Broswer panel
	****************************************************************************/
	ScriptBrowser::ScriptBrowser()
	{
		flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse;
		panelTitle = "Script Browser";
		windowPadding = ImVec2(10.f, 10.f);

		buttonSize = 60.f;
		deleteSceneConfirmation = false;
		renameFileID = -1;
		renameFileOldName = "";

	}

	/*!*************************************************************************
	This function is the update function for Scene Broswer panel
	****************************************************************************/
	void ScriptBrowser::update()
	{
		auto& sceneManager = SceneManager::GetInstance();

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New"))
				{
					std::filesystem::path filePath = std::filesystem::path(sceneManager->getScenePath()).parent_path().parent_path().parent_path().string() + std::string("\\ManagedScripts\\");
					int newScriptCount = 0;
					bool recheck = true;
					while (recheck)
					{
						std::string tempNewSceneName = "NewScript" + (newScriptCount ? ("(" + std::to_string(newScriptCount) + ")") : "");
						recheck = false;
						for (auto& directory_entry : std::filesystem::directory_iterator(filePath))
						{
							if (tempNewSceneName == directory_entry.path().stem().string())
							{
								++newScriptCount;
								recheck = true;
								break;
							}
						}
					}

					std::ofstream ofs(filePath.string() + "NewScript" + (newScriptCount ? ("(" + std::to_string(newScriptCount) + ")") : "") + ".cs");
					ofs.close();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / buttonSize);
		ImGui::Columns(std::max(columnCount, 1), 0, false);
		int i = 0;
		std::filesystem::path filePath = std::filesystem::path(sceneManager->getScenePath()).parent_path().parent_path().parent_path().string() + std::string("\\ManagedScripts\\");
		for (auto& directory_entry : std::filesystem::directory_iterator(filePath))
		{
			if (directory_entry.path().extension() == ".cs")
			{
				ImGui::PushID(i);
				std::string scriptName = directory_entry.path().stem().string();
				if (ImGui::Button(scriptName.c_str(), { buttonSize , buttonSize }))
				{
					system((std::string("code ") + directory_entry.path().string()).c_str());
				}
				if (rightClick && ImGui::IsItemHovered())
				{
					ImGui::OpenPopup("scriptEditPopup");
				}

				if (ImGui::BeginPopupContextItem("scriptEditPopup"))
				{
					if (ImGui::Selectable("Rename Script"))
					{
						renameFileID = i;
						renameFileOldName = scriptName;
						TDS_INFO("Renamed Scene");
					}
					if (ImGui::Selectable("Delete Script"))
					{
						deleteSceneConfirmation = true;
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}

				if (deleteSceneConfirmation)
				{
					ImGui::OpenPopup("deleteScript");
					deleteSceneConfirmation = false;
				}

				ImVec2 center = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
				if (ImGui::BeginPopupModal("deleteScript", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text((scriptName + " will be deleted.\nThis operation cannot be undone!").c_str());
					ImGui::Separator();

					//static bool dont_ask_me_next_time = false;
					//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
					//ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
					//ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0)))
					{
						std::filesystem::remove(filePath.string() + scriptName + ".cs");
						TDS_INFO("Deleted Script");
						ImGui::CloseCurrentPopup();
					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0)))
					{
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}

				ImGui::TextWrapped(scriptName.c_str());

				ImGui::PopID();
				++i;
				ImGui::NextColumn();
			}
		}

		ImGui::Columns(1);
	}
}