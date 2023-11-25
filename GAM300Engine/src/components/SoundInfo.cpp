#include "components/SoundInfo.h"

RTTR_REGISTRATION
{
    using namespace TDS;

    rttr::registration::class_<SoundInfo>("Audio")
    .property("File path", &SoundInfo::filePath)
    .property("Loop", &SoundInfo::isitLoop)
    .property("3D", &SoundInfo::isit3D)
    .property("Muted", &SoundInfo::isitmuted)
    .property("Position", &SoundInfo::position)
    .property("volume", &SoundInfo::volume)
    .property("Reverb", &SoundInfo::ReverbAmount);
}

namespace TDS
{
    SoundInfo* GetSoundInfo(EntityID ID)
    {
        return ecs.getComponent<SoundInfo>(ID);
    }
}