/*!*************************************************************************
****
\file ImguiSceneBrowser.cpp
\author Go Ruo Yan
\par DP email: ruoyan.go@digipen.edu
\date 28-9-2023
\brief  This program defines the functions in the Level Editor Scene Browser
		panel
****************************************************************************
***/

#include "imguiHelper/ImguiSceneBrowser.h"
#include "imguiHelper/ImguiHierarchy.h"


namespace TDS
{
	/*!*************************************************************************
	This function initializes the Scene Broswer panel
	****************************************************************************/
	SceneBrowser::SceneBrowser()
	{
		flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse;
		panelTitle = "Scene Browser";
		windowPadding = ImVec2(10.f, 10.f);

		buttonSize = 40.f;
		deleteSceneConfirmation = false;
		renameFileID = -1;
		renameFileOldName = "";

	}

	/*!*************************************************************************
	This function is the update function for Scene Broswer panel
	****************************************************************************/
	void SceneBrowser::update()
	{
		std::shared_ptr<Hierarchy> hierarchyPanel = static_pointer_cast<Hierarchy>(LevelEditorManager::GetInstance()->panels[PanelTypes::HIERARCHY]);
		auto& sceneManager = SceneManager::GetInstance();

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
				if (ImGui::MenuItem("New")) 
				{
					int newSceneCount = 0;
					bool recheck = true;
					while (recheck)
					{
						std::string tempNewSceneName = "NewScene" + (newSceneCount ? ("(" + std::to_string(newSceneCount) + ")") : "");
						recheck = false;
						for (std::string scene : sceneManager->getScenes())
						{
							if (tempNewSceneName == scene)
							{
								++newSceneCount;
								recheck = true;
								break;
							}
						}
					}

					sceneManager->newScene("NewScene" + (newSceneCount ? ("(" + std::to_string(newSceneCount) + ")") : ""));
				}
				if (ImGui::MenuItem("Save")) 
				{
					hierarchyPanel->changeIndexInEntity();
					sceneManager->saveCurrentScene();
				}
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / buttonSize);
		ImGui::Columns(std::max(columnCount, 1), 0, false);
		//ImGui::Columns(4, 0, false);
		int i = 0;
		for (std::string scene : sceneManager->getScenes())
		{
			ImGui::PushID(i);
			if (ImGui::Button(scene.c_str(), { buttonSize , buttonSize }) && scene != sceneManager->getCurrentScene())
			{
				hierarchyPanel->setSelectedEntity(0);
				sceneManager->loadScene(scene);
				hierarchyPanel->init();
			}

			if (rightClick && ImGui::IsItemHovered())
			{
				ImGui::OpenPopup("sceneEditPopup");
			}

			if (ImGui::BeginPopupContextItem("sceneEditPopup"))
			{
				if (ImGui::Selectable("Rename Scene"))
				{
					renameFileID = i;
					renameFileOldName = scene;
					TDS_INFO("Renamed Scene");
				}
				if (ImGui::Selectable("Delete Scene"))
				{
					deleteSceneConfirmation = true;
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

			if (deleteSceneConfirmation)
			{
				ImGui::OpenPopup("deleteScene");
				deleteSceneConfirmation = false;
			}

			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
			if (ImGui::BeginPopupModal("deleteScene", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text((scene + " will be deleted.\nThis operation cannot be undone!").c_str());
				ImGui::Separator();

				//static bool dont_ask_me_next_time = false;
				//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
				//ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
				//ImGui::PopStyleVar();

				if (ImGui::Button("OK", ImVec2(120, 0)))
				{
					sceneManager->deleteScene(scene);
					TDS_INFO("Deleted Scene");
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

			if (renameFileID == i)
			{
				ImGui::SetKeyboardFocusHere();
				char temp[100];
				strcpy_s(temp, scene.c_str());
				ImGui::InputText(("##" + std::to_string(i)).c_str(), temp, 100);
				scene = std::string(temp);

				if ((!ImGui::IsItemHovered() && (rightClick || ImGui::IsMouseClicked(ImGuiMouseButton_Left))) || ImGui::IsKeyPressed(ImGuiKey_Enter))
				{
					if (renameFileOldName != scene && !sceneManager->renameScene(renameFileOldName, scene))
					{
						scene = renameFileOldName;
						TDS_INFO("Scene name exist already!");
					}

					renameFileID = -1;
				}
			}
			else
			{
				ImGui::Text(scene.c_str());
			}

			ImGui::PopID();
			++i;
			ImGui::NextColumn();
		}

		ImGui::Columns(1);

	}
}