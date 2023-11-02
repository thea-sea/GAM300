/*!*************************************************************************
****
\file sceneManager.h
\author Go Ruo Yan
\par DP email: ruoyan.go@digipen.edu
\date 28-9-2023
\brief  This program declares the functions in the sceneManager class
****************************************************************************
***/

#ifndef SCENEMANAGER
#define SCENEMANAGER

#include <filesystem>

#include "ecs/ecs.h"
#include "components/components.h"

namespace TDS
{
	void fromJsonRecur(rttr::instance& obj, rapidjson::Value& jsonObj);

	void ToJsonRecur(const rttr::instance& obj, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);

	bool writeVariant(const rttr::variant& var, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);

	class SceneManager : public Serializer
	{
	public:
		struct ScriptReferenceVariable
		{
			EntityID entityHoldingScript;
			std::string scriptName;
			std::string variableName;
			EntityID entityScriptReference;
			std::string scriptReference;
		};

		/*!*************************************************************************
		Returns an instance of the SceneManager
		****************************************************************************/
		DLL_API static std::unique_ptr<SceneManager>& GetInstance();

		/*!*************************************************************************
		Initialize all registered components
		****************************************************************************/
		DLL_API void Init();
		/*!*************************************************************************
		Deserializes ECS entities and data from JSON file to build ECS (File loading)
		****************************************************************************/
		DLL_API virtual bool Deserialize(const rapidjson::Value& obj, rapidjson::Document& doc);
		/*!*************************************************************************
		Serializes ECS entities and data to JSON file to save ECS (File saving)
		****************************************************************************/
		DLL_API virtual bool Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const;

		/*!*************************************************************************
		This function serializes scenes into JSON files
		****************************************************************************/
		DLL_API bool sceneSerialize();
		/*!*************************************************************************
		This function deserializes scenes from JSON files
		****************************************************************************/
		DLL_API bool sceneDeserialize();
		
		/*!*************************************************************************
		This function finds scripts in project
		****************************************************************************/
		DLL_API void scriptsDeserialize(std::string filepath = "");

		/*!*************************************************************************
		This function adds a new scene into the Scene Manager
		****************************************************************************/
		DLL_API void newScene(std::string scene);
		/*!*************************************************************************
		This function loads given scene
		****************************************************************************/
		DLL_API void loadScene(std::string scene);

		/*!*************************************************************************
		This function saves the current scene
		****************************************************************************/
		DLL_API void saveCurrentScene();
		/*!*************************************************************************
		This function deletes the given scene
		****************************************************************************/
		DLL_API void deleteScene(std::string scene);

		/*!*************************************************************************
		This function is the getter function for current scene
		****************************************************************************/
		DLL_API std::string getCurrentScene();
		/*!*************************************************************************
		This function is the setter function for current scene
		****************************************************************************/
		DLL_API void setCurrentScene(std::string _currentScene);
		/*!*************************************************************************
		This function is the getter function for current scene saved
		****************************************************************************/
		DLL_API bool getCurrentSceneSaved();
		/*!*************************************************************************
		This function is the setter function for current scene saved
		****************************************************************************/
		DLL_API void setCurrentSceneSaved(bool _currentSceneSaved);

		/*!*************************************************************************
		This function renames a file
		****************************************************************************/
		DLL_API static bool stringCompare(std::string a, std::string b);
		DLL_API bool renameScene(std::string oldName, std::string newName);

		/*!*************************************************************************
		This function is the getter function for all scenes in Scene Browser
		****************************************************************************/
		//DLL_API std::vector<std::string>& getScenes();

		/*!*************************************************************************
		This function is the getter function for the path to scenes
		****************************************************************************/
		DLL_API std::string getScenePath();

		/*!*************************************************************************
		This function is the setter function to reset starting scene
		****************************************************************************/
		DLL_API std::string getStartingScene();

		/*!*************************************************************************
		This function is the setter function to reset starting scene
		****************************************************************************/
		DLL_API void setStartingScene(std::string newStartScene);

		bool isGameplay;

		// Function pointers from scriptAPI
		std::vector<TDS::ScriptValues>(*getScriptVariables)(EntityID entityID, std::string scriptName);
		bool(*hasScript)(EntityID entityID, std::string scriptName);
		std::vector<std::string>(*getAllScripts)();

		bool (*addScript)(EntityID entityID, std::string scriptName);

		void (*setBool)(EntityID entityID, std::string script, std::string variableName, bool value);
		void (*setInt)(EntityID entityID, std::string script, std::string variableName, int value);
		void (*setDouble)(EntityID entityID, std::string script, std::string variableName, double value);
		void (*setFloat)(EntityID entityID, std::string script, std::string variableName, float value);
		void (*setString)(EntityID entityID, std::string script, std::string variableName, std::string value);
		//void (*setChar)(EntityID entityID, std::string script, std::string variableName, char value);
		void (*setGameObject)(EntityID entityID, std::string script, std::string variableName, EntityID gameObjectEntityID);
		void (*setScriptReference)(EntityID entityID, std::string script, std::string variableName, EntityID gameObjectEntityID, std::string scriptReference);

		bool (*updateName)(EntityID entityID, std::string newName);

	private:
		// Unique pointer to SceneManager
		static std::unique_ptr<SceneManager> m_instance;

		// For Scenes
		std::vector<std::string> allScenes;
		std::string startScene;

		std::string currentScene;
		bool currentSceneSaved;

		std::string parentFilePath;
		std::string filePath;

		// For scripts
		std::vector<std::string> allScripts;
		std::string scriptFilePath;
	};
}

#endif