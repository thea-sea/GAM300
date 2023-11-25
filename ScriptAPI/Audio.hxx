//#pragma once
//#include "IncludeFromEngine.hxx"
//#include <fmod_engine/AudioEngine.h>
//#include <components/SoundInfo.h>
//#include <filesystem>
//
//namespace ScriptAPI
//{
//    public ref class AudioClip
//    {
//    public:        
//        void add_clips(std::filesystem::path file);
//        
//        System::Collections::Generic::List<System::String^> clips;
//        int sub;
//    };
//    
//    public ref class AudioSource
//    {
//    public:
//        static ref struct volume
//        {
//            float value;
//        };
//        
//        static ref struct pitch
//        {
//            float value;
//        };
//
//    public:
//        AudioSource();
//        
//        void Play(unsigned long delay);
//        void Pause();
//        void Stop();
//
//        /*float get_volume();
//        void set_volume(float vol);*/
//        void Loop(bool set);
//        bool enabled();
//        /*float get_pitch();
//        void set_pitch(float set);*/
//
//        template<typename T>
//        T& operator=(float val);
//
//        bool isPlaying();
//
//        AudioClip^ clip; //AudioClips are attached to AudioSource
//                     //returns a clip,
//        TDS::AudioWerks::AudioEngine* audio_engine;
//        unsigned long wait;
//        float deltatime;
//    };
//}