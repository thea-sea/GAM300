#ifndef SOUND
#define SOUND

#include <string>
#include <array>
#include <time.h>

#include "components/iComponent.h"
#include "Vector3.h"
#include "ecs/ecs.h"

namespace TDS
{    
    static unsigned int ID_Count{ 0 };

    /*enum SOUND_STATE : uint16_t
    {
        SOUND_LOADED = 0,
        SOUND_PLAYING,
        SOUND_STOP,
        SOUND_MUTED
    };*/

    enum SOUND_STATE
    {
        SOUND_ERR = 0,
        SOUND_LOADED,
        SOUND_PLAYING,
        SOUND_PAUSE
    };

    DLL_API struct SoundInfo : public IComponent
    {        
        unsigned int uniqueID, MSLength;
        std::string filePath;
        bool isitLoop, isit3D, isitmuted;
        SOUND_STATE whatState;
        Vec3 position;
        float volume, ReverbAmount;

        /**
         * @brief Loading info from a file into SOUNDINFO.
         * @param obj 
         * @return true when success.
        */
        virtual bool Deserialize(const rapidjson::Value& obj)
        {
            position = { obj["PositionX"].GetFloat(), obj["PositionY"].GetFloat(), obj["PositionZ"].GetFloat() };
            filePath = { obj["file"].GetString() };
            isitLoop = { obj["Loop"].GetBool() };
            isit3D = { obj["3D"].GetBool() };
            

            return true; //Change this to return based of whether it's really successful or not
        }

        /**
         * @brief Storing info from SOUNDINFO into a file
         * @param writer 
         * @return true when success.
        */
        virtual bool Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const
        {
            writer->Key("PositionX");
            writer->Double(static_cast<float>(position[0]));

            return true; //Change this to return based of whether it's really successful or not
        }

        // convenience method to set the 3D coordinates of the sound.
        void set3DCoords(float x, float y, float z)
        {
            position.x = x;
            position.y = y;
            position.z = z;
        }

        void set3DCoords(Vec3 set_this)
        {
            position = set_this;
        }

        void setFilePath(std::string _path)
        {
            filePath = _path;
        }

        bool isLoaded()
        {
            return (whatState == SOUND_LOADED);
        }

        bool is3D()
        {
            return isit3D;
        }

        bool isLoop()
        {
            return isitLoop;
        }
        
        bool isMuted()
        {
            return isitmuted;
        }

        bool isPlaying()
        {
            return (whatState == SOUND_PLAYING);
        }

        bool isPaused()
        {
            return (whatState == SOUND_PAUSE);
        }

        /*bool isPlaying()
        {
            return (bool)(whatState & (1 << SOUND_PLAYING));
        }

        bool isPaused()
        {
            return (bool)(whatState ^ (1 << SOUND_PLAYING));
        }

        bool isMuted()
        {
            return (bool)(whatState & (1 << SOUND_MUTED));
        }*/

        Vec3 get3DCoords()
        {
            return position;
        }

        SOUND_STATE getState()
        {
            return whatState;
        }

        unsigned int getUniqueID()
        {
            return uniqueID;
        }

        unsigned int getMSLength()
        {
            return MSLength;
        }

        std::string getFilePath()
        {
            return filePath;
        }

        const char* getFilePath_inChar()
        {
            const char* name = filePath.c_str();

            return name;
        }

        float getX()
        {
            return position[0]; //!!!!!!!To be replaced when vec container is used
        }

        float getY()
        {
            return position[1]; //!!!!!!!To be replaced when vec container is used
        }

        float getZ()
        {
            return position[2]; //!!!!!!!To be replaced when vec container is used
        }

        float getReverbAmount()
        {
            return ReverbAmount;
        }

        float getVolume()
        {
            return volume;
        }

        /**
        * Parameter takes in Volume values (0 - 100)
        */
        void setVolume(float vol)
        {
            volume = 20.0f * log10f(vol);

            if (volume > 1.f)
            {
                volume = 1.f;
            }
        }

        void setMSLength(unsigned int len)
        {
            MSLength = len;
        }

        /*void setState(SOUND_STATE setting, bool set)
        {
            whatState |= (set << setting);
        }*/

        void setState(SOUND_STATE setting)
        {
            whatState = setting;
        }

        void setLoop(bool condition)
        {
            isitLoop = condition;
        }

        void set3D(bool condition)
        {
            isit3D = condition;
        }

        void setMute(bool condition)
        {
            isitmuted = condition;
        }

        void setReverbAmount(float reverb)
        {
            ReverbAmount = reverb;
        }

        SoundInfo(std::string _filePath = "", bool _isLoop = false, bool _is3D = false, bool _muted = false, SOUND_STATE _theState = SOUND_ERR, float _x = 0.0f, float _y = 0.0f, float _z = 0.0f, float _volume = 1.f, float _reverbamount = 0.f)
            : filePath(_filePath), isitLoop(_isLoop), isit3D(_is3D), isitmuted(_muted), whatState(_theState), volume(_volume), ReverbAmount(_reverbamount)
        {            
            position.x = _x;
            position.y = _y;
            position.z = _z;
            MSLength = 0;
            uniqueID = ID_Count++; //Change UID to include time when added
        }

        // TODO  implement sound instancing
        // int instanceID = -1; 
        void operator=(const SoundInfo& rhs)
        {
            uniqueID = rhs.uniqueID;
            MSLength = rhs.MSLength;
            filePath = rhs.filePath;
            isitLoop = rhs.isitLoop;
            isit3D = rhs.isit3D;
            isitmuted = rhs.isitmuted;
            whatState = rhs.whatState;
            position = rhs.position;
            volume = rhs.volume;
            ReverbAmount = rhs.ReverbAmount;
        }
        RTTR_ENABLE(IComponent);
        RTTR_REGISTRATION_FRIEND

    }; //end of SoundInfo struct

    DLL_API  SoundInfo* GetSoundInfo(EntityID ID);

} //end of namespace TDS

#endif