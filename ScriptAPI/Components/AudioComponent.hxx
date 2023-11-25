#pragma once

#include "ComponentBase.hxx"
#include "TransformComponent.hxx"

namespace ScriptAPI
{
	using snd = TDS::SOUND_STATE;
	
	public value class AudioComponent : ComponentBase
	{
	public:
		void set3DCoords(float x, float y, float z);

		bool isLoaded();
		bool is3D();
		bool isLoop();
		bool isMuted();
		bool isPlaying();
		bool isPaused();

		Vector3 get3DCoords();
		snd getState();
		unsigned int getUniqueID();
		unsigned int getMSLength();
		std::string getFilePath();
		const char* getFilePath_inChar();

		float getX();
		float getY();
		float getZ();
		float getReverbAmount();
		float getVolume();

		void setVolume(float vol);
		void setMSLength(unsigned int len);
		void setState(snd setting);
		void setLoop(bool condition);
		void set3D(bool condition);
		void setMute(bool condition);

		virtual void SetEntityID(TDS::EntityID ID);

		TransformComponent transform;

	internal:
		AudioComponent(TDS::EntityID ID);
		TDS::EntityID GetEntityID();

	private:
		property unsigned int uniqueID
		{
			unsigned int get();
		}
		property unsigned int MSLength
		{
			unsigned int get();
			void set(unsigned int value);
		}
		property std::string filePath
		{
			std::string get();
		}
		property bool isitLoop
		{
			bool get();
			void set(bool value);
		}
		property bool isit3D
		{
			bool get();
			void set(bool value);
		}
		property bool isitMuted
		{
			bool get();
			void set(bool value);
		}
		property snd whatState
		{
			snd get();
			void set(snd value);
		}
		property Vector3 pos
		{
			Vector3 get();
			void set(Vector3 value);
		}
		property float volume
		{
			float get();
			void set(float value);
		}
		property float ReverbAmount
		{
			float get();
			void set(float value);
		}

		TDS::EntityID entityID;
	};
}