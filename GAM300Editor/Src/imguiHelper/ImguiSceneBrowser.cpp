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
#include <commdlg.h> //file dialogue
#include "../EditorApp.h" //get hWnd
#include "sceneManager/serialization.h" //create new files
#include "sceneManager/sceneManager.h" //for scenemanager instance

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

		buttonSize = 60.f;
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
						std::filesystem::path filePath = sceneManager->getScenePath();
						for (auto& directory_entry : std::filesystem::directory_iterator(filePath))
						{
							if (tempNewSceneName == directory_entry.path().stem().string())
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
					sceneManager->saveCurrentScene();
				}
				if (ImGui::MenuItem("Save Scene As..."))
				{
					TDS_INFO("Save Button Pressed");

					std::string entered_filename = SaveFile("*.json", Application::GetWindowHandle());
					if (!entered_filename.empty())
					{
						SceneManager::GetInstance()->SerializeToFile(entered_filename);

					}

				}
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

		//for auto resizing of buttons
		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / buttonSize);
		ImGui::Columns(std::max(columnCount, 1), 0, false);

		int i = 0;
		std::filesystem::path filePath = sceneManager->getScenePath();
		for (auto& directory_entry : std::filesystem::directory_iterator(filePath))
		{
			if (directory_entry.path().extension() == ".json")
			{
				ImGui::PushID(i);
				std::string sceneName = directory_entry.path().stem().string();
				if (ImGui::Button(sceneName.c_str(), {buttonSize , buttonSize}) && sceneName != sceneManager->getCurrentScene())
				{
					hierarchyPanel->setSelectedEntity(0);
					sceneManager->loadScene(sceneName);
					hierarchyPanel->init();
				}

				if (rightClick && ImGui::IsItemHovered())
				{
					ImGui::OpenPopup("sceneEditPopup");
				}

				if (ImGui::BeginPopupContextItem("sceneEditPopup"))
				{
					if (sceneManager->getStartingScene() != sceneName)
					{
						if (ImGui::Selectable("Set As Entry"))
						{
							sceneManager->setStartingScene(sceneName);
							TDS_INFO("Reset entry scene");
						}
						ImGui::Separator();
					}

					if (ImGui::Selectable("Rename Scene"))
					{
						renameFileID = i;
						renameFileOldName = sceneName;
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
					ImGui::Text((sceneName + " will be deleted.\nThis operation cannot be undone!").c_str());
					ImGui::Separator();

					//static bool dont_ask_me_next_time = false;
					//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
					//ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
					//ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0)))
					{
						sceneManager->deleteScene(sceneName);
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
					strcpy_s(temp, sceneName.c_str());
					ImGui::InputText(("##" + std::to_string(i)).c_str(), temp, 100);
					sceneName = std::string(temp);

					if ((!ImGui::IsItemHovered() && (rightClick || ImGui::IsMouseClicked(ImGuiMouseButton_Left))) || ImGui::IsKeyPressed(ImGuiKey_Enter))
					{
						if (renameFileOldName != sceneName && !sceneManager->renameScene(renameFileOldName, sceneName))
						{
							sceneName = renameFileOldName;
							TDS_INFO("Scene name exist already!");
						}

						renameFileID = -1;
					}
				}
				else
				{
					ImGui::TextWrapped(sceneName.c_str());
				}

				ImGui::PopID();
				++i;
				ImGui::NextColumn();
			}
		}

		ImGui::Columns(1);
	}
	std::string SceneBrowser::SaveFile(const char* filter, HWND inHwnd)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = "../assets/scenes";
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT /*| OFN_NOCHANGEDIR*/;
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = inHwnd;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);

		//testtttttt
		if (GetCurrentDirectoryA(256, currentDir))
			ofn.lpstrInitialDir = "../assets/scenes";
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;

		// Sets the default extension by extracting it from the filter
		ofn.lpstrDefExt = strchr(filter, '\0') + 1;

		if (GetSaveFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;
		std::cout << "<<<<<<<<<< filepath is: " << ofn.lpstrFile << std::endl;
		return std::string();
	}
}