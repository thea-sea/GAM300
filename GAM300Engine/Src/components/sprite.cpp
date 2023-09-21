#include "components/sprite.h"

namespace TDS
{
	Sprite::Sprite() : mIndex			(Vec2(1.0f, 1.0f)),
					   mIsSpriteSheet	(false),
					   mIsAnimated		(false),
					   mInternalTimer	(0.0f),
					   mLayerOrder		(0),
					   mTextureName		("none"),
					   mUVcooridnates	(Vec2(0.0f, 0.0f)),
					   mAlpha			(1.0f),
					   mMaxIndex		(1)
	{ }

	bool Sprite::Deserialize(const rapidjson::Value& obj)
	{
		mIndex = Vec2(obj["indexX"].GetFloat(), obj["indexY"].GetFloat());
		mIsSpriteSheet = obj["isSpriteSheet"].GetBool();
		mIsAnimated = obj["isAnimated"].GetBool();
		mLayerOrder = obj["layerOrder"].GetInt();
		mTextureName = obj["textureName"].GetString();
		mUVcooridnates = Vec2(obj["Ucoordinates"].GetFloat(), obj["Vcoordinates"].GetFloat());
		mAlpha = obj["alpha"].GetFloat();
		mMaxIndex = obj["maxIndex"].GetInt();

		for (auto i = 0; i < mMaxIndex; ++i)
		{
			std::string displayNumber;
			displayNumber = "DisplayTime " + std::to_string(i);
			float individualtime = float(obj[displayNumber.c_str()].GetFloat());
			mDisplayTime.emplace_back(individualtime);
		}

		return true;
	}

	bool Sprite::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const
	{
		writer->Key("indexX");
		writer->Double(mIndex.x);
		writer->Key("indexY");
		writer->Double(mIndex.y);

		writer->Key("isSpriteSheet");
		writer->Bool(mIsSpriteSheet);

		writer->Key("isAnimated");
		writer->Uint(mIsAnimated);

		writer->Key("layerOrder");
		writer->Int(mLayerOrder);

		writer->Key("textureName");
		writer->String(mTextureName.c_str());

		writer->Key("Ucoordinates");
		writer->Double(mUVcooridnates.x);
		writer->Key("Vcoordinates");
		writer->Double(mUVcooridnates.y);

		writer->Key("alpha");
		writer->Double(mAlpha);

		writer->Key("maxIndex");
		writer->Int(mMaxIndex);

		for (auto i = 0; i < mDisplayTime.size(); i++)
		{
			std::string displayNumber;
			displayNumber = "DisplayTime " + std::to_string(i);
			writer->Key(displayNumber.c_str());
			writer->Double(mDisplayTime[i]);
		}

		return true;
	}

	void Sprite::ImGuiDisplay()
	{
		ImguiVec2Input("Index", mIndex);
		ImguiBoolInput("Is Spritesheet", mIsSpriteSheet);
		ImguiBoolInput("Is Animated", mIsAnimated);
		ImguiFloatInput("Internal Timer", mInternalTimer);
		ImguiIntInput("Layer Order", mLayerOrder);
		ImguiTextInput("Texture Name", mTextureName);
		ImguiVec2Input("UV Coordinate", mUVcooridnates);
		ImguiFloatInput("Alpha", mAlpha);
		ImguiIntInput("Max Index", mMaxIndex);
	}
}