#include "imguiHelper/ImguiToolbar.h"
#include "imguiHelper/ImguiConsole.h"

#include "sceneManager/sceneManager.h"
#include "Logger/Logger.h"

namespace TDS
{
	/*EditorConsole console;*/ //Wrong method of getting instance
	std::shared_ptr<EditorConsole> console = static_pointer_cast<EditorConsole>(LevelEditorManager::GetInstance()->panels[PanelTypes::CONSOLE]);

	Toolbar::Toolbar()
	{
		//selected = 0;
		//selectedFolder = -1;
		//renameCheck = false;

		flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse;
		panelTitle = "Toolbar";
		windowPadding = ImVec2(0.f, 0.f);
		

		//insertEntities();
	}

	
	void Toolbar::update()
	{
		ImGui::Text("Display: "); ImGui::SameLine();

		if (ImGui::ArrowButton("Play", ImGuiDir_Right))
		{
			TDS_INFO("Play button pressed");
			console->AddLog(Log::GetImguiLog().c_str());
			//console->AddLog("Play button pressed");
			//if (isPlay) {
			//	//App->timeManagement->Play();
			//	////TODO: Call the Init of the particles
			//	//App->scene->PlayScene(App->scene->GetRoot(), App->scene->GetRoot());
			//}
			//else if (isPause) {
			//	//set game to resume
			//	//App->timeManagement->Resume();
			//}
			/*App->scene->inGame = true;*/

			isPlaying = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("||", { 23, 19 }))
		{
			TDS_INFO("Pause button pressed");
			console->AddLog(Log::GetImguiLog().c_str());

			//if (isPause) {
			//	//pause
			//	console->AddLog("Pause button pressed");

			//}

			isPlaying = false;
		}
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.8f,0,0,1 });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1,0.2f,0,1 });
		if (ImGui::Button("STOP", { 40, 19 }))
		{
			TDS_INFO("Stop button pressed");
			console->AddLog(Log::GetImguiLog().c_str());

			if (isStopped) {

				//stop the app
				/*App->timeManagement->Stop();
				App->scene->inGame = false;
				App->scene->StopScene(App->scene->GetRoot(), App->scene->GetRoot());*/
			}
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();


		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.1f,0,0,1 });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1,0.2f,0,1 });
		if (ImGui::Button("Open FMOD Studio", { 120, 19 }))
		{

			TDS_INFO("Opening FMOD Studio");
			console->AddLog(Log::GetImguiLog().c_str());

			if (isOpenFMOD) {

				const char* fmodStudioPath = "..\\Dependencies\\FMOD_Studio_2.02.17\\FMOD_Studio.exe";
				const char* command = fmodStudioPath;
				system(command);
			}
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();

		//ImGui::SameLine();
		//ImGui::PushStyleColor(ImGuiCol_Button, { 0.1f,0.1f,0.1f,1 });
		//ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1,0.1f,0,1 });
		//if (ImGui::Button("Load Scene", { 100, 19 }))
		//{
		//	console->AddLog("Load Scene Button Pressed");
		//	if (isLoadScene) {


		//	}
		//}
		//ImGui::PopStyleColor();
		//ImGui::PopStyleColor();

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.1f,0.1f,0.1f,1 });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1,0.1f,0,1 });
		if (ImGui::Button("Save Scene", { 100, 19 }))
		{
			TDS_INFO("Save Scene Button Pressed");
			console->AddLog(Log::GetImguiLog().c_str());

			if (isSaveScene) {

				SceneManager::GetInstance()->saveScene(SceneManager::GetInstance()->getCurrentScene());
			}
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();

		//ImGui::SameLine();
		//ImGui::PushStyleColor(ImGuiCol_Button, { 0.1f,0.1f,0.1f,1 });
		//ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1,0.1f,0,1 });
		//if (ImGui::Button("Add Component", { 100, 19 }))
		//{

		//	console->AddLog("Adding Component");
		//	//do add component
		//}
		//ImGui::PopStyleColor();
		//ImGui::PopStyleColor();
	}
}