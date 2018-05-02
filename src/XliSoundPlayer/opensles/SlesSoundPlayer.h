#pragma once
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <XliSoundPlayer/SoundPlayer.h>

namespace Xli
{
    namespace PlatformSpecific
    {
        class SlesAudioEngine : public Object
        {
        public:
            SLObjectItf EngineObject;
            SLEngineItf EngineEngine;
            SLObjectItf OutputMixObject;
        };
    }
}
