/*!*************************************************************************
****
\file ImguiAudio.h
\author w.chongsheng
\par DP email: w.chongsheng@digipen.edu
\par Course: CSD3400
\par Section: a
\brief  Imgui UI in charge of audio manipulations
****************************************************************************
***/

#ifndef IMGUI_AUDIO
#define IMGUI_AUDIO

#include <filesystem>

#include "ImguiHelper.h"

namespace TDS
{
	class AudioImgui : public LevelEditorPanel
	{
	public:
		/**
		 * @brief Constructor that sets appear to false and bar as 0.f. Initiates the Audio Engine.
		*/
		AudioImgui();

		/**
		 * @brief Destructor that deactivates the Audio Engine
		*/
		~AudioImgui();

		/**
		 * @brief Show or hide audio controls. Also can be used to see if active or not
		 * @param state 
		 * @return 
		*/
		bool ToggleControls(bool state = false);

		/**
		 * @brief Add sound into vector container
		 * @param folder_path 
		*/
		void add_audio_files(std::filesystem::path folder_path);

		/**
		 * @brief Step into multiple levels of folders
		*/
		std::vector<std::filesystem::path> go_deeper(std::filesystem::path f_path);

		void play(std::string file); //temp testing

		/**
		 * @brief Initialize
		*/
		virtual void init() {}

		/**
		 * @brief Update loop.
		*/
		void update();

	private:
		AudioWerks::AudioEngine* audeng;

		bool appear, playing;
		float bar;

		std::vector<SoundInfo> music;
		std::vector<SoundInfo> SFX;
		std::vector<SoundInfo> background;
		std::vector<SoundInfo> VO;
	};
}

#endif