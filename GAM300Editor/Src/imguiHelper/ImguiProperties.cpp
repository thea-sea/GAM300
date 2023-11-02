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
#include "components/rigidBody.h"

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
				ImGuiTreeNodeFlags_Selected;

			int i = 0;
			for (auto componentName : allComponentNames)
			{
				IComponent* componentBase = getComponentByName(componentName, selectedEntity);

				if (componentName == "Name Tag" && ImGui::BeginTable("###", 2))
				{
					auto nameTagComponent = reinterpret_cast<NameTag*>(componentBase);
					nameTagComponent->SetName(ImguiInput("", nameTagComponent->GetName()));
					sceneManagerInstance->updateName(selectedEntity, ecs.getComponent<NameTag>(selectedEntity)->GetName());
					//ImguiInput("", static_cast<int>(hierarchyPanel->hierarchyMap[selectedEntity].parent));
					//ImguiInput("", hierarchyPanel->hierarchyMap[selectedEntity].indexInParent);

					ImGui::EndTable();

					continue;
				}

				if (!componentBase)
				{
					continue;
				}

				bool componentOpen = ImGui::TreeNodeEx(componentName.c_str(), nodeFlags);

				ImGui::PushID(i);
				if (ImGui::BeginPopupContextItem("componentEditPopup"))
				{
					if (componentName != "Name Tag" && componentName != "Transform" && ImGui::Selectable("Remove Component"))
					{
						removeComponentByName(componentName, selectedEntity);
						TDS_INFO("Removed Component");
					}

					ImGui::EndPopup();
				}

				if (rightClick && ImGui::IsItemHovered())
				{
					ImGui::OpenPopup("componentEditPopup");
				}
				ImGui::PopID();
				++i;

				

				if (componentOpen)
				{
					if (ImGui::BeginTable("###", 2, /*ImGuiTableFlags_Borders |*/ ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_SizingStretchSame, ImVec2(0.0f, 5.5f)))
					{
						ImGui::TableSetupColumn("Component variable name", ImGuiTableColumnFlags_None, ImGui::GetContentRegionAvail().x * 0.4f);
						ImGui::TableSetupColumn("Component variable value", ImGuiTableColumnFlags_None, ImGui::GetContentRegionAvail().x * 0.6f);

						ImGui::TableNextRow();

						ImGui::TableNextColumn();
						ImGui::TableNextColumn();
						ImGui::PushItemWidth(-FLT_MIN); // Right-aligned

						ImguiComponentDisplay(componentName, componentBase);

						ImGui::PopItemWidth();
						ImGui::EndTable();
					}
					ImGui::TreePop();
				}

				//set the EMotion Type
				if (componentName == "Rigid Body")
				{
					const char* motion_types[] = { "None", "Static", "Kinematic", "Dynamic" };
					const char* selected = motion_types[0]; //set to none by default
					if (ImGui::BeginCombo("Motion Type: ", RigidBody::current_motion_type))
					{
						for (int n = 0; n < IM_ARRAYSIZE(motion_types); n++)
						{
							bool is_selected = (selected == motion_types[n]); // You can store your selection however you want, outside or inside your objects
							if (ImGui::Selectable(motion_types[n], is_selected))
							{
								selected = motion_types[n];
								if (is_selected)
									ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)

							}
						}
						RigidBody::current_motion_type = selected;
						//RigidBody::UpdateMotionType(); //unresolved external? TO DO

						ImGui::EndCombo();
					}

				}
			}

			int buttonID = 0;
			for (auto scriptName : sceneManagerInstance->getAllScripts())
			{
				if (!sceneManagerInstance->hasScript(selectedEntity, scriptName))
				{
					continue;
				}

				if (ImGui::TreeNodeEx(scriptName.c_str(), nodeFlags))
				{
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
								value = ImguiInput(scriptValue.name, value);
								sceneManagerInstance->setBool(selectedEntity, scriptName, scriptValue.name, value);
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
								value = ImguiInput(scriptValue.name, value);
								sceneManagerInstance->setInt(selectedEntity, scriptName, scriptValue.name, value);
							}
							else if (scriptValue.type == "System.Double")
							{
								float value = std::stod(scriptValue.value);
								value = ImguiInput(scriptValue.name, value);
								sceneManagerInstance->setDouble(selectedEntity, scriptName, scriptValue.name, static_cast<double>(value));
							}
							else if (scriptValue.type == "System.Single")
							{
								float value = std::stod(scriptValue.value);
								value = ImguiInput(scriptValue.name, value);
								sceneManagerInstance->setFloat(selectedEntity, scriptName, scriptValue.name, value);
							}
							else if (scriptValue.type == "System.String")
							{
								std::string value = scriptValue.value;
								value = ImguiInput(scriptValue.name, value);
								sceneManagerInstance->setString(selectedEntity, scriptName, scriptValue.name, value);
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
										strcpy_s(temp, (ecs.getComponent<NameTag>(scriptValue.referenceEntityID)->GetName() + " (" + scriptName + ")").c_str());
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
				propertyName.set_value(componentInstance, ImguiInput(propertyName.get_name().to_string(), propertyName.get_value(componentInstance).convert<int>()));
			}
			if (propertyName.get_type() == rttr::type::get<EntityID>())
			{
				propertyName.set_value(componentInstance, ImguiInput(propertyName.get_name().to_string(), propertyName.get_value(componentInstance).convert<int>()));
			}
			else if (propertyName.get_type() == rttr::type::get<float>())
			{
				propertyName.set_value(componentInstance, ImguiInput(propertyName.get_name().to_string(), propertyName.get_value(componentInstance).convert<float>()));
			}
			else if (propertyName.get_type() == rttr::type::get<bool>())
			{
				propertyName.set_value(componentInstance, ImguiInput(propertyName.get_name().to_string(), propertyName.get_value(componentInstance).convert<bool>()));
			}
			else if (propertyName.get_type() == rttr::type::get<std::string>())
			{
				propertyName.set_value(componentInstance, ImguiInput(propertyName.get_name().to_string(), propertyName.get_value(componentInstance).convert<std::string>()));
			}
			else if (propertyName.get_type() == rttr::type::get<Vec2>())
			{
				propertyName.set_value(componentInstance, ImguiInput(propertyName.get_name().to_string(), propertyName.get_value(componentInstance).convert<Vec2>()));
			}
			else if (propertyName.get_type() == rttr::type::get<Vec3>())
			{
				propertyName.set_value(componentInstance, ImguiInput(propertyName.get_name().to_string(), propertyName.get_value(componentInstance).convert<Vec3>()));
			}
			else if (propertyName.get_type() == rttr::type::get<Vec4>())
			{
				propertyName.set_value(componentInstance, ImguiInput(propertyName.get_name().to_string(), propertyName.get_value(componentInstance).convert<Vec4>()));
			}
			else if (propertyName.get_type() == rttr::type::get<RigidBody::MotionType>())
			{
				static std::vector<std::string> rigidbodyMotionTypeString = { "NONE", "STATIC", "KINEMATIC", "DYNAMIC" };
				int temp = propertyName.get_value(componentInstance).convert<int>();
				propertyName.set_value(componentInstance, static_cast<RigidBody::MotionType>(ImguiInput(propertyName.get_name().to_string(), rigidbodyMotionTypeString, propertyName.get_value(componentInstance).convert<int>())));
			}
		}
	}

	/*!*************************************************************************
	This function is a helper function for draw TEXT variables
	****************************************************************************/
	std::string ImguiInput(std::string variableName, std::string textVariable)
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text(variableName.c_str());

		ImGui::TableNextColumn();
		char temp[100];
		strcpy_s(temp, textVariable.c_str());
		ImGui::InputText(("##" + variableName).c_str(), temp, 100);

		return std::string(temp);
	}

	/*!*************************************************************************
	This function is a helper function for draw BOOl variables
	****************************************************************************/
	bool ImguiInput(std::string variableName, bool boolVariable)
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text(variableName.c_str());

		ImGui::TableNextColumn();
		ImGui::Checkbox(("##" + variableName).c_str(), &boolVariable);

		return boolVariable;
	}

	/*!*************************************************************************
	This function is a helper function for draw INT variables
	****************************************************************************/
	int ImguiInput(std::string variableName, int intVariable, float speed, int min, int max)
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text(variableName.c_str());

		ImGui::TableNextColumn();
		ImGui::InputInt(("##" + variableName).c_str(), &intVariable, 0/*, speed, min, max*/);

		return intVariable;
	}

	/*!*************************************************************************
	This function is a helper function for draw FLOAT variables
	****************************************************************************/
	float ImguiInput(std::string variableName, float floatVariable, float speed, float min, float max)
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text(variableName.c_str());

		ImGui::TableNextColumn();
		ImGui::InputFloat(("##" + variableName).c_str(), &floatVariable/*, speed, min, max*/);

		return floatVariable;
	}

	/*!*************************************************************************
	This function is a helper function for draw VEC2 variables
	****************************************************************************/
	Vec2 ImguiInput(std::string variableName, Vec2 Vec2Variable)
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text(variableName.c_str());

		ImGui::TableNextColumn();
		float temp[2]{ Vec2Variable.x, Vec2Variable.y };
		ImGui::InputFloat2(("##" + variableName).c_str(), temp/*, 0.05f*/);
		Vec2Variable.x = temp[0];
		Vec2Variable.y = temp[1];

		return Vec2Variable;
	}

	/*!*************************************************************************
	This function is a helper function for draw VEC3 variables
	****************************************************************************/
	Vec3 ImguiInput(std::string variableName, Vec3 Vec3Variable)
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

		ImGui::SameLine();
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::PushItemWidth(inputWidth);
		format = (temp[1] == floor(temp[1]) ? "%.0f" : "%.3f");
		ImGui::InputFloat(("##" + variableName + "Y").c_str(), &temp[1], 0.0f, 0.0f, format.c_str());
		ImGui::PopItemWidth();

		ImGui::SameLine();
		ImGui::Text("Z");
		ImGui::SameLine();
		//ImGui::PushItemWidth(inputWidth);
		ImGui::PushItemWidth(-FLT_MIN); // Right-aligned
		format = (temp[2] == floor(temp[2]) ? "%.0f" : "%.3f");
		ImGui::InputFloat(("##" + variableName + "Z").c_str(), &temp[2], 0.0f, 0.0f, format.c_str());
		ImGui::PopItemWidth();

		Vec3Variable.x = temp[0];
		Vec3Variable.y = temp[1];
		Vec3Variable.z = temp[2];

		return Vec3Variable;
	}

	/*!*************************************************************************
	This function is a helper function for draw VEC4 variables
	****************************************************************************/
	Vec4 ImguiInput(std::string variableName, Vec4 Vec4Variable)
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text(variableName.c_str());

		ImGui::TableNextColumn();
		float temp[4]{ Vec4Variable.x, Vec4Variable.y, Vec4Variable.z, Vec4Variable.w };
		if (variableName == "Color")
		{
			ImGui::ColorEdit4(("##" + variableName).c_str(), temp);
		}
		else
		{
			ImGui::InputFloat4(("##" + variableName).c_str(), temp);
		}
		Vec4Variable.x = temp[0];
		Vec4Variable.y = temp[1];
		Vec4Variable.z = temp[2];
		Vec4Variable.w = temp[3];

		return Vec4Variable;
	}

	/*!*************************************************************************
	This function is a helper function for draw VEC4 variables
	****************************************************************************/
	int ImguiInput(std::string variableName, std::vector<std::string>& enumString, int enumVariable)
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

		return enumVariable;
	}
}