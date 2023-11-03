#include "imguiHelper/ImguiAssetBrowser.h"
#include "Tools/FontLoader.h"
#include "Tools/FileExtensions.h"
#include "Tools/TextureCompressor.h"
#include "Tools/DDSConverter.h"
#include "Tools/GeomCompiler.h"
#include "imguiHelper/ImguiHierarchy.h"
#include "AssetManagement/AssetManager.h"
#include "vulkanTools/vulkanInstance.h"
#include "vulkanTools/VulkanTexture.h"
#include <string>


#define ASSET_PATH "../../assets"
namespace TDS
{
	bool lookUp = false;

	//for icon loading
	Texture fileIcon{}, folderIcon{};
	VkDescriptorSet file_DescSet{}, folder_DescSet{};
	//int file_image_count = 0, folder_image_count = 0;

	AssetBrowser::AssetBrowser()
	{
		//selected = 0;
		//selectedFolder = -1;
		//renameCheck = false;

		flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse;
		panelTitle = "Asset Browser";
		windowPadding = ImVec2(0.f, 0.f);
		m_curr_path = std::filesystem::path(ASSET_PATH);

		//insertEntities();
	}

	void AssetBrowser::getFileNameFromPath(const char* full_path, std::string* path, std::string* file, std::string* fileWithExtension, std::string* extension) const
	{
		if (full_path != nullptr)
		{
			std::string nwFullPath = full_path;
			std::string& full_path_ = nwFullPath;
			//bool toLower;
			for (std::string::iterator charIterator = full_path_.begin(); charIterator != full_path_.end(); ++charIterator)
			{
				if (*charIterator == '\\')
					*charIterator = '/';
				/*else
					if (toLower) {
						*charIterator = tolower(*charIterator);
					}*/
			}
			size_t posSlash = nwFullPath.find_last_of("\\/");
			size_t posDot = nwFullPath.find_last_of(".");

			if (path != nullptr)
			{
				if (posSlash < nwFullPath.length())
					*path = nwFullPath.substr(0, posSlash + 1);
				else
					path->clear();
			}
			if (fileWithExtension != nullptr) {
				if (posSlash < nwFullPath.length()) {
					*fileWithExtension = nwFullPath.substr(posSlash + 1);
				}
				else
					*fileWithExtension = nwFullPath;
			}

			if (file != nullptr)
			{
				nwFullPath = nwFullPath.substr(0, posDot);
				posSlash = nwFullPath.find_last_of("\\/");
				*file = nwFullPath.substr(posSlash + 1);

			}

			if (extension != nullptr)
			{
				if (posDot < nwFullPath.length())
					*extension = nwFullPath.substr(posDot);
				else
					extension->clear();
			}
		}
	}

