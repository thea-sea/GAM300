/*!*************************************************************************
****
\file ImguiSceneBrowser.h
\author Go Ruo Yan
\par DP email: ruoyan.go@digipen.edu
\date 28-9-2023
\brief  This program declares the functions used in the Level Editor Scene
		Browser panel
****************************************************************************
***/

#ifndef IMGUI_SCENE_BROWSER
#define IMGUI_SCENE_BROWSER

#include <filesystem>

#include "sceneManager/sceneManager.h"
#include "ImguiHelper.h"

namespace TDS 
{
	class SceneBrowser : public LevelEditorPanel
	{
	public:
		/*!*************************************************************************
		This function initializes the Scene Broswer panel
		****************************************************************************/
		SceneBrowser();

		/*!*************************************************************************
		This function is the init function for Scene Broswer panel
		****************************************************************************/
		void init() {}

		/*!*************************************************************************
		This function is the update function for Scene Broswer panel
		****************************************************************************/
		void update();

		/*!*************************************************************************
		This function is to open a file dialog for Save As with a filename based on user input
		****************************************************************************/
		static std::string SaveFile(const char* filter, HWND inHwnd);



	private:
		float buttonSize;
		bool deleteSceneConfirmation;
		int renameFileID;
		std::string renameFileOldName;
	};
}

#endif // IMGUI_SCENE_BROWSER
