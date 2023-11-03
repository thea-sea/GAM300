#ifndef IMGUI_ASSET_BROWSER
#define IMGUI_ASSET_BROWSER

#include "ImguiHelper.h"

namespace TDS 
{
	class AssetBrowser : public LevelEditorPanel
	{
	public:
		AssetBrowser();
		void getFileNameFromPath(const char* full_path, std::string* path, std::string* file, std::string* fileWithExtension, std::string* extension) const;
		void init() {}
		void update();
		static void destroyIcons();
		static inline std::filesystem::path m_curr_path;
	private:
		float thumbnail_size = 67.791f;
		float padding = 10.f;
		std::string path1;
		std::string selectedpath;
		bool loadonce = true;

	};
}

#endif // IMGUI_ASSET_BROWSER
