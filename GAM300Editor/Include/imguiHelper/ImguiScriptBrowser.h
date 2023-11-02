/*!*************************************************************************
****
\file ImguiScriptBrowser.h
\author Go Ruo Yan
\par DP email: ruoyan.go@digipen.edu
\date 20-10-2023
\brief  This program declares the functions used in the Level Editor Script
		Browser panel
****************************************************************************
***/

#ifndef IMGUI_SCRIPT_BROWSER
#define IMGUI_SCRIPT_BROWSER

#include <filesystem>

#include "sceneManager/sceneManager.h"
#include "ImguiHelper.h"

namespace TDS
{
	class ScriptBrowser : public LevelEditorPanel
	{
	public:
		/*!*************************************************************************
		This function initializes the Script Broswer panel
		****************************************************************************/
		ScriptBrowser();

		/*!*************************************************************************
		This function is the init function for Script Broswer panel
		****************************************************************************/
		void init() {}

		/*!*************************************************************************
		This function is the update function for Script Broswer panel
		****************************************************************************/
		void update();

	private:
		float buttonSize;
		bool deleteSceneConfirmation;
		int renameFileID;
		std::string renameFileOldName;
	};
}

#endif // IMGUI_SCRIPT_BROWSER
