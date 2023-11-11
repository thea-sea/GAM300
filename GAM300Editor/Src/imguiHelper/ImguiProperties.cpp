/*!*************************************************************************
****
\file ImguiProperties.cpp
\author Go Ruo Yan
\par DP email: ruoyan.go@digipen.edu
\date 28-9-2023
\brief  This program defines the functions in the Level Editor Properties
		panel
****************************************************************************
***/


#include "imguiHelper/ImGuiFunctionHelper.h"

#include "imguiHelper/ImguiProperties.h"
#include "imguiHelper/ImguiHierarchy.h"

#include "eventManager/eventHandler.h"
#include "imguiHelper/ImguiAssetBrowser.h"


namespace TDS
{
	/*!*************************************************************************
	This function initializes the Properties panel
	****************************************************************************/
	Properties::Properties()
	{
		flags = ImGuiWindowFlags_NoCollapse;
		panelTitle = "Properties";
		windowPadding = ImVec2(0.f, 0.f);
	}

	/*!*************************************************************************
	This function is the update function for Properties panel
	****************************************************************************/
	void Properties::update()
	{
		std::shared_ptr<Hierarchy> hierarchyPanel = static_pointer_cast<Hierarchy>(LevelEditorManager::GetInstance()->panels[PanelTypes::HIERARCHY]);
		auto& sceneManagerInstance = SceneManager::GetInstance();

		// If there is a selected entity
		if (EntityID selectedEntity = hierarchyPanel->getSelectedEntity())
		{
			std::vector<std::string> allComponentNames = ecs.getAllRegisteredComponents();

			ImGuiTreeNodeFlags nodeFlags =
				ImGuiTreeNodeFlags_DefaultOpen |
				ImGuiTreeNodeFlags_FramePadding |
				ImGuiTreeNodeFlags_SpanAvailWidth |
				ImGuiTreeNodeFlags_SpanFullWidth |
				ImGuiTreeNodeFlags_Selected | 
				ImGuiTreeNodeFlags_AllowOverlap;

			int i = 0;
			for (auto componentName : allComponentNames)
			{
				IComponent* componentBase = getComponentByName(componentName, selectedEntity);

				if (!componentBase)
				{
					continue;
				}

				if (componentName == "Name Tag"/* && ImGui::BeginTable("###", 2)*/)
				{
					auto nameTagComponent = reinterpret_cast<NameTag*>(componentBase);

					bool entityEnabled = ecs.getEntityIsEnabled(selectedEntity);

					ImGui::Checkbox(("##" + componentName + "CheckBox").c_str(), &entityEnabled);

					if (ImGui::IsItemDeactivatedAfterEdit())
					{
						ecs.setEntityIsEnabled(selectedEntity, entityEnabled);
					}

					ImGui::SameLine();
					ImGui::PushItemWidth(-FLT_MIN);

					std::string newValue = nameTagComponent->GetName();
					char temp[100];
					strcpy_s(temp, newValue.c_str());
					ImGui::InputText(("##" + newValue).c_str(), temp, 100);

					if (ImGui::IsItemDeactivatedAfterEdit())
					{
						nameTagComponent->SetName(std::string(temp));
						sceneManagerInstance->updateName(selectedEntity, std::string(temp));
					}

					ImGui::PopItemWidth();
					
					//int parent = static_cast<int>(nameTagComponent->GetHierarchyParent());
					//ImguiInput("", parent);
					//int indexInParent = static_cast<int>(nameTagComponent->GetHierarchyIndex());
					//ImguiInput("", indexInParent);

					//ImGui::EndTable();

					continue;
				}

				bool componentOpen = ImGui::TreeNodeEx(("##" + componentName).c_str(), nodeFlags);
				bool componentRemoved = false;

				ImGui::PushID(i);
				if (ImGui::BeginPopupContextItem("componentEditPopup"))
				{
					if (componentName != "Name Tag" && componentName != "Transform" && ImGui::Selectable("Remove Component"))
					{
						removeComponentByName(componentName, selectedEntity);
						TDS_INFO("Removed Component");
						componentRemoved = true;
					}

					ImGui::EndPopup();
				}

				if (rightClick && ImGui::IsItemHovered())
				{
					ImGui::OpenPopup("componentEditPopup");
				}
				ImGui::PopID();

				if (componentName != "Transform")
				{
					bool componentEnabled = getComponentIsEnable(componentName, selectedEntity);

					ImGui::SameLine();
					ImGui::Checkbox(("##" + componentName + "CheckBox").c_str(), &componentEnabled);

					if (ImGui::IsItemDeactivatedAfterEdit())
					{
						setComponentIsEnable(componentName, selectedEntity, componentEnabled);
					}
				}

				ImGui::SameLine();
				ImGui::Text(componentName.c_str());

				++i;

				if (componentOpen && !componentRemoved)
				{
					if (ImGui::BeginTable("###", 2, /*ImGuiTableFlags_Borders |*/ ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_SizingStretchSame, ImVec2(0.0f, 5.5f)))
					{
						ImGui::TableSetupColumn("Component variable name", ImGuiTableColumnFlags_None, ImGui::GetContentRegionAvail().x * 0.4f);
						ImGui::TableSetupColumn("Component variable value", ImGuiTableColumnFlags_None, ImGui::GetContentRegionAvail().x * 0.6f);

						ImGui::TableNextRow();

						ImGui::TableNextColumn();
						ImGui::TableNextColumn();
						ImGui::PushItemWidth(-FLT_MIN); // Right-aligned

						// For child transform change
						Vec3 oldPosition, oldScale, oldRotation;
						if (componentName == "Transform")
						{
							Transform* transformComponent = reinterpret_cast<Transform*>(componentBase);
							oldPosition = transformComponent->GetPosition();
							oldScale = transformComponent->GetScale();
							oldRotation = transformComponent->GetRotation();

							NameTag* nameTagComponent = GetNameTag(selectedEntity);
							if (nameTagComponent->GetHierarchyParent() > 0)
							{
								Transform* parentTransformComponent = GetTransform(nameTagComponent->GetHierarchyParent());

								Vec3 parentPosition = parentTransformComponent->GetPosition();
								Vec3 reflectedPosition = oldPosition - parentPosition;
								ImguiInput("Position", reflectedPosition);
								transformComponent->SetPosition(reflectedPosition + parentPosition);

								Vec3 parentScale = parentTransformComponent->GetScale();
								Vec3 reflectedScale = oldScale - parentScale;
								ImguiInput("Scale", reflectedScale);
								transformComponent->SetScale(reflectedScale + parentScale);

								Vec3 parentRotation = parentTransformComponent->GetRotation();
								Vec3 reflectedRotation = oldRotation - parentRotation;
								ImguiInput("Position", reflectedRotation);
								transformComponent->SetRotation(reflectedRotation + parentRotation);
							}
							else
							{
								ImguiComponentDisplay(componentName, componentBase);
							}

							EventHandler::postChildTransformationEvent(selectedEntity, oldPosition, oldScale, oldRotation);
						}
						else
						{
							ImguiComponentDisplay(componentName, componentBase);
						}

						ImGui::PopItemWidth();
						ImGui::EndTable();
					}

					ImGui::TreePop();
				}
				else if (componentRemoved)
				{
					ImGui::TreePop();
				}
			}

			int buttonID = 0;
			for (auto scriptName : sceneManagerInstance->getAllScripts())
			{
				if (!sceneManagerInstance->hasScript(selectedEntity, scriptName))
				{
					continue;
				}

				if (ImGui::TreeNodeEx(("##" + scriptName).c_str(), nodeFlags))
				{
					bool componentEnabled = sceneManagerInstance->isScriptEnabled(selectedEntity, scriptName);

					ImGui::SameLine();
					ImGui::Checkbox(("##" + scriptName + "CheckBox").c_str(), &componentEnabled);

					if (ImGui::IsItemDeactivatedAfterEdit())
					{
						sceneManagerInstance->toggleScript(selectedEntity, scriptName.c_str());
						ImGui::TreePop();
						continue;
					}

					ImGui::SameLine();
					ImGui::Text(scriptName.c_str());

					//ImGui::PushID(selectedEntity);
					if (ImGui::BeginTable("components", 2, /*ImGuiTableFlags_Borders |*/ ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_SizingStretchSame, ImVec2(0.0f, 5.5f)))
					{
						ImGui::TableSetupColumn("Component variable name", ImGuiTableColumnFlags_None, ImGui::GetContentRegionAvail().x * 0.4f);
						ImGui::TableSetupColumn("Component variable value", ImGuiTableColumnFlags_None, ImGui::GetContentRegionAvail().x * 0.6f);

						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						//ImGui::SetColumnWidth(ImGui::GetContentRegionAvail().x * 0.3f);
						ImGui::TableNextColumn();
						ImGui::PushItemWidth(-FLT_MIN); // Right-aligned

						std::vector<ScriptValues> allValues = sceneManagerInstance->getScriptVariables(selectedEntity, scriptName);

						for (ScriptValues scriptValue : allValues)
						{
							if (scriptValue.type == "System.Boolean")
							{
								bool value = scriptValue.value == "False" ? false : true;
								if (ImguiInput(scriptValue.name, value))
								{
									sceneManagerInstance->setBool(selectedEntity, scriptName, scriptValue.name, value);
								}
							}
							else if (scriptValue.type == "System.Int16"
								|| scriptValue.type == "System.Int32"
								|| scriptValue.type == "System.Int64"
								|| scriptValue.type == "System.UInt16"
								|| scriptValue.type == "System.UInt32"
								|| scriptValue.type == "System.UInt64"
								|| scriptValue.type == "System.Byte"
								|| scriptValue.type == "System.SByte")
							{
								int value = std::stoi(scriptValue.value);
								if (ImguiInput(scriptValue.name, value))
								{
									sceneManagerInstance->setInt(selectedEntity, scriptName, scriptValue.name, value);
								}
							}
							else if (scriptValue.type == "System.Double")
							{
								float value = std::stod(scriptValue.value);
								if (ImguiInput(scriptValue.name, value))
								{
									sceneManagerInstance->setDouble(selectedEntity, scriptName, scriptValue.name, static_cast<double>(value));
								}
							}
							else if (scriptValue.type == "System.Single")
							{
								float value = std::stod(scriptValue.value);
								if (ImguiInput(scriptValue.name, value))
								{
									sceneManagerInstance->setFloat(selectedEntity, scriptName, scriptValue.name, value);
								}
							}
							else if (scriptValue.type == "System.String")
							{
								std::string value = scriptValue.value;
								if (ImguiInput(scriptValue.name, value))
								{
									sceneManagerInstance->setString(selectedEntity, scriptName, scriptValue.name, value);
								}
							}
							else if (scriptValue.type == "System.Char")
							{
								//char value = scriptValue.value[0];
								//value = ImguiInput(scriptValue.name, value);
								//sceneManagerInstance->setChar(selectedEntity, scriptName, scriptValue.name, value);
							}
							else // scripts & game object
							{
								// To Do 
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text(scriptValue.name.c_str());

								ImGui::TableNextColumn();

								// Game Object
								ImGui::PushItemWidth(-ImGui::GetContentRegionAvail().x * 0.15f);

								if (scriptValue.referenceEntityID) // there is a entity reference 
								{
									if (scriptValue.type == "ScriptAPI.GameObject")
									{
										ImGui::BeginDisabled();
										char temp[100];
										strcpy_s(temp, ecs.getComponent<NameTag>(scriptValue.referenceEntityID)->GetName().c_str());
										ImGui::InputText("###", temp, 100, ImGuiInputTextFlags_ReadOnly);
										ImGui::EndDisabled();
									}
									else
									{
										ImGui::BeginDisabled();
										char temp[100];
										strcpy_s(temp, (ecs.getComponent<NameTag>(scriptValue.referenceEntityID)->GetName() + " (" + scriptValue.type + ")").c_str());
										ImGui::InputText("###", temp, 100, ImGuiInputTextFlags_ReadOnly);
										ImGui::EndDisabled();
									}
								}
								else
								{
									ImGui::BeginDisabled();
									char temp[100] = "None";
									ImGui::InputText("###", temp, 100, ImGuiInputTextFlags_ReadOnly);
									ImGui::EndDisabled();
								}

								ImGui::PushID(buttonID++);
								ImGui::SameLine();
								if (ImGui::Button("O"))
								{
									popupPosition = ImGui::GetCursorPos();
									popupPosition.x += ImGui::GetWindowPos().x;
									ImGui::SetNextWindowPos(popupPosition);
									ImGui::SetNextWindowSize(ImVec2(300.f, 175.f));

									ImVec4* colors = ImGui::GetStyle().Colors;
									colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.00f);

									ImGui::OpenPopup(("selectGameObject" + scriptValue.name).c_str());
								}

								bool unused_open = true;
								if (ImGui::BeginPopupModal(("selectGameObject" + scriptValue.name).c_str(), &unused_open, ImGuiWindowFlags_AlwaysVerticalScrollbar))
								{
									if (scriptValue.type == "ScriptAPI.GameObject")
									{
										if (ImGui::Selectable("None", false, ImGuiSelectableFlags_SpanAllColumns))
										{
											sceneManagerInstance->setGameObject(selectedEntity, scriptName, scriptValue.name, 0);
											ImGui::CloseCurrentPopup();
										}

										for (EntityID entityID : ecs.getEntities())
										{
											if (entityID == selectedEntity)
											{
												continue;
											}

											if (ImGui::Selectable(ecs.getComponent<NameTag>(entityID)->GetName().c_str(), entityID == selectedEntity, ImGuiSelectableFlags_SpanAllColumns))
											{
												sceneManagerInstance->setGameObject(selectedEntity, scriptName, scriptValue.name, entityID);
												ImGui::CloseCurrentPopup();
											}
										}
									}
									else // Script
									{
										if (ImGui::Selectable("None", false, ImGuiSelectableFlags_SpanAllColumns))
										{
											sceneManagerInstance->setScriptReference(selectedEntity, scriptName, scriptValue.name, 0, scriptValue.type);
											ImGui::CloseCurrentPopup();
										}

										for (EntityID entityID : ecs.getEntities())
										{
											if (entityID == selectedEntity)
											{
												continue;
											}
											if (sceneManagerInstance->hasScript(entityID, scriptValue.type) && ImGui::Selectable(ecs.getComponent<NameTag>(entityID)->GetName().c_str(), entityID == selectedEntity, ImGuiSelectableFlags_SpanAllColumns))
											{
												sceneManagerInstance->setScriptReference(selectedEntity, scriptName, scriptValue.name, entityID, scriptValue.type);
												ImGui::CloseCurrentPopup();
											}
										}
									}

									ImGui::EndPopup();
								}
								ImGui::PopID();

								ImGui::PopItemWidth();
							}
						}

						ImGui::PopItemWidth();
						ImGui::EndTable();
					}
					ImGui::TreePop();
				}
			}

			// all script names
			// call manage side to gimme variables that the script has

			// Add component button
			static ImGuiStyle& style = ImGui::GetStyle();

			float size = ImGui::CalcTextSize("Add Component").x + style.FramePadding.x * 2.0f;
			float avail = ImGui::GetContentRegionAvail().x;

			float off = (avail - size) * 0.5f;
			if (off > 0.0f)
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

			if (ImGui::Button("Add Component", ImVec2(100.f, 20.f)))
			{
				ImGui::OpenPopup("componentAddPopup");
			}

			if (ImGui::GetCursorPosY() < ImGui::GetWindowHeight())
			{
				ImGui::SetNextWindowPos(ImVec2(ImGui::GetContentRegionAvail().x / 2 + ImGui::GetWindowPos().x + style.FramePadding.x * 2.0f, ImGui::GetCursorPosY()), ImGuiCond_Appearing, ImVec2(0.5f, 0.0f));
			}
			else
			{
				ImGui::SetNextWindowPos(ImVec2(ImGui::GetContentRegionAvail().x / 2 + ImGui::GetWindowPos().x + style.FramePadding.x * 2.0f, ImGui::GetWindowHeight()), ImGuiCond_Appearing, ImVec2(0.5f, 0.0f));
			}
			ImGui::SetNextWindowSize(ImVec2(150.f, 0.f));
			
			if (ImGui::BeginPopupContextItem("componentAddPopup"))
			{
				for (auto componentName : allComponentNames)
				{
					if (!getComponentByName(componentName, selectedEntity) && ImGui::Selectable(componentName.c_str()))
					{
						addComponentByName(componentName, selectedEntity);
						TDS_INFO("Added Component");
					}
				}
				for (auto scriptName : sceneManagerInstance->getAllScripts())
				{
					if (!sceneManagerInstance->hasScript(selectedEntity, scriptName) && ImGui::Selectable(scriptName.c_str()))
					{
						sceneManagerInstance->addScript(selectedEntity, scriptName);
					}
				}
				ImGui::EndPopup();
			}
		}
	}

	/*!*************************************************************************
	This function is the helper function for Properties panel
	****************************************************************************/
	void Properties::ImguiComponentDisplay(std::string componentName, IComponent* componentBase)
	{

		rttr::type type = rttr::type::get_by_name(componentName);
		rttr::instance componentInstance = componentBase;

		for (rttr::property propertyName : type.get_properties())
		{
			if (propertyName.get_type() == rttr::type::get<int>())
			{
				int newValue = propertyName.get_value(componentInstance).convert<int>();
				ImguiInput(propertyName.get_name().to_string(), newValue);
				propertyName.set_value(componentInstance, newValue);
			}
			if (propertyName.get_type() == rttr::type::get<EntityID>())
			{
				int newValue = propertyName.get_value(componentInstance).convert<int>();
				ImguiInput(propertyName.get_name().to_string(), newValue);
				propertyName.set_value(componentInstance, newValue);
			}
			else if (propertyName.get_type() == rttr::type::get<float>())
			{
				float newValue = propertyName.get_value(componentInstance).convert<float>();
				ImguiInput(propertyName.get_name().to_string(), newValue);
				propertyName.set_value(componentInstance, newValue);
			}
			else if (propertyName.get_type() == rttr::type::get<bool>())
			{
				bool newValue = propertyName.get_value(componentInstance).convert<bool>();
				ImguiInput(propertyName.get_name().to_string(), newValue);
				propertyName.set_value(componentInstance, newValue);
			}
			else if (propertyName.get_type() == rttr::type::get<std::string>())
			{
				std::string newValue = propertyName.get_value(componentInstance).convert<std::string>();
				ImguiInput(propertyName.get_name().to_string(), newValue);

				// To drag drop texture name - will change and take out after
				if (componentName == "Graphics Component" && ImGui::BeginDragDropTarget())
				{
					GraphicsComponent* g = reinterpret_cast<GraphicsComponent*>(componentBase);
					std::string finaltexture = g->GetTextureName();
					std::string finalmodel = g->GetModelName();
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::wstring ws(path);
						// your new String
						std::string str(ws.begin(), ws.end());
						const std::filesystem::path filesystempath = str;

						if (propertyName.get_name() == "TextureName")
						{
							if (filesystempath.extension() == ".dds")
							{

								AssetBrowser assetbroswer;
								assetbroswer.getFileNameFromPath(str.c_str(), nullptr, nullptr, &finaltexture, nullptr);
								//g.m_TextureName = final;
								g->SetTextureName(finaltexture);

								std::wcout << " Path of dragged file is: " << path << std::endl;
							}
							else
							{
								TDS_INFO("invalid file type, please drag a .dds for TextureName");
							}

						}
						if (propertyName.get_name() == "ModelName")
						{
							if (filesystempath.extension() == ".bin")
							{

								AssetBrowser assetbroswer;
								assetbroswer.getFileNameFromPath(str.c_str(), nullptr, nullptr, &finalmodel, nullptr);
								//g.m_TextureName = final;
								g->SetModelName(finalmodel);

								std::wcout << " Path of dragged file is: " << path << std::endl;
							}
							else
							{
								TDS_INFO("invalid file type, please drag a .bin or .fbx for ModelName ");
							}
						}
					}
					else
					{
						propertyName.set_value(componentInstance, newValue);
					}
					ImGui::EndDragDropTarget();
				}
				else
				{
					propertyName.set_value(componentInstance, newValue);
				}
			}
			else if (propertyName.get_type() == rttr::type::get<Vec2>())
			{
				Vec2 newValue = propertyName.get_value(componentInstance).convert<Vec2>();
				ImguiInput(propertyName.get_name().to_string(), newValue);
				propertyName.set_value(componentInstance, newValue);
			}
			else if (propertyName.get_type() == rttr::type::get<Vec3>())
			{
				Vec3 newValue = propertyName.get_value(componentInstance).convert<Vec3>();
				ImguiInput(propertyName.get_name().to_string(), newValue);
				propertyName.set_value(componentInstance, newValue);
			}
			else if (propertyName.get_type() == rttr::type::get<Vec4>())
			{
				Vec4 newValue = propertyName.get_value(componentInstance).convert<Vec4>();
				ImguiInput(propertyName.get_name().to_string(), newValue);
				propertyName.set_value(componentInstance, newValue);
			}
			else if (propertyName.get_type() == rttr::type::get<RigidBody::MotionType>())
			{
				static std::vector<std::string> rigidbodyMotionTypeString = { "STATIC", "KINEMATIC", "DYNAMIC" };
				int newValue = propertyName.get_value(componentInstance).convert<int>();
				ImguiInput(propertyName.get_name().to_string(), rigidbodyMotionTypeString, newValue);
				propertyName.set_value(componentInstance, static_cast<RigidBody::MotionType>(newValue));
			}
		}
	}

	/*!*************************************************************************
	This function is a helper function for draw TEXT variables
	****************************************************************************/
	bool ImguiInput(std::string variableName, std::string& textVariable)
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text(variableName.c_str());

		ImGui::TableNextColumn();
		char temp[100];
		strcpy_s(temp, textVariable.c_str());
		ImGui::InputText(("##" + variableName).c_str(), temp, 100);

		if (ImGui::IsItemDeactivatedAfterEdit())
		{
			textVariable = std::string(temp);
			return true;
		}
		return false;
	}

	/*!*************************************************************************
	This function is a helper function for draw BOOl variables
	****************************************************************************/
	bool ImguiInput(std::string variableName, bool& boolVariable)
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text(variableName.c_str());

		ImGui::TableNextColumn();
		ImGui::Checkbox(("##" + variableName).c_str(), &boolVariable);

		if (ImGui::IsItemDeactivatedAfterEdit())
		{
			return true;
		}
		return false;
	}

	/*!*************************************************************************
	This function is a helper function for draw INT variables
	****************************************************************************/
	bool ImguiInput(std::string variableName, int& intVariable, float speed, int min, int max)
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text(variableName.c_str());

		ImGui::TableNextColumn();
		ImGui::InputInt(("##" + variableName).c_str(), &intVariable, 0/*, speed, min, max*/);

		if (ImGui::IsItemDeactivatedAfterEdit())
		{
			return true;
		}
		return false;
	}

	/*!*************************************************************************
	This function is a helper function for draw FLOAT variables
	****************************************************************************/
	bool ImguiInput(std::string variableName, float& floatVariable, float speed, float min, float max)
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text(variableName.c_str());

		ImGui::TableNextColumn();
		ImGui::InputFloat(("##" + variableName).c_str(), &floatVariable/*, speed, min, max*/);

		if (ImGui::IsItemDeactivatedAfterEdit())
		{
			return true;
		}
		return false;
	}

	/*!*************************************************************************
	This function is a helper function for draw VEC2 variables
	****************************************************************************/
	bool ImguiInput(std::string variableName, Vec2& Vec2Variable)
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text(variableName.c_str());

		ImGui::TableNextColumn();
		float temp[2]{ Vec2Variable.x, Vec2Variable.y };
		ImGui::InputFloat2(("##" + variableName).c_str(), temp/*, 0.05f*/);
		
		if (ImGui::IsItemDeactivatedAfterEdit())
		{
			Vec2Variable.x = temp[0];
			Vec2Variable.y = temp[1];

			return true;
		}
		return false;
	}

	/*!*************************************************************************
	This function is a helper function for draw VEC3 variables
	****************************************************************************/
	bool ImguiInput(std::string variableName, Vec3& Vec3Variable)
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text(variableName.c_str());

		ImGui::TableNextColumn();
		float temp[3]{ Vec3Variable.x, Vec3Variable.y, Vec3Variable.z };

		static ImGuiStyle& style = ImGui::GetStyle();
		auto tagWidth = ImGui::CalcTextSize("X").x + style.FramePadding.x * 2;
		auto inputWidth = (ImGui::GetColumnWidth() - tagWidth * 3) / 3.f;

		std::string format = "";

		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::PushItemWidth(inputWidth);
		format = (temp[0] == floor(temp[0]) ? "%.0f" : "%.3f");
		ImGui::InputFloat(("##" + variableName + "X").c_str(), &temp[0], 0.0f, 0.0f, format.c_str());
		ImGui::PopItemWidth();

		if (ImGui::IsItemDeactivatedAfterEdit())
		{
			Vec3Variable.x = temp[0];

			return true;
		}

		ImGui::SameLine();
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::PushItemWidth(inputWidth);
		format = (temp[1] == floor(temp[1]) ? "%.0f" : "%.3f");
		ImGui::InputFloat(("##" + variableName + "Y").c_str(), &temp[1], 0.0f, 0.0f, format.c_str());
		ImGui::PopItemWidth();

		if (ImGui::IsItemDeactivatedAfterEdit())
		{
			Vec3Variable.y = temp[1];

			return true;
		}

		ImGui::SameLine();
		ImGui::Text("Z");
		ImGui::SameLine();
		//ImGui::PushItemWidth(inputWidth);
		ImGui::PushItemWidth(-FLT_MIN); // Right-aligned
		format = (temp[2] == floor(temp[2]) ? "%.0f" : "%.3f");
		ImGui::InputFloat(("##" + variableName + "Z").c_str(), &temp[2], 0.0f, 0.0f, format.c_str());
		ImGui::PopItemWidth();

		if (ImGui::IsItemDeactivatedAfterEdit())
		{
			Vec3Variable.z = temp[2];

			return true;
		}
		return false;
	}

	/*!*************************************************************************
	This function is a helper function for draw VEC4 variables
	****************************************************************************/
	bool ImguiInput(std::string variableName, Vec4& Vec4Variable)
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text(variableName.c_str());

		ImGui::TableNextColumn();
		float temp[4]{ Vec4Variable.x, Vec4Variable.y, Vec4Variable.z, Vec4Variable.w };

		for (auto& c : variableName)
		{
			c = std::tolower(c);
		}

		if (variableName.find("color") != std::string::npos)
		{
			ImGui::ColorEdit4(("##" + variableName).c_str(), temp);
			Vec4Variable.x = temp[0];
			Vec4Variable.y = temp[1];
			Vec4Variable.z = temp[2];
			Vec4Variable.w = temp[3];
			return true;
		}

		ImGui::InputFloat4(("##" + variableName).c_str(), temp);

		if (ImGui::IsItemDeactivatedAfterEdit())
		{
			Vec4Variable.x = temp[0];
			Vec4Variable.y = temp[1];
			Vec4Variable.z = temp[2];
			Vec4Variable.w = temp[3];

			return true;
		}
		return false;
	}

	/*!*************************************************************************
	This function is a helper function for draw ENUM variables
	****************************************************************************/
	bool ImguiInput(std::string variableName, std::vector<std::string>& enumString, int& enumVariable)
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text(variableName.c_str());

		ImGui::TableNextColumn();

		// Uninitialized
		if (enumVariable == -1)
		{
			enumVariable = 0;
		}

		if (ImGui::BeginCombo(("##" + variableName).c_str(), enumString[enumVariable].c_str()))
		{
			for (int n = 0; n < enumString.size(); n++)
			{
				const bool is_selected = (enumVariable == n);
				if (ImGui::Selectable(enumString[n].c_str(), is_selected))
				{
					enumVariable = n;
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		if (ImGui::IsItemDeactivatedAfterEdit())
		{
			return true;
		}
		return false;
	}
}