/*!*****************************************************************************
 * \file          ModelFactory.h
 * \author        Eugene Ho Shu Sheng
 * \par DP email: shushengeugene.ho@digipen.edu
 * \par Course:   CSD3400
 * \par Section:  A
 * \date          01/10/2023
 * \brief         This file contains the declaration of the ModelFactory class.
 *******************************************************************************/
#pragma once
#include "FactoryBase.h"
#include <string_view>
#include <vector>
#include <filesystem>
#include <fstream>
#include <iostream>

#define MODEL_PATH "../assets/models/"
#define MAX_PRELOAD_MODELS 5

namespace TDS
{
	/*!*************************************************************************
	 * template class ModelFactory
	 ***************************************************************************/
	template<>
	class AssetFactory<AssetModel>
	{
	public:
		std::array<const char*, 6> m_PrimitiveModels = { "capsule_bin.bin", "cube_bin.bin", "Quad1_bin.bin",
			"Quad2_bin.bin","sphere_bin.bin", "torus_bin.bin" };

		std::array<AssetModel, 4096> m_Models;
		std::unordered_map<std::string, std::uint32_t> m_ModelIndices; //Map with string and instance ID/index
		std::unordered_map<std::string, std::uint32_t> m_InstanceCnt; //Each instance count
		std::uint32_t m_CurrentIndex = 0;


		std::array<AssetModel, 4096>& GetModelArray()
		{
			return m_Models;
		}
		AssetModel* GetModel(std::string_view modelName, TypeReference<AssetModel>& model)
		{
			auto itr = m_ModelIndices.find(modelName.data());
			if (itr != m_ModelIndices.end())
			{
				--m_InstanceCnt[model.m_AssetName];
				model.m_AssetName = modelName;
				model.m_ResourcePtr = &m_Models[itr->second];
				++m_InstanceCnt[modelName.data()];
				return model.m_ResourcePtr;
			}
			return nullptr;
			TDS_WARN("Model doesnt exist!");
		}

		/*!*************************************************************************
		 * Deserialize the model from the file.
		 ***************************************************************************/
		static void DeserializeGeom(Geom& geomOut, std::string_view PathData)
		{
			std::ifstream inFile(PathData.data(), std::ios::binary);
			if (!inFile)
			{
				std::cerr << "ERROR: Could not open file for reading!\n";
				return;
			}

			auto deserializeVec3 = [](std::ifstream& in, Vec3& vec)
			{
				in.read(reinterpret_cast<char*>(&vec.x), sizeof(vec.x));
				in.read(reinterpret_cast<char*>(&vec.y), sizeof(vec.y));
				in.read(reinterpret_cast<char*>(&vec.z), sizeof(vec.z));
			};

			auto deserializeVec2 = [](std::ifstream& in, Vec2& vec)
			{
				in.read(reinterpret_cast<char*>(&vec.x), sizeof(vec.x));
				in.read(reinterpret_cast<char*>(&vec.y), sizeof(vec.y));
			};

			auto deserializeMesh = [](std::ifstream& in, Geom::Mesh& mesh)
			{
				in.read(reinterpret_cast<char*>(&mesh.m_Name), sizeof(mesh.m_Name));
			};

			auto deserializeSubMesh = [](std::ifstream& in, Geom::SubMesh& subMesh)
			{
				in.read(reinterpret_cast<char*>(&subMesh.m_nFaces), sizeof(subMesh.m_nFaces));
				in.read(reinterpret_cast<char*>(&subMesh.m_iIndices), sizeof(subMesh.m_iIndices));
				in.read(reinterpret_cast<char*>(&subMesh.m_iVertices), sizeof(subMesh.m_iVertices));
				in.read(reinterpret_cast<char*>(&subMesh.m_nVertices), sizeof(subMesh.m_nVertices));
				in.read(reinterpret_cast<char*>(&subMesh.m_iMaterial), sizeof(subMesh.m_iMaterial));
			};

			auto deserializeExtraVertices = [&](std::ifstream& in, Geom::ExtraVertices& extraVertices)
			{
				deserializeVec2(in, extraVertices.m_UV);
				deserializeVec3(in, extraVertices.m_Normal);
				deserializeVec3(in, extraVertices.m_Tanget);
				deserializeVec3(in, extraVertices.m_Bitangent);
				in.read(reinterpret_cast<char*>(&extraVertices.m_Colour), sizeof(extraVertices.m_Colour));
			};

			uint32_t meshSize;
			inFile.read(reinterpret_cast<char*>(&meshSize), sizeof(meshSize));
			geomOut.m_Mesh.resize(meshSize);
			for (auto& mesh : geomOut.m_Mesh)
			{
				deserializeMesh(inFile, mesh);
			}

			uint32_t subMeshSize;
			inFile.read(reinterpret_cast<char*>(&subMeshSize), sizeof(subMeshSize));
			geomOut.m_SubMesh.resize(subMeshSize);
			for (auto& subMesh : geomOut.m_SubMesh)
			{
				deserializeSubMesh(inFile, subMesh);
			}

			uint32_t posSize;
			inFile.read(reinterpret_cast<char*>(&posSize), sizeof(posSize));
			geomOut.m_Pos.resize(posSize);
			for (auto& pos : geomOut.m_Pos)
			{
				deserializeVec3(inFile, pos);
			}

			uint32_t extraSize;
			inFile.read(reinterpret_cast<char*>(&extraSize), sizeof(extraSize));
			geomOut.m_Extra.resize(extraSize);
			for (auto& extra : geomOut.m_Extra)
			{
				deserializeExtraVertices(inFile, extra);
			}

			uint32_t indicesSize;
			inFile.read(reinterpret_cast<char*>(&indicesSize), sizeof(indicesSize));
			geomOut.m_Indices.resize(indicesSize);
			inFile.read(reinterpret_cast<char*>(geomOut.m_Indices.data()), indicesSize * sizeof(uint32_t));

			inFile.close();
		}

