#pragma once
#include "vulkanTools/VMATypes/VMABuffer.h"
#include "TDSMath.h"
namespace TDS
{
	enum class GRID_STATE
	{
		CLEAR = 0,
		WALL = 1
	};


	struct GridData
	{
		int m_Row = 10, m_Col = 10;
		GRID_STATE m_State;
	};

	struct Map
	{
		Vec2 CellDimension;
	};
	struct GridMap
	{
		int m_MapWidth{5000}, m_MapHeight{ 3000 };
		static inline int m_NumRows = 50;
		static inline int m_NumCols = 30;
		Vec2 CellDimension = { 100.f, 100.f };

		inline int TotalNumberOfGrids() const
		{
			return m_NumRows * m_NumCols;
		}

	};

	class GridManagerBase
	{
		GridMap m_GridMap;
	private:
		std::vector<std::vector<GridData>> m_Grids;

	public:
		DLL_API GridManagerBase();
		DLL_API ~GridManagerBase();

		DLL_API inline GridMap& GetMapInfo()
		{
			return m_GridMap;
		}
		

		DLL_API void CreateTileMap(std::uint32_t Width, std::uint32_t Height);

		DLL_API void ResizeMap(std::uint32_t Width, std::uint32_t Height);

		DLL_API void SetNumberOfGrids(int NumRows, int numCols);

		DLL_API void SetTileState(GRID_STATE tileColor, Vec2 TilePos);

		DLL_API void UpdateGridSize();

		DLL_API void DeserializeGridData(std::string_view path);

		DLL_API void SerializeGridData(std::string_view path);

		DLL_API Vec3 GetWorldPos(int Row, int Col);

		DLL_API std::pair<int, int> GetGridPos(Vec3 worldPos);

		inline static std::shared_ptr<GridManagerBase> m_Instance = nullptr;

		DLL_API std::shared_ptr<GridManagerBase> GetInstance()
		{
			if (m_Instance == nullptr)
				m_Instance = std::make_shared<GridManagerBase>();
			return m_Instance;
		}


	};
}