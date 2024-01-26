#include "Rendering/GridRenderer.h"
#include "GridManager/GridManager.h"
#include "vulkanTools/VulkanPipeline.h"
#include "Rendering/GraphicsManager.h"
#include "vulkanTools/GlobalBufferPool.h"
#include "GridManager/GridManager.h"
#include "vulkanTools/Renderer.h"
namespace TDS
{
	GridManagerBase gridbase{};
	struct GridUniformBuffer
	{
		Mat4			m_Projection{};
		Mat4			m_View{};
	}gridubo2;
	void GridRenderer::Init()
	{
		//m_GridPipeline = std::make_shared<VulkanPipeline>();
		////m_DebugPipeline = std::make_shared<VulkanPipeline>();

		//m_GridModel.m_AssetName = "cube_Bin.bin";
		////m_GridModel.m_ResourcePtr = ;

		//if (!AssetManager::GetInstance()->GetMeshFactory().GetMeshController("cube_Bin.bin", m_GridModel))
		//{
		//	TDS_ERROR("FAILED TO LOAD GRID!");
		//	return;
		//}
		//

		//m_VertexBuffer = std::make_shared<VMABuffer>();
		//std::vector<Vec3> pos(m_GridModel.m_ResourcePtr->GetVertexData().size());

		//for (size_t i = 0; i < m_GridModel.m_ResourcePtr->GetVertexData().size(); ++i)
		//{
		//	pos[i] = m_GridModel.m_ResourcePtr->GetVertexData().at(i).m_Pos;
		//}

		//m_VertexBuffer->CreateVertexBuffer(pos.size() * sizeof(Vec3), true, pos.data());
		//m_VertexBuffer->SetDataCnt((std::uint32_t)pos.size());

		//m_IndexBuffer = std::make_shared<VMABuffer>();
		//m_IndexBuffer->CreateIndexBuffer(m_GridModel.m_ResourcePtr->GetIndexData().size() * sizeof(uint32_t), true, m_GridModel.m_ResourcePtr->GetIndexData().data());
		//m_IndexBuffer->SetDataCnt((std::uint32_t)m_GridModel.m_ResourcePtr->GetIndexData().size());

		//PipelineCreateEntry GridEntry{};
		//PipelineCreateEntry DebugEntry{};
		//GridEntry.m_NumDescriptorSets = 1;
		//DebugEntry.m_NumDescriptorSets = 1;

		//DebugEntry.m_ShaderInputs.m_Shaders.insert(std::make_pair(SHADER_FLAG::VERTEX, "../assets/shaders/GridLineVertex.spv"));
		//DebugEntry.m_ShaderInputs.m_Shaders.insert(std::make_pair(SHADER_FLAG::FRAGMENT, "../assets/shaders/GridLineFrag.spv"));
		//GridEntry.m_ShaderInputs.m_Shaders.insert(std::make_pair(SHADER_FLAG::VERTEX, "../assets/shaders/GridVertex.spv"));
		//GridEntry.m_ShaderInputs.m_Shaders.insert(std::make_pair(SHADER_FLAG::FRAGMENT, "../assets/shaders/GridFrag.spv"));
		//VertexLayout layout0 =
		//	VertexLayout(
		//		{
		//		  VertexBufferElement(VAR_TYPE::VEC3, "vPosition")
		//		});
		//GridEntry.m_ShaderInputs.m_InputVertex.push_back(VertexBufferInfo(false, layout0, sizeof(Vec3)));
		//DebugEntry.m_ShaderInputs.m_InputVertex.push_back(VertexBufferInfo(false, layout0, sizeof(Vec3)));
		//BufferInfo info1{};
		//info1.m_Data = nullptr;
		//info1.m_Size = sizeof(GridUniform) * m_GridUniform.size();
		//info1.m_Static = false;

		//GridEntry.m_ShaderInputs.m_InputBuffers.insert(std::make_pair(27, info1));
		//BufferInfo info2{};
		//info2.m_Data = nullptr;
		//info2.m_Size = sizeof(GridLineUniform) * m_GridLineUniform.size();
		//info2.m_Static = false;

		//DebugEntry.m_ShaderInputs.m_InputBuffers.insert(std::make_pair(30, info2));
		//m_GridPipeline->Create(GridEntry);
		////m_DebugPipeline->Create(DebugEntry);

		////trying to make sure getmapinfo no longer throws exception
		////m_pGridManager = GridManager::GetInstance();

	}
	void GridRenderer::Render(VkCommandBuffer commandBuffer, uint32_t frameIndex)
	{
		//auto cmdBuffer = GraphicsManager::getInstance().getCommandBuffer();
		//m_DebugPipeline->SetCommandBuffer(cmdBuffer);
		m_GridPipeline->SetCommandBuffer(commandBuffer/*cmdBuffer*/);
		//std::uint32_t frame = GraphicsManager::getInstance().GetSwapchainRenderer().getFrameIndex();
		//std::cout << "Grid of rows: " << gridbase.GetMapInfo().m_NumRows << " and cols: " << gridbase.GetMapInfo().m_NumCols << " is being rendered" << std::endl;
		

		for (int i = 0; i < gridbase.GetMapInfo().m_NumRows; ++i)
		{
			for (int j = 0; j < gridbase.GetMapInfo().m_NumCols; ++j)
			{
				//auto cmdBuffer = GraphicsManager::getInstance().getCommandBuffer();

				int index = i * gridbase.GetMapInfo().m_NumCols + j;
				Vec2 CellDimension = gridbase.GetMapInfo().CellDimension;
				Vec3 WorldPosition = gridbase.GetWorldPos(i, j);

				/*m_GridUniform[index].model = Mat4::identity();
				m_GridUniform[index].model *= Mat4::Translate(WorldPosition);
				m_GridUniform[index].model *= Mat4::Scale(Vec3(CellDimension.x, 1.f, CellDimension.y));
				m_GridLineUniform[index].view = GraphicsManager::getInstance().GetCamera().GetViewMatrix();
				m_GridLineUniform[index].proj = Mat4::Perspective(GraphicsManager::getInstance().GetCamera().m_Fov * Mathf::Deg2Rad,
					GraphicsManager::getInstance().GetSwapchainRenderer().getAspectRatio(), 0.1f, 10.f);
				m_GridLineUniform[index].proj.m[1][1] *= -1;*/


				//std::cout << "world pos is: X:" << WorldPosition.x << " y: " << WorldPosition.y << " Z: " << WorldPosition.z << std::endl;


			}
		}
				//test with new ubo
				Mat4 TransformMatrix;
				TransformMatrix = Mat4::identity() * Mat4::Translate(Vec3(1,1,1)) * Mat4::Scale(Vec3(100.0f, 1.f, 100.0));
				gridubo2.m_View = GraphicsManager::getInstance().GetCamera().GetViewMatrix() * TransformMatrix;
				gridubo2.m_Projection = Mat4::Perspective(GraphicsManager::getInstance().GetCamera().m_Fov * Mathf::Deg2Rad,
					GraphicsManager::getInstance().GetSwapchainRenderer().getAspectRatio(), 0.1f, 10.f);
				gridubo2.m_Projection.m[1][1] *= -1;

		//m_DebugPipeline->UpdateUBO(m_GridLineUniform.data(), 30, sizeof(GridLineUniform) * m_GridLineUniform.size(), frame);

		//m_DebugPipeline->BindPipeline();
		//m_DebugPipeline->BindVertexBuffer(*m_VertexBuffer);
		//m_DebugPipeline->BindIndexBuffer(*m_IndexBuffer);
		//m_DebugPipeline->DrawInstancedIndexed(*m_VertexBuffer, *m_IndexBuffer, gridbase.GetMapInfo().TotalNumberOfGrids(), frame);

		m_GridPipeline->BindPipeline();
		m_GridPipeline->BindDescriptor(frameIndex, 1, 0);
		//m_GridPipeline->UpdateUBO(m_GridUniform.data(), 27, sizeof(GridUniform) * m_GridUniform.size(), frameIndex); //original code causes bining error

		m_GridPipeline->UpdateUBO(&gridubo2, sizeof(GridUniformBuffer/*GridUniform*/) /** m_GridUniform.size()*/, 27, frameIndex);

		m_GridPipeline->BindVertexBuffer(*m_VertexBuffer);
		m_GridPipeline->BindIndexBuffer(*m_IndexBuffer);
		m_GridPipeline->DrawInstancedIndexed(*m_VertexBuffer, *m_IndexBuffer, gridbase.GetMapInfo().TotalNumberOfGrids(), frameIndex);
		//m_GridPipeline->DrawIndexed(*m_VertexBuffer, *m_IndexBuffer, frameIndex);

	}
	void GridRenderer::ShutDown()
	{
		m_GridPipeline->ShutDown();
		//m_DebugPipeline->ShutDown();

	}
	void GridRenderer::SetColour(int row, int col, Color color)
	{
		int index = row * gridbase.GetMapInfo().m_NumCols + col;
		m_GridUniform[index].gridColor = color.GetAsVec4();
	}
	DLL_API std::shared_ptr<GridRenderer> GridRenderer::GetInstance()
	{
		if (m_Instance == nullptr)
			m_Instance = std::make_shared<GridRenderer>();
		return m_Instance;
	}
}