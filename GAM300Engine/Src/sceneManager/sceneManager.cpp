/*!*************************************************************************
****
\file sceneManager.cpp
\author Go Ruo Yan
\par DP email: ruoyan.go@digipen.edu
\date 28-9-2023
\brief  This program defines the functions in the SceneManager class
****************************************************************************
***/

#include <fstream>

#include "sceneManager/sceneManager.h"
#include "propertyManager/registration.h"

namespace TDS
{
	// Unique pointer to instance of Scene
	std::unique_ptr<SceneManager> SceneManager::m_instance;

	/*!*************************************************************************
	Returns an instance of the SceneManager
	****************************************************************************/
	std::unique_ptr<SceneManager>& SceneManager::GetInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = std::make_unique<SceneManager>();

			std::filesystem::path currentPath = std::filesystem::current_path();
			while (!std::filesystem::exists(currentPath.string() + "\\assets"))
			{
				if (currentPath == "C:\\")
				{
					std::cout << "No asset directory found" << std::endl;
					break;
				}
				currentPath = currentPath.parent_path();
			}
			m_instance->parentFilePath = currentPath.string() + "\\assets\\";
			m_instance->filePath = currentPath.string() + "\\assets\\scenes\\";
			m_instance->scriptFilePath = currentPath.string() + "\\ManagedScripts\\";
		}
		return m_instance;
	}

	/*!*************************************************************************
	Initialize all registered components
	****************************************************************************/
	void SceneManager::Init()
	{
		ecs.registerComponent<NameTag>("Name Tag");
		ecs.registerComponent<Transform>("Transform");

		ecs.registerComponent<AI>("AI");
		ecs.registerComponent<BoxCollider>("Box Collider");
		ecs.registerComponent<CameraComponent>("Camera Component");
		ecs.registerComponent<CapsuleCollider>("Capsule Collider");
		ecs.registerComponent<GraphicsComponent>("Graphics Component");
		ecs.registerComponent<RigidBody>("Rigid Body");
		ecs.registerComponent<SphereCollider>("Sphere Collider");
		ecs.registerComponent<Sprite>("Sprite");
		ecs.registerComponent<Tag>("Tag");
		ecs.registerComponent<WinData>("Win Data");
		ecs.registerComponent<GraphicsComponent>("Graphics Component");

		startScene = "";
		//startScene = "MainMenu";

		//Entity entity1;
		//ecs.addComponent<NameTag>(entity1.getID());
		//ecs.getComponent<NameTag>(entity1.getID())->SetName("entity1");
		//ecs.addComponent<Transform>(entity1.getID());
		//ecs.addComponent<GraphicsComponent>(entity1.getID());
		//Entity entity2;
		//ecs.addComponent<NameTag>(entity2.getID());
		//ecs.getComponent<NameTag>(entity1.getID())->SetName("entity2");
		//ecs.addComponent<Transform>(entity2.getID());
		//ecs.addComponent<GraphicsComponent>(entity2.getID());
		//Entity entity3;
		//ecs.addComponent<NameTag>(entity3.getID());
		//ecs.getComponent<NameTag>(entity1.getID())->SetName("entity3");
		//ecs.addComponent<Transform>(entity3.getID());
		//ecs.addComponent<GraphicsComponent>(entity3.getID());
		//Entity entity4;
		//ecs.addComponent<NameTag>(entity4.getID());
		//ecs.getComponent<NameTag>(entity1.getID())->SetName("entity4");
		//ecs.addComponent<Transform>(entity4.getID());
		//ecs.addComponent<GraphicsComponent>(entity4.getID());

		//SerializeToFile(filePath + "MainMenu.json");

		//ecs.removeAllEntities();

		//ecs.removeEntity(entity4.getID());
		//SerializeToFile(filePath + "Game.json");

		bindSystemFunctions();
		allScripts = getAllScripts();
		// Setting default scene
		sceneDeserialize();
	}

	/*!*************************************************************************
	Deserializes ECS entities and data from JSON file to build ECS (File loading)
	****************************************************************************/
	bool SceneManager::Deserialize(const rapidjson::Value& obj, rapidjson::Document& doc)
	{
		obj.GetObject();
		for (rapidjson::Value::ConstMemberIterator itr = obj["Archetype Sizes"].MemberBegin(); itr != obj["Archetype Sizes"].MemberEnd(); ++itr)
		{
			std::string archetypeID = itr->name.GetString();
			auto archetypeSizes = itr->value.GetObject();

			ecs.addArchetype(archetypeID, false);

			for (rapidjson::Value::ConstMemberIterator componentItr = archetypeSizes.MemberBegin(); componentItr != archetypeSizes.MemberEnd(); ++componentItr)
			{
				int componentID = std::stoi(componentItr->name.GetString());
				auto componentSize = componentItr->value.GetInt();

				ecs.setComponentSize(archetypeID, componentID, componentSize);
			}

			ecs.commitArchetype(archetypeID);
		}

		EntityID lastEntity = 0;
		int i = 0;
		for (rapidjson::Value::ConstMemberIterator itr = obj["Entity Data"].MemberBegin(); itr != obj["Entity Data"].MemberEnd(); ++itr, ++i)
		{
			if (itr == obj["Entity Data"].MemberEnd())
			{
				continue;
			}

			//EntityID newEntity = ecs.getNewID();
			lastEntity = static_cast<EntityID>(std::stoi(itr->name.GetString()));
			ecs.registerEntity(lastEntity);

			for (auto& m : itr->value.GetObject())	
			{
				std::string componentName = m.name.GetString();
				if (componentName == "ArchetypeID") // First "componentName" to immediately find the archetype of entity
				{
					// Add all components at once
					ecs.addComponentsByArchetype(lastEntity, m.value.GetString());

					continue;
				}

				rapidjson::Value componentData;
				componentData.CopyFrom(m.value, doc.GetAllocator());

				rttr::type component = rttr::type::get_by_name(componentName);

				rttr::instance addedComponent = getComponentByName(component, lastEntity);
				fromJsonRecur(addedComponent, componentData);
			}

			updateName(lastEntity, ecs.getComponent<NameTag>(lastEntity)->GetName());
		}

		ecs.setIDCounter(lastEntity + 1);

		std::vector<ScriptReferenceVariable> scriptReferences;
		for (rapidjson::Value::ConstMemberIterator itr = obj["Scripts"].MemberBegin(); itr != obj["Scripts"].MemberEnd(); ++itr, ++i)
		{
			EntityID currentEntity = static_cast<EntityID>(std::stoi(itr->name.GetString()));

			for (auto& script : itr->value.GetObject())
			{
				std::string scriptName = script.name.GetString();

				std::cout << "script" << std::endl;
				addScript(currentEntity, scriptName);

				for (auto& variable : script.value.GetObject())
				{
					std::string variableName = variable.name.GetString();
					auto variableTypeValue = variable.value.GetObject();

					std::string variableType = variableTypeValue.MemberBegin()->name.GetString();

					if (variableType == "Bool")
					{
						bool value = variableTypeValue.MemberBegin()->value.GetBool();
						setBool(currentEntity, scriptName, variableName, value);
					}
					else if (variableType == "Int")
					{
						int value = variableTypeValue.MemberBegin()->value.GetInt();
						setInt(currentEntity, scriptName, variableName, value);
					}
					else if (variableType == "Double")
					{
						double value = variableTypeValue.MemberBegin()->value.GetDouble();
						setDouble(currentEntity, scriptName, variableName, value);
					}
					else if (variableType == "Float")
					{
						float value = variableTypeValue.MemberBegin()->value.GetDouble();
						setFloat(currentEntity, scriptName, variableName, value);
					}
					else if (variableType == "String")
					{
						std::string value = variableTypeValue.MemberBegin()->value.GetString();
						setString(currentEntity, scriptName, variableName, value);
					}
					else if (variableType == "Char")
					{
						//std::string value = variableTypeValue.MemberBegin()->value.GetString();
						//if (value != "")
						//{
						//	setChar(currentEntity, scriptName, variableName, value[0]);
						//}
					}
					else if (variableType == "GameObject")
					{
						EntityID value = variableTypeValue.MemberBegin()->value.GetDouble();
						if (value > 0)
						{
							setGameObject(currentEntity, scriptName, variableName, value);
						}
					}
					else if (variableTypeValue.MemberBegin()->value.GetInt() > 0) // scripts (saving to add later)
					{
						scriptReferences.emplace_back(ScriptReferenceVariable(currentEntity, scriptName, variableName, variableTypeValue.MemberBegin()->value.GetInt(), variableType));
					}
				}
			}
		}

		for (auto reference : scriptReferences)
		{
			setScriptReference(reference.entityHoldingScript, reference.scriptName, reference.variableName, reference.entityScriptReference, reference.scriptReference);
		}

		return true;
	}

	/*!*************************************************************************
	Serializes ECS entities and data to JSON file to save ECS (File saving)
	****************************************************************************/
	bool SceneManager::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const
	{
		writer->StartObject();

		// Serialize archetype sizes first
		writer->String("Archetype Sizes", static_cast<rapidjson::SizeType>(std::string("Archetype Sizes").length()), false);

		writer->StartObject();

		for (Archetype* a : ecs.getAllArchetypes())
		{
			if (!a->entityIds.size())
			{
				continue;	// there is no entities under this archetype
			}

			writer->String(a->type.c_str(), static_cast<rapidjson::SizeType>(a->type.length()), false);

			// Start of data sizes
			writer->StartObject();

			for (unsigned int i = 0; i < ecs.getNumberOfComponents(); ++i)
			{
				if (a->type[i] == '1')
				{
					writer->Key(std::to_string(i).c_str());
					writer->Int(a->componentDataSize[i]);
				}
			}

			writer->EndObject();
		}

		// End of archetype sizes
		writer->EndObject();

		// =======================================================
		// Serialize entity data
		writer->String("Entity Data", static_cast<rapidjson::SizeType>(std::string("Entity Data").length()), false);
		writer->StartObject();

		std::string entityNo = "EntityNo";
		//std::string component = "Component";

		std::vector<EntityID> entityList = ecs.getEntities();

		for (int i = 0; i < entityList.size(); ++i)
		{
			writer->String(std::to_string(entityList[i]).c_str(), static_cast<rapidjson::SizeType>(std::to_string(entityList[i]).length()), false);
			writer->StartObject();

			std::string archetype = ecs.getArchetypeID(entityList[i]);

			writer->Key("ArchetypeID");
			writer->String(archetype.c_str());

			for (std::string j : ecs.getEntityComponents(entityList[i]))
			{
				writer->String(j.c_str(), static_cast<rapidjson::SizeType>(j.length()), false);

				rttr::type type = rttr::type::get_by_name(j);
				ToJsonRecur(getComponentByName(type, entityList[i]), *writer);
			}

			writer->EndObject();
		}

		// End of entity data
		writer->EndObject();

		// =======================================================
		// Start of scripts
		writer->String("Scripts", static_cast<rapidjson::SizeType>(std::string("Scripts").length()), false);
		writer->StartObject();

		for (int i = 0; i < entityList.size(); ++i)
		{
			// EntityID
			writer->String(std::to_string(entityList[i]).c_str(), static_cast<rapidjson::SizeType>(std::to_string(entityList[i]).length()), false);
			writer->StartObject();

			for (std::string scriptName : allScripts)
			{
				if (hasScript(entityList[i], scriptName))
				{
					writer->String(scriptName.c_str(), static_cast<rapidjson::SizeType>(scriptName.length()), false);
					writer->StartObject();

					std::vector<ScriptValues> allValues = getScriptVariables(entityList[i], scriptName);
					for (ScriptValues& scriptValues : allValues)
					{
						writer->String(scriptValues.name.c_str(), static_cast<rapidjson::SizeType>(scriptValues.name.length()), false);
						writer->StartObject();

						if (scriptValues.type == "System.Boolean")
						{
							writer->String("Bool", static_cast<rapidjson::SizeType>(std::string("Bool").length()), false);
							scriptValues.value == "False" ? writer->Bool(false) : writer->Bool(true);
						}
						else if (scriptValues.type == "System.Int16"
							|| scriptValues.type == "System.Int32"
							|| scriptValues.type == "System.Int64"
							|| scriptValues.type == "System.UInt16"
							|| scriptValues.type == "System.UInt32"
							|| scriptValues.type == "System.UInt64"
							|| scriptValues.type == "System.Byte"
							|| scriptValues.type == "System.SByte")
						{
							writer->String("Int", static_cast<rapidjson::SizeType>(std::string("Int").length()), false);
							scriptValues.value == "" ? writer->Int(0) : writer->Int(std::stoi(scriptValues.value));
						}
						else if (scriptValues.type == "System.Double")
						{
							writer->String("Double", static_cast<rapidjson::SizeType>(std::string("Double").length()), false);
							scriptValues.value == "" ? writer->Double(0) : writer->Double(std::stod(scriptValues.value));
						}
						else if (scriptValues.type == "System.Single")
						{
							writer->String("Float", static_cast<rapidjson::SizeType>(std::string("Float").length()), false);
							scriptValues.value == "" ? writer->Double(0) : writer->Double(std::stod(scriptValues.value));
						}
						else if (scriptValues.type == "System.String")
						{
							writer->String("String", static_cast<rapidjson::SizeType>(std::string("String").length()), false);
							writer->String(scriptValues.value.c_str(), static_cast<rapidjson::SizeType>(scriptValues.value.length()), false);
						}
						else if (scriptValues.type == "ScriptAPI.GameObject")
						{
							writer->String("GameObject", static_cast<rapidjson::SizeType>(std::string("GameObject").length()), false);
							writer->Int(scriptValues.referenceEntityID);
						}
						else if (scriptValues.type == "System.Char")
						{
							writer->String("Char", static_cast<rapidjson::SizeType>(std::string("Char").length()), false);
							writer->String(scriptValues.value.c_str(), static_cast<rapidjson::SizeType>(scriptValues.value.length()), false);
						}
						else // scripts 
						{
							// To Do 
							writer->String(scriptValues.type.c_str(), static_cast<rapidjson::SizeType>(scriptValues.type.length()), false);
							writer->Int(scriptValues.referenceEntityID);
						}
						writer->EndObject();
					}

					writer->EndObject();
				}
			}

			writer->EndObject();
		}

		// End of scripts
		writer->EndObject();

		// End of file
		writer->EndObject();

		return true;
	}

	/*!*************************************************************************
	This function serializes scenes into JSON files
	****************************************************************************/
	bool SceneManager::sceneSerialize()
	{
		std::ofstream ofs(parentFilePath + "scene.json");

		rapidjson::StringBuffer sb;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("start", 5, false);
		writer.String(startScene.c_str(), static_cast<rapidjson::SizeType>(startScene.length()), false);

		writer.EndObject();

		ofs << sb.GetString();
		ofs.flush();
		ofs.close();

		return true;
	}

	/*!*************************************************************************
	This function deserializes scenes from JSON files
	****************************************************************************/
	bool SceneManager::sceneDeserialize()
	{
		std::string allScenesFilepath = parentFilePath + "scene.json";
		std::ifstream ifs(allScenesFilepath, std::ios::in);
		std::stringstream buffer;
		buffer << ifs.rdbuf();
		ifs.close();

		rapidjson::Document doc;
		doc.Parse(buffer.str().c_str());

		if (!InitDocument(buffer.str().c_str(), doc))
		{
			return false;
		}

		rapidjson::Value& value = doc.GetObject();
		
		// Starting scene
		rapidjson::Value::MemberIterator theItr = value.FindMember("start");
		std::string startingScene = theItr->value.GetString();

		bool fileCheck = false;
		for (auto& directory_entry : std::filesystem::directory_iterator(filePath))
		{
			if (directory_entry.path().extension() == ".json" && startingScene == directory_entry.path().stem().string()) // found
			{
				loadScene(startingScene);
				fileCheck = true;
			}
		}
		if (!fileCheck) // entry scene not found
		{
			newScene("NewScene");
			startingScene = "NewScene";
			loadScene("NewScene");
		}

		currentScene = startingScene;
		startScene = startingScene;
		currentSceneSaved = true;

		return true;
	}

	/*!*************************************************************************
	This function finds scripts in project
	****************************************************************************/
	void SceneManager::scriptsDeserialize(std::string filepath)
	{
		if (filepath == "")
		{
			filepath = scriptFilePath;
		}

		for (auto& directory_entry : std::filesystem::directory_iterator(filepath))
		{
			if (directory_entry.path().extension() == ".cs")
			{
				allScripts.emplace_back(directory_entry.path().stem().string());
			}
			if (directory_entry.is_directory() && directory_entry.path().filename() != "obj" && directory_entry.path().filename() != ".bin")
			{
				scriptsDeserialize(directory_entry.path().string());
			}
		}
	}

	bool SceneManager::stringCompare(std::string a, std::string b)
	{
		for (int index = 0; index < a.size() && index < b.size(); ++index)
		{
			char lowerA = std::tolower(a[index]);
			char lowerB = std::tolower(b[index]);

			if (lowerA < lowerB)
			{
				return true;
			}
			else if (lowerA > lowerB)
			{
				return false;
			}
		}
		if (a.size() > b.size())
		{
			return false;
		}
		return true;
	}

	/*!*************************************************************************
	This function adds a new scene into the Scene Manager
	****************************************************************************/
	void SceneManager::newScene(std::string scene)
	{
		//allScenes.emplace_back(scene);

		if (!std::filesystem::exists(filePath + scene + ".json"))
		{
			std::ofstream ofs(filePath + scene + ".json");
			ofs.close();
		}

		//std::sort(allScenes.begin(), allScenes.end(), stringCompare);
	}
	/*!*************************************************************************
	This function loads given scene
	****************************************************************************/
	void SceneManager::loadScene(std::string scene)
	{
		ecs.removeAllEntities();
		DeserializeFromFile(filePath + scene + ".json");
		currentScene = scene;
		currentSceneSaved = true;
	}

	/*!*************************************************************************
	This function saves the given scene 
	****************************************************************************/
	void SceneManager::saveCurrentScene()
	{
		setCurrentSceneSaved(true);

		SerializeToFile(filePath + currentScene + ".json");

		//if (std::find(allScenes.begin(), allScenes.end(), currentScene) == allScenes.end())
		//{
		//	newScene(currentScene);
		//}
	}
	/*!*************************************************************************
	This function deletes the given scene
	****************************************************************************/
	void SceneManager::deleteScene(std::string scene)
	{
		if (scene == currentScene)
		{
			setCurrentSceneSaved(false);
		}

		std::filesystem::remove(filePath + scene + ".json");
		//auto sceneInVector = std::find(allScenes.begin(), allScenes.end(), scene);

		//if (sceneInVector != allScenes.end())
		//{
		//	allScenes.erase(sceneInVector);
		//}
	}

	/*!*************************************************************************
	This function is the getter function for current scene
	****************************************************************************/
	std::string SceneManager::getCurrentScene()
	{
		return currentScene;
	}

	/*!*************************************************************************
	This function is the setter function for current scene
	****************************************************************************/
	void SceneManager::setCurrentScene(std::string _currentScene)
	{
		currentScene = _currentScene;
	}
	/*!*************************************************************************
	This function is the getter function for current scene saved
	****************************************************************************/
	bool SceneManager::getCurrentSceneSaved()
	{
		return currentSceneSaved;
	}
	/*!*************************************************************************
	This function is the setter function for current scene saved
	****************************************************************************/
	void SceneManager::setCurrentSceneSaved(bool _currentSceneSaved)
	{
		if (!_currentSceneSaved && currentSceneSaved)
		{
			currentScene += "*";
		}
		else if (_currentSceneSaved && !currentSceneSaved)
		{
			currentScene.erase(currentScene.end() - 1);
		}

		currentSceneSaved = _currentSceneSaved;
	}

	/*!*************************************************************************
	This function renames a file
	****************************************************************************/
	bool SceneManager::renameScene(std::string oldName, std::string newName)
	{
		//if (std::find(allScenes.begin(), allScenes.end(), newName) != allScenes.end())
		//{
		//	return false;
		//}
		if (std::filesystem::exists(filePath + newName + ".json"))
		{
			return false;
		}

		std::filesystem::rename(filePath + oldName + ".json", filePath + newName + ".json");
		//std::replace(allScenes.begin(), allScenes.end(), oldName, newName);
		//std::sort(allScenes.begin(), allScenes.end(), stringCompare);
		return true;
	}

	/*!*************************************************************************
	This function is the getter function for all scenes in Scene Browser
	****************************************************************************/
	//std::vector<std::string>& SceneManager::getScenes()
	//{
	//	return allScenes;
	//}

	/*!*************************************************************************
	This function is the getter function for the path to scenes
	****************************************************************************/
	std::string SceneManager::getScenePath()
	{
		return filePath;
	}

	/*!*************************************************************************
	This function is the setter function to reset starting scene
	****************************************************************************/
	DLL_API std::string SceneManager::getStartingScene()
	{
		return startScene;
	}

	/*!*************************************************************************
	This function is the setter function to reset starting scene
	****************************************************************************/
	void SceneManager::setStartingScene(std::string newStartScene)
	{
		startScene = newStartScene;
		sceneSerialize();
	}
}