#include "imguiHelper/ImguiAudio.h"

namespace TDS
{
	AudioImgui::AudioImgui()
	{
		flags = ImGuiWindowFlags_NoCollapse;
		panelTitle = "Audio Files";
		windowPadding = ImVec2(0.f, 0.f);

		audeng.init();
		
	}

	AudioImgui::~AudioImgui()
	{
		
		audeng.deactivate();
	}

	void AudioImgui::update()
	{
		
	}
}