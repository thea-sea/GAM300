#include "Tools/ShaderReflector.h"
#include "../ToolsDependencies/glslang/Public/ShaderLang.h"
#include "../ToolsDependencies/glslang/SPIRV/GlslangToSpv.h"
#include "../ToolsDependencies/spirv_cross/spirv_cross.hpp"
#include "../ToolsDependencies/spirv_cross/spirv_glsl.hpp"


namespace TDS
{

	void ShaderReflector::Init(std::string_view srcFolder, std::string_view OutFolder)
	{
		StartReflectingAll(srcFolder);

		Serialize(OutFolder);

		CleanUp();

	}
	void ShaderReflector::StartReflectingAll(std::string_view Path)
	{
		std::filesystem::path dirPath(Path);

		if (!std::filesystem::exists(dirPath) || !std::filesystem::is_directory(dirPath))
		{
			// Handle error, provided path is not a valid directory.
			std::cerr << "Invalid directory path: " << Path << std::endl;
			return;
		}

		for (const auto& entry : std::filesystem::directory_iterator(dirPath))
		{
			if (entry.is_regular_file() && entry.path().extension() == ".spv")
			{
				std::string fileName = entry.path().filename().string();

				std::vector<std::uint32_t> spirvData;
				LoadSPIRVBinaries(entry.path().string(), spirvData);
				m_Output.m_ShaderDatas[fileName].m_ShaderName = fileName;
				ReflectUniform(spirvData, fileName);
				ReflectSSBO(spirvData, fileName);
				ReflectPushConstants(spirvData, fileName);
				ReflectSampler(spirvData, fileName);
				ReflectStorageImages(spirvData, fileName);
			}
		}
	}
	void ShaderReflector::LoadSPIRVBinaries(const std::string_view fileName, std::vector<std::uint32_t>& spirv)
	{
		std::ifstream in(fileName.data(), std::ios::in | std::ios::binary);
		if (in.is_open())
		{
			in.seekg(0, std::ios::end);
			auto size = in.tellg();
			in.seekg(0, std::ios::beg);

			spirv.resize(size / sizeof(uint32_t));
			in.read((char*)spirv.data(), size);
		}
	}

