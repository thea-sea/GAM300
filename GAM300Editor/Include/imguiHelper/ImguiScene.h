#ifndef IMGUI_SCENE
#define IMGUI_SCENE

#include "ImguiHelper.h"
#include "imguiHelper/ImguiHierarchy.h"
namespace TDS 
{
	class EditorScene : public LevelEditorPanel
	{
	public:
		EditorScene();

		void init();
		void update();
		void Resize();

	private:
		
		int m_gizmoType = 7;
		bool m_gizmoActive = false;
	};
}

#endif // IMGUI_SCENE
