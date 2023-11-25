#include "components/AudioSource.h"

namespace TDS
{
	AudioSource::AudioSource()
	{
		position.Set(0.f, 0.f, 0.f);
	}

	AudioSource::~AudioSource()
	{
		//Empty
	}

	bool AudioSource::Deserialize(const rapidjson::Value& obj)
	{		
		position = { obj["PositionX"].GetFloat(), obj["PositionY"].GetFloat(), obj["PositionZ"].GetFloat() };

		return true; //Change this to return based of whether it's really successful or not
	}

	bool AudioSource::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const
	{
		writer->Key("PositionX");
		writer->Double(static_cast<float>(position[0]));

		return true; //Change this to return based of whether it's really successful or not
	}

	void AudioSource::Play()
	{

	}

	void AudioSource::Stop()
	{

	}

	void AudioSource::Add(unsigned int ID, FMOD::Sound& clip)
	{
		audioclips[ID] = &clip;
	}

	void AudioSource::remove(unsigned int ID)
	{
		audioclips.erase(ID);
	}

	void AudioSource::Update()
	{

	}
}