		void PreloadDefaultPrimitives()
		{
			for (auto& primitive : m_PrimitiveModels)
			{
				Geom newGeom{};
				std::string Path = MODEL_PATH;
				Path += primitive;
				std::string primitiveName = std::filesystem::path(Path).filename().string();
				DeserializeGeom(newGeom, Path);
				m_Models[m_CurrentIndex].LoadGeomData(newGeom);
				m_ModelIndices[primitiveName] = m_CurrentIndex++;
				m_InstanceCnt[primitiveName] = 1;
				
			}
		}
		/*!*************************************************************************
		 * Preload the model from the file.
		 ***************************************************************************/
		/*void Preload(ResourceAllocator& resourceMgr)
		{
			std::filesystem::path dir = MODEL_PATH;

			if (!std::filesystem::exists(dir) || !std::filesystem::is_directory(dir))
			{
				std::cout << "Invalid directory" << std::endl;
				return;
			}
			std::uint32_t numLoadedModels = 0;
			for (const auto& entry : std::filesystem::directory_iterator(dir))
			{
				if (numLoadedModels >= MAX_PRELOAD_MODELS)
					break;


				const std::filesystem::path& path = entry.path();

				if (path.extension() == ".bin")
				{
					Geom newGeom{};
					TypeReference<AssetModel> modelInstance{};

					DeserializeGeom(newGeom, entry.path().string());
					std::string fileName = path.filename().string();
					modelInstance.m_AssetName = fileName;
					auto newModel = resourceMgr.LoadResource(modelInstance);
					if (newModel == nullptr)
					{
						std::cout << "Failed to create model instance!" << std::endl;
						continue;
					}
					newModel->LoadGeomData(newGeom);


					++numLoadedModels;


				}
			}
		}*/
		void Preload()
		{
			std::filesystem::path dir = MODEL_PATH;

			if (!std::filesystem::exists(dir) || !std::filesystem::is_directory(dir))
			{
				std::cout << "Invalid directory" << std::endl;
				return;
			}
			std::uint32_t numPreLoadedModels = 0;
			for (const auto& entry : std::filesystem::directory_iterator(dir))
			{
				if (numPreLoadedModels >= MAX_PRELOAD_MODELS)
					break;


				const std::filesystem::path& path = entry.path();

				if (path.extension() == ".bin")
				{
					Geom newGeom{};
					

					DeserializeGeom(newGeom, entry.path().string());
					std::string fileName = path.filename().string();
					
					m_Models[m_CurrentIndex].LoadGeomData(newGeom);
					m_ModelIndices[fileName.data()] = m_CurrentIndex++;
					m_InstanceCnt[fileName.data()] = 1;
					++numPreLoadedModels;


				}
			}
		}
		/*!*************************************************************************
		 * Loading the model from the file.
		 ***************************************************************************/
		//static void Load(std::string_view path, TypeReference<AssetModel>& model, ResourceAllocator& resourceMgr)
		//{

		//	std::filesystem::path FilePath(path);
		//	std::string fileName = FilePath.filename().string();
		//	model.m_AssetName = fileName;
		//	if (!resourceMgr.GetResource(model))
		//	{
		//		if (!resourceMgr.LoadResource(model))
		//		{
		//			TDS_WARN("Failed to load resource!");
		//			return;
		//		}
		//	}


		//	Geom geom{};
		//	DeserializeGeom(geom, path);

		//	model.m_ResourcePtr->LoadGeomData(geom);


		//}

		static void Load(std::string_view path, TypeReference<AssetModel>& model, AssetFactory<AssetModel>& modelFactory)
		{
			std::filesystem::path FilePath(path);
			std::string fileName = FilePath.filename().string();
		
			
			auto& modelIndices = modelFactory.m_ModelIndices;
			auto& modelArray = modelFactory.m_Models;
			auto& instanceContainer = modelFactory.m_InstanceCnt;

			auto itr = modelIndices.find(fileName);

			if (itr != modelIndices.end())
			{
				--instanceContainer[model.m_AssetName];
				model.m_AssetName = fileName;
				TDS_INFO("Model {} is already loaded!", model.m_AssetName);
				++instanceContainer[fileName];
				model.m_ResourcePtr = &modelArray[itr->second];
				return;
			}

			//its a new model
			model.m_AssetName = fileName;
			std::uint32_t& newIndex = modelFactory.m_CurrentIndex;
			Geom geom{};
			DeserializeGeom(geom, path);
			modelArray[newIndex].LoadGeomData(geom);
			model.m_ResourcePtr = &modelArray[newIndex];
			modelIndices[fileName] = newIndex++;
			instanceContainer[fileName] = 1;
		}

		void DestroyAllModels()
		{
			for (auto& model : m_Models)
			{
				model.Destroy();
			}
			m_ModelIndices.clear();
		}

	};



}