	static const std::filesystem::path s_TextureDirectory = "../../assets";
	void AssetBrowser::update()
	{
		if (m_curr_path != std::filesystem::path(s_TextureDirectory))
		{
			if (ImGui::Button("<-")) //will only show if u went into a folder in the current directory above
			{
				m_curr_path = m_curr_path.parent_path();
			}
		}
		float cellSize = thumbnail_size + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		ImGui::Columns(std::max(columnCount, 1), 0, false);

		//load texture info ONCE
		
		if (loadonce == true)
		{
			fileIcon.LoadTexture("../../assets/icons/icon.dds"); //can only take dds files for now
			file_DescSet = ImGui_ImplVulkan_AddTexture(fileIcon.m_VulkanTexture->getInfo().sampler, fileIcon.m_VulkanTexture->getInfo().imageView, fileIcon.m_VulkanTexture->getInfo().imageLayout);

			folderIcon.LoadTexture("../../assets/icons/folder.dds"); //can only take dds files for now
			folder_DescSet = ImGui_ImplVulkan_AddTexture(folderIcon.m_VulkanTexture->getInfo().sampler, folderIcon.m_VulkanTexture->getInfo().imageView, folderIcon.m_VulkanTexture->getInfo().imageLayout);

			loadonce = false;
		}

		

		for (auto& directory_entry : std::filesystem::directory_iterator(m_curr_path))
		{
			path1 = directory_entry.path().string();
			auto relative_path = std::filesystem::relative(directory_entry.path(), s_TextureDirectory);

			//ImGui::Button(path1.c_str(), { thumbnail_size, thumbnail_size });
			//shorten the path name
			std::string filename;
			getFileNameFromPath(path1.c_str(), nullptr, nullptr, &filename, nullptr);
			ImGui::PushID(filename.c_str()); //store the current id in each button that is created in each iteration of files
			
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			

			if (directory_entry.is_directory()) //draw folder icon
			{
				ImGui::ImageButton(reinterpret_cast<void*>(folder_DescSet), ImVec2{ thumbnail_size, thumbnail_size }, ImVec2{ 0, 0 }, ImVec2{ 1, 1 });
				//folder_image_count++;

			}
			else //draw file icon
			{
				//then render button
				ImGui::ImageButton(reinterpret_cast<void*>(file_DescSet), ImVec2{ thumbnail_size, thumbnail_size }, ImVec2{ 0, 0 }, ImVec2{ 1, 1 });
				//do drag drop ONLY on files, not folder
				if (ImGui::BeginDragDropSource())
				{
					std::filesystem::path relativePath(path1);
					const wchar_t* itemPath = relativePath.c_str();
					ImGui::Text(filename.c_str()); //need to get info from image
					ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
					ImGui::EndDragDropSource();
				}
				//file_image_count++;

				//ImGui::Button(filename.c_str(), { thumbnail_size, thumbnail_size });

			}
			ImGui::PopStyleColor();

			if (ImGui::IsItemClicked(0))
			{
				
				//if it has a "." in it, it is a file, do not add to path
				//only directories/folders can be added to path

				//if it is a folder, open it and update the asset broswer curr dir
				if (!strstr(filename.c_str(), ".")) 
				{
					m_curr_path += "/" + filename;
				

					//attempt at drag drop
					selectedpath = path1;
					

				}
				//use the rest of the checks below to handle what happens when u press different kinds of file extensions

				//if .jpg/.PNG, load 2d texture...
				if (strstr(filename.c_str(), ".jpg") || strstr(filename.c_str(), ".png") || strstr(filename.c_str(), ".dds"))
				{
					lookUp = false;



					std::shared_ptr<Hierarchy> panel = static_pointer_cast<Hierarchy>(LevelEditorManager::GetInstance()->panels[HIERARCHY]);

					EntityID currEntity = panel->getSelectedEntity();
					IComponent* Graph = getComponentByName("Graphics Component", panel->getSelectedEntity());
					if (Graph == nullptr)
						Graph = addComponentByName("Graphics Component", panel->getSelectedEntity());
					GraphicsComponent* graphComp = reinterpret_cast<GraphicsComponent*>(Graph);

					std::string OutPath = ASSET_PATH;
					OutPath += "/textures/";
					OutPath += filename.c_str();
					std::string inPath = OutPath;
					if (strstr(filename.c_str(), ".jpg"))
						OutPath = RemoveFileExtension(OutPath, ".jpg");
					else if (strstr(filename.c_str(), ".png"))
						OutPath = RemoveFileExtension(OutPath, ".png");


					if (strstr(filename.c_str(), ".dds"))
						lookUp = true;
					else
						OutPath += ".dds";


					if (lookUp == false)
						TextureCompressor::GetInstance().Run(inPath, OutPath);



					AssetManager::GetInstance()->LoadAsset(OutPath, graphComp->GetTexture());





				}
				if (strstr(filename.c_str(), ".obj") || strstr(filename.c_str(), ".fbx") || strstr(filename.c_str(), ".gltf") || strstr(filename.c_str(), ".bin"))
				{
					lookUp = false;
					std::string& OutPath = GeomCompiler::GetInstance()->OutPath;
					OutPath = MODEL_PATH;
					OutPath += filename.c_str();
					if (strstr(filename.c_str(), ".fbx"))
						OutPath = RemoveFileExtension(OutPath, ".fbx");
					else if (strstr(filename.c_str(), ".gltf"))
						OutPath = RemoveFileExtension(OutPath, ".gltf");
					else if (strstr(filename.c_str(), ".obj"))
						OutPath = RemoveFileExtension(OutPath, ".obj");
					else if (strstr(filename.c_str(), ".bin"))
					{
						lookUp = true;
						/*OutPath = std::filesystem::path(OutPath).filename().string();*/
					}

					std::shared_ptr<Hierarchy> panel = static_pointer_cast<Hierarchy>(LevelEditorManager::GetInstance()->panels[HIERARCHY]);

					EntityID currEntity = panel->getSelectedEntity();
					IComponent* Graph = getComponentByName("Graphics Component", panel->getSelectedEntity());
					if (Graph == nullptr)
						Graph = addComponentByName("Graphics Component", panel->getSelectedEntity());
					GraphicsComponent* graphComp = reinterpret_cast<GraphicsComponent*>(Graph);

					if (lookUp == false)
					{
						GeomDescriptor m_GeomDescriptor{};
						m_GeomDescriptor.m_Descriptor.m_FilePath = std::filesystem::path(filename).filename().string();
						GeomCompiler::GetInstance()->InitDesc(m_GeomDescriptor);
						std::string OutputFile = GeomCompiler::GetInstance()->LoadModel();
						AssetManager::GetInstance()->LoadAsset(OutputFile, graphComp->GetAsset());
					}
					else
						AssetManager::GetInstance()->LoadAsset(OutPath, graphComp->GetAsset());



				}
				//if .json, load scene...
				if (strstr(filename.c_str(), ".json"))
				{

				}

				//if .wav, play audio...
				if (strstr(filename.c_str(), ".wav"))
				{

				}


			}
			ImGui::TextWrapped(filename.c_str()); //currently using this to display full file name
			
			/*if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceExtern))
			{
				ImGui::SetDragDropPayload("path", path1.c_str(), 1);
				ImGui::Text(path1.c_str());
				if (!ImGui::IsMouseDown(0))
				{
					ImGui::EndDragDropSource();

				}
			}

			if (ImGui::BeginDragDropTarget())
			{
				ImGui::EndDragDropTarget();
			}*/
			ImGui::NextColumn();
			ImGui::PopID(); //store new id for next button
		}


		ImGui::Columns(1);
		//ImGui::SliderFloat("Thumbnail Size", &thumbnail_size, 16, 512);
		//ImGui::SliderFloat("Padding", &padding, 0, 32);
	}
	void AssetBrowser::destroyIcons()
	{
		fileIcon.m_VulkanTexture->Destroy(); //temp, to prevent mem leak
		folderIcon.m_VulkanTexture->Destroy(); //temp, to prevent mem leak
		
		/* when breakpoint, these 2 lines below free nothing
		
		fileIcon.Destroy();
		folderIcon.Destroy();*/

		//seems like the while loops dont cause multiple textures to be freed 
		//while (file_image_count)
		//{
		//	fileIcon.m_VulkanTexture->Destroy(); //temp, to prevent mem leak
		//	//fileIcon.Destroy();
		//	file_image_count--;

		//}
		//while (folder_image_count)
		//{
		//	folderIcon.m_VulkanTexture->Destroy(); //temp, to prevent mem leak
		//	//folderIcon.Destroy();
		//	folder_image_count--;

		//}
	}
}