	void ShaderReflector::ReflectUniform(const std::vector<std::uint32_t>& spirvData, const std::string_view fileName)
	{
		spirv_cross::Compiler compiler(spirvData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();
		for (auto& ub : resources.uniform_buffers)
		{
			auto& type = compiler.get_type(ub.base_type_id);
			std::uint32_t bufferCnt = std::uint32_t(compiler.get_type(ub.base_type_id).member_types.size());
			Uniform uniformObject{};
			uniformObject.m_BufferType = BUFFER_TYPE::UNIFORM;
			uniformObject.m_BindingPoint = compiler.get_decoration(ub.id, spv::DecorationBinding);
			uniformObject.m_Size = compiler.get_declared_struct_size(type);
			uniformObject.m_InstanceName = compiler.get_name(ub.id);

			for (unsigned i = 0; i < bufferCnt; ++i)
			{
				BufferVariable uniformBuffer{};
				uniformBuffer.m_Name = compiler.get_member_name(type.self, i);
				uniformBuffer.m_Offset = compiler.type_struct_member_offset(type, i);
				uniformBuffer.m_Size = compiler.get_declared_struct_member_size(type, i);
				uniformObject.m_UniformsBuffer.emplace_back(uniformBuffer);

			}
			m_Output.m_ShaderDatas[fileName.data()].m_ReflectedData.m_UniformBuffers[uniformObject.m_InstanceName] =
				uniformObject;

		}
	}

	void ShaderReflector::ReflectSSBO(const std::vector<std::uint32_t>& spirvData, const std::string_view fileName)
	{
		spirv_cross::Compiler compiler(spirvData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		for (auto& sb : resources.storage_buffers)
		{
			auto& type = compiler.get_type(sb.base_type_id);
			std::uint32_t bufferCnt = std::uint32_t(compiler.get_type(sb.base_type_id).member_types.size());
			Uniform storageBuffer{};
			storageBuffer.m_InstanceName = compiler.get_name(sb.id);
			storageBuffer.m_BindingPoint = compiler.get_decoration(sb.id, spv::DecorationBinding);
			storageBuffer.m_Size = compiler.get_declared_struct_size(type);
			storageBuffer.m_BufferType = BUFFER_TYPE::SSBO;

			for (unsigned i = 0; i < bufferCnt; ++i)
			{
				BufferVariable storageBufferVar{};
				storageBufferVar.m_Name = compiler.get_member_name(type.self, i);
				storageBufferVar.m_Offset = compiler.type_struct_member_offset(type, i);
				storageBufferVar.m_Size = compiler.get_declared_struct_member_size(type, i);
				storageBuffer.m_UniformsBuffer.push_back(storageBufferVar);
			}
			m_Output.m_ShaderDatas[fileName.data()].m_ReflectedData.m_UniformBuffers[storageBuffer.m_InstanceName] =
				storageBuffer;
		}
	}

	void ShaderReflector::ReflectPushConstants(const std::vector<std::uint32_t>& spirvData, const std::string_view fileName)
	{
		spirv_cross::Compiler compiler(spirvData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		for (auto& pc : resources.push_constant_buffers)
		{
			auto& type = compiler.get_type(pc.base_type_id);
			PushContantReflect pushConstantInfo{};
			pushConstantInfo.m_Offset = 0;
			pushConstantInfo.m_Size = std::uint32_t(compiler.get_declared_struct_size(type));
			m_Output.m_ShaderDatas[fileName.data()].m_ReflectedData.m_PushConstant = pushConstantInfo;
		}
	}

	void ShaderReflector::ReflectSampler(const std::vector<std::uint32_t>& spirvData, const std::string_view fileName)
	{
		spirv_cross::Compiler compiler(spirvData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		for (auto& sb : resources.sampled_images)
		{
			auto& type = compiler.get_type(sb.base_type_id);
			SamplerBuffer sampleBuffer{};
			sampleBuffer.m_BufferType = BUFFER_TYPE::SAMPLER;
			sampleBuffer.m_BindingPoint = compiler.get_decoration(sb.id, spv::DecorationBinding);
			sampleBuffer.m_Location = compiler.get_decoration(sb.id, spv::DecorationLocation);
			sampleBuffer.m_Dimension = compiler.get_type(sb.type_id).image.dim;
			sampleBuffer.m_ArraySize = compiler.get_type(sb.type_id).array[0];
			sampleBuffer.m_Name = compiler.get_name(sb.id);
			m_Output.m_ShaderDatas[fileName.data()].m_ReflectedData.m_ImageSamplers[sampleBuffer.m_Name] = sampleBuffer;
		}

	}

	void ShaderReflector::ReflectStorageImages(const std::vector<std::uint32_t>& spirvData, const std::string_view fileName)
	{
		spirv_cross::Compiler compiler(spirvData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		for (auto& sb : resources.storage_images)
		{
			auto& type = compiler.get_type(sb.base_type_id);
			SamplerBuffer StorageBuffer{};
			StorageBuffer.m_BufferType = BUFFER_TYPE::STORAGE_IMAGE;
			StorageBuffer.m_BindingPoint = compiler.get_decoration(sb.id, spv::DecorationBinding);
			StorageBuffer.m_Location = compiler.get_decoration(sb.id, spv::DecorationLocation);
			StorageBuffer.m_ArraySize = compiler.get_type(sb.type_id).array[0];
			StorageBuffer.m_Name = compiler.get_name(sb.id);
			m_Output.m_ShaderDatas[fileName.data()].m_ReflectedData.m_StorageImages[StorageBuffer.m_Name] = StorageBuffer;
		}
	}

	void ShaderReflector::Serialize(std::string_view Output)
	{
		std::ofstream file(Output.data(), std::ios::binary);
		if (!file.is_open())
		{
			// Handle error - file could not be opened.
			return;
		}

		auto& metadata = m_Output;
		std::size_t dataSize = metadata.m_ShaderDatas.size();
		file.write(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));

		auto writeString = [&file](const std::string& str) {
			std::size_t length = str.size();
			file.write(reinterpret_cast<const char*>(&length), sizeof(length));
			file.write(str.data(), length);
		};

		auto writeSamplerBufferMap = [&file, &writeString](const std::map<std::string, SamplerBuffer>& map)
		{
			std::size_t mapSize = map.size();
			file.write(reinterpret_cast<const char*>(&mapSize), sizeof(mapSize));

			for (const auto& [key, value] : map)
			{
				writeString(key);
				file.write(reinterpret_cast<const char*>(&value.m_Location), sizeof(std::uint32_t));
				file.write(reinterpret_cast<const char*>(&value.m_BindingPoint), sizeof(std::uint32_t));
				file.write(reinterpret_cast<const char*>(&value.m_Dimension), sizeof(std::uint32_t));
				file.write(reinterpret_cast<const char*>(&value.m_ArraySize), sizeof(std::uint32_t));
				writeString(value.m_Name);
				file.write(reinterpret_cast<const char*>(&value.m_BufferType), sizeof(BUFFER_TYPE));
			}
		};

		for (const auto& pair : metadata.m_ShaderDatas)
		{
			const auto& key = pair.first;
			const auto& value = pair.second;

			auto stageType = static_cast<int32_t>(value.m_StageType);
			file.write(reinterpret_cast<const char*>(&stageType), sizeof(stageType));
			writeString(key);
			writeString(value.m_ShaderName);
			const auto& reflectedData = value.m_ReflectedData;
			file.write(reinterpret_cast<const char*>(&reflectedData.m_PushConstant), sizeof(PushContantReflect));
			writeSamplerBufferMap(reflectedData.m_ImageSamplers);
			writeSamplerBufferMap(reflectedData.m_StorageImages);

			std::size_t uniformSize = reflectedData.m_UniformBuffers.size();
			file.write(reinterpret_cast<const char*>(&uniformSize), sizeof(uniformSize));

			for (const auto& [key, uniform] : reflectedData.m_UniformBuffers)
			{
				writeString(key);
				file.write(reinterpret_cast<const char*>(&uniform.m_BufferType), sizeof(BUFFER_TYPE));
				file.write(reinterpret_cast<const char*>(&uniform.m_BindingPoint), sizeof(uint32_t));
				writeString(uniform.m_InstanceName);
				file.write(reinterpret_cast<const char*>(&uniform.m_Size), sizeof(size_t));

				std::size_t bufferVariableSize = uniform.m_UniformsBuffer.size();
				file.write(reinterpret_cast<const char*>(&bufferVariableSize), sizeof(bufferVariableSize));

				for (const auto& bufferVariable : uniform.m_UniformsBuffer)
				{
					file.write(reinterpret_cast<const char*>(&bufferVariable.m_Size), sizeof(size_t));
					file.write(reinterpret_cast<const char*>(&bufferVariable.m_Offset), sizeof(size_t));
					writeString(bufferVariable.m_Name);
					file.write(reinterpret_cast<const char*>(&bufferVariable.m_Var), sizeof(VAR_TYPE));
				}
			}
		}
		file.close();
	}




	void ShaderReflector::CleanUp()
	{
		for (auto data : m_Output.m_ShaderDatas)
		{
			data.second.m_ReflectedData.Reset();
		}
	}

}