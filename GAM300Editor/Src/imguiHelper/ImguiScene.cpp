#include "imguiHelper/ImguiScene.h"
#include "imguiHelper/ImguiAssetBrowser.h"
#include "GraphicsResource/TextureInfo.h"
#include "vulkanTools/VulkanTexture.h"
#include <sstream>
#include <corecrt_wstring.h>
#include "imguiHelper/ImguiConsole.h"
#include "Rendering/GraphicsManager.h"
#include "Rendering/RenderTarget.h"
#include "vulkanTools/Renderer.h"
#include "imgui/ImGuizmo.h"
namespace TDS
{
	Texture data{};
	VkDescriptorSet m_DescSet = nullptr;
	std::shared_ptr<EditorConsole> consolelog = static_pointer_cast<EditorConsole>(LevelEditorManager::GetInstance()->panels[PanelTypes::CONSOLE]);

	EditorScene::EditorScene()
	{
		//selected = 0;
		//selectedFolder = -1;
		//renameCheck = false;

		flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse;
		panelTitle = "Scene";
		windowPadding = ImVec2(0.f, 0.f);

	}

	std::string tempPath = "../../assets/textures/texture.dds";
	void EditorScene::init()
	{
		m_DescSet = ImGui_ImplVulkan_AddTexture(GraphicsManager::getInstance().getFinalImage().getSampler(), GraphicsManager::getInstance().getFinalImage().getImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}
	void EditorScene::update()
	{
		//ImGui::Image(reinterpret_cast<void*>(vkTexture.m_DescSet), ImVec2{ 200, 200 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		
		//data.LoadTexture(tempPath);
		//vkTexture.CreateBasicTexture(data.m_TextureInfo);
		ImVec2 vSize = ImGui::GetContentRegionAvail();
		m_DescSet = ImGui_ImplVulkan_AddTexture(GraphicsManager::getInstance().getFinalImage().getSampler(), GraphicsManager::getInstance().getFinalImage().getImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		ImGui::Image((ImTextureID)m_DescSet, vSize);
		//shifted drag drop here, must be directly under the line where imgui image is created
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::wstring ws(path);
				//		//your new String
				std::string str(ws.begin(), ws.end());
				const std::filesystem::path filesystempath = str;

				if (filesystempath.extension() != ".dds")
				{
					consolelog->AddLog("Invalid file type, please drag a .DDS file.");
				}
				else
				{
					//tempPath = str;
		//			std::wcout << " Path is: " << path << std::endl;

		//			//currently i am only changing filepath names, next time i need to impose pictures onto pictures, create new image layer 
		//			//and also parse each image's width and height, rn its hardcoded to 500 x 500 px
		//			/*tempPath = path;
		//			data.LoadTexture(tempPath);
		//			vkTexture.CreateBasicTexture(data.m_TextureInfo);*/

				}
			}
			ImGui::EndDragDropTarget();
		}
		//ImGui::End();
		//code above is currently commented out due to how images are infinitely rendered in update loop and not freed, just for testing

	/*	ImGui::Text("comment out update() in \n ImguiScene.cpp to test dragdrop");
		ImGui::Text("The Image above tests \n Drag and Drop of .dds files");
		ImGui::Text("Drag a .dds file from the \n content browser on the left and drop onto the image");
		ImGui::Text("xx u can render the frame \n buffer here to replace the ImGui::Image");
		ImGui::Text("so in future we will drag \n and rendering multiple images onto the scene");*/


		std::shared_ptr<Hierarchy> hierarchyPanel = static_pointer_cast<Hierarchy>(LevelEditorManager::GetInstance()->panels[PanelTypes::HIERARCHY]);
		if (EntityID selectedEntity = hierarchyPanel->getSelectedEntity())
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

			Mat4 projection = Mat4::Perspective(GraphicsManager::getInstance().GetCamera().m_Fov * Mathf::Deg2Rad,
				GraphicsManager::getInstance().GetSwapchainRenderer().getAspectRatio(), 0.1f, 10.f);
			//projection.m[1][1] *= -1;
			Mat4 view = GraphicsManager::getInstance().GetCamera().GetViewMatrix();

			const auto& trans = ecs.getComponent<Transform>(selectedEntity)/*->GetTransformMatrix()*/;

			Vec3 snap = 1.0f;

			float* _proj = Mat4::Mat4Value_ptr(projection);
			float* _view = Mat4::Mat4Value_ptr(view);
			float* _trans = Mat4::Mat4Value_ptr(trans->GetTransformMatrix());
			float* _snap = Vec3::Vec3Value_ptr(snap);
			if (TDS::Input::isKeyPressed(TDS_1)) { m_gizmoType = static_cast<int>(ImGuizmo::OPERATION::TRANSLATE); }
			else if (TDS::Input::isKeyPressed(TDS_2)) { m_gizmoType = static_cast<int>(ImGuizmo::OPERATION::SCALE); }
			else if (TDS::Input::isKeyPressed(TDS_3)) { m_gizmoType = static_cast<int>(ImGuizmo::OPERATION::ROTATE); }
			bool val = ImGuizmo::Manipulate(
				_view, _proj, (ImGuizmo::OPERATION)m_gizmoType, ImGuizmo::WORLD, _trans,
				nullptr, false ? _snap : nullptr
			);

			m_gizmoActive = ImGuizmo::IsUsing();

			if (ImGuizmo::IsUsing() && m_gizmoType != -1)
			{

				Vec3 transl{};
				Vec3 rotat{};
				Vec3 scal{};

				float* _transl = Vec3::Vec3Value_ptr(transl);
				float* _rotat = Vec3::Vec3Value_ptr(rotat);
				float* _scal = Vec3::Vec3Value_ptr(scal);

				ImGuizmo::DecomposeMatrixToComponents(_trans, _transl, _rotat, _scal);

				trans->SetPosition(_transl[0], _transl[1], _transl[2]);
				trans->SetRotation(_rotat[0] * Mathf::Deg2Rad, _rotat[1] * Mathf::Deg2Rad, _rotat[2] * Mathf::Deg2Rad);
				if (_scal[0] > 0.f && _scal[1] > 0.f && _scal[2] > 0.f)
					trans->SetScale(_scal[0], _scal[1], _scal[2]);


				delete[] _transl;
				delete[] _rotat;
				delete[] _scal;
			}

			delete[] _proj;
			delete[] _view;
			delete[] _trans;
			delete[] _snap;
		}
	}
	void EditorScene::Resize()
	{
		if (m_DescSet)
		{
			ImGui_ImplVulkan_RemoveTexture(m_DescSet);
			m_DescSet = ImGui_ImplVulkan_AddTexture(GraphicsManager::getInstance().getFinalImage().getSampler(), GraphicsManager::getInstance().getFinalImage().getImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		}
	}
}