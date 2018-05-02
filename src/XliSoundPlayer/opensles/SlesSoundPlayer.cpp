#include <sys/types.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <XliSoundPlayer/SoundPlayer.h>
#include <XliPlatform/PlatformSpecific/Android.h>
#include "../../XliPlatform/android/AJniHelper.h"
#include "SlesSoundPlayer.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <assert.h>
#include <math.h>

namespace Xli
{
    static Xli::PlatformSpecific::SlesAudioEngine* GlobalAAudioEngine = 0;

    class AudioEngine
    {
    public:
        static void Startup()
        {
            SLresult result;
            Xli::PlatformSpecific::SlesAudioEngine* newAudioEngine = new Xli::PlatformSpecific::SlesAudioEngine();

            SLEngineOption options[] = { SL_ENGINEOPTION_THREADSAFE, SL_BOOLEAN_TRUE };
            result = slCreateEngine(&newAudioEngine->EngineObject, 1, options, 0, NULL, NULL);
            assert(SL_RESULT_SUCCESS == result);
            (void)result;

            // realize the engine
            result = (*newAudioEngine->EngineObject)->Realize(newAudioEngine->EngineObject,SL_BOOLEAN_FALSE);
            assert(SL_RESULT_SUCCESS == result);
            (void)result;

            // get the engine interface, which is needed in order to create other objects
            result = (*newAudioEngine->EngineObject)->GetInterface(newAudioEngine->EngineObject, SL_IID_ENGINE, &newAudioEngine->EngineEngine);
            assert(SL_RESULT_SUCCESS == result);
            (void)result;

            // create output mix
            result = (*newAudioEngine->EngineEngine)->CreateOutputMix(newAudioEngine->EngineEngine, &newAudioEngine->OutputMixObject, 0, NULL, NULL);
            assert(SL_RESULT_SUCCESS == result);
            (void)result;

            // realize the output mix
            // [TODO] What does that FALSE do?
            result = (*newAudioEngine->OutputMixObject)->Realize(newAudioEngine->OutputMixObject, SL_BOOLEAN_FALSE);
            assert(SL_RESULT_SUCCESS == result);
            (void)result;
            if (GlobalAAudioEngine != 0) GlobalAAudioEngine->Release();
            GlobalAAudioEngine = newAudioEngine;

            LOGD("Engine Created");
        }

        static void Shutdown()
        {
            // destroy engine object and interfaces
            if (GlobalAAudioEngine->EngineObject != NULL) {
                (*GlobalAAudioEngine->EngineObject)->Destroy(GlobalAAudioEngine->EngineObject);
                GlobalAAudioEngine->EngineObject = NULL;
                GlobalAAudioEngine->EngineEngine = NULL;
            }
        }
    };

    static int SoundRefCount = 0;
    static void AssertInit()
    {
        if (!SoundRefCount)
        {
            AudioEngine::Startup();
            atexit(AudioEngine::Shutdown);
            SoundRefCount+=1;
        }
    }

    class SlesSoundChannel : public SoundChannel
    {
    private:
        SLObjectItf playerObject;
        SLPlayItf playerPlayItf;
        SLSeekItf playerSeekItf;
        SLVolumeItf playerVolumeItf;
        SLBufferQueueItf bufferQueueItf;
        bool userPaused;
        bool atStart;
        bool Closed;
        mutable double cachedDuration;

    public:
        SlesSoundChannel(const Sound* const sound)
        {
            playerObject = NULL;
            playerPlayItf = NULL;
            playerSeekItf = NULL;
            playerVolumeItf = NULL;
            bufferQueueItf = NULL;
            userPaused = false;
            atStart = true;

            cachedDuration = -1.0;
            Closed = !PrepareSlesPlayer(sound->GetPath(), sound->IsAsset());
        }

        virtual ~SlesSoundChannel()
        {
            playerPlayItf = NULL;
            playerSeekItf = NULL;
            playerVolumeItf = NULL;
            bufferQueueItf = NULL;
            (*playerObject)->Destroy(playerObject);
            Closed=true;
        }

        virtual void Pause()
        {
            if (!IsFinished())
            {
                userPaused = true;
                (*playerPlayItf)->SetPlayState(playerPlayItf, SL_PLAYSTATE_PAUSED);
            }
        }
        virtual void UnPause()
        {
            if (this->IsPaused() && userPaused)
            {
                userPaused = false;
                atStart = false;
                (*playerPlayItf)->SetPlayState(playerPlayItf, SL_PLAYSTATE_PLAYING);
            }
        }

        virtual double GetPosition() const
        {
            SLmillisecond pos = -1;
            SLresult result = (*playerPlayItf)->GetPosition(playerPlayItf, &pos);
            assert(SL_RESULT_SUCCESS == result);
            return ((double)((int)pos))/1000.0;
        }
        virtual void SetPosition(double position)
        {
            atStart = (position == 0);
            int pos = (int)(position * 1000);
            SLresult result = (*playerSeekItf)->SetPosition(playerSeekItf, pos, SL_SEEKMODE_FAST);
            assert(SL_RESULT_SUCCESS == result);
        }

        float GainToAttenuation(float gain) const
        {
            //http://vec3.ca/getting-started-with-opensl-on-android/
            return gain < 0.01f ? -96.0f : 20 * log10( gain );
        }
        float AttenuationToGain(float att) const
        {
            return att <= -96.0f ? 0.0f : pow(10, (att / 20.0f));
        }
        virtual float GetVolume() const
        {
            SLmillibel vol;
            SLresult result = (*playerVolumeItf)->GetVolumeLevel(playerVolumeItf, &vol);
            assert(SL_RESULT_SUCCESS == result);
            return AttenuationToGain(vol / 100.0f);
        }
        virtual void SetVolume(float volume) const
        {
            float att = (GainToAttenuation(volume) * 100.0f);
            SLresult result = (*playerVolumeItf)->SetVolumeLevel(playerVolumeItf, (SLmillibel)att);
            assert(SL_RESULT_SUCCESS == result);
        }

        virtual bool IsPlaying()
        {
            SLuint32 pState;
            SLresult result = (*playerPlayItf)->GetPlayState(playerPlayItf, &pState);
            return (pState == SL_PLAYSTATE_PLAYING);
        }

        virtual bool IsFinished()
        {
            SLuint32 pState;
            SLresult result = (*playerPlayItf)->GetPlayState(playerPlayItf, &pState);
            return (!atStart && (pState == SL_PLAYSTATE_STOPPED) || ((pState == SL_PLAYSTATE_PAUSED) && !userPaused));
        }

        virtual double GetDuration() const
        {
            if (cachedDuration<0)
            {
                SLmillisecond duration = -1;
                SLresult result = (*playerPlayItf)->GetDuration(playerPlayItf, &duration);
                assert(SL_RESULT_SUCCESS == result);
                if (cachedDuration >= 0)
                    cachedDuration = ((double)((int)duration)/1000.0);
            }
            return cachedDuration;
        }

        virtual void Play()
        {
            atStart = false;
            (*playerPlayItf)->SetPlayState(playerPlayItf, SL_PLAYSTATE_PLAYING);
        }
        virtual void Stop()
        {
            SLresult result=(*playerPlayItf)->SetPlayState(playerPlayItf, SL_PLAYSTATE_STOPPED);
            assert(SL_RESULT_SUCCESS == result);
        }

        virtual bool GetLoop()
        {
            SLboolean looping;
            SLmillisecond milli_start;
            SLmillisecond milli_end;
            SLresult result = (*playerSeekItf)->GetLoop(playerSeekItf, &looping,
                                                        &milli_start, &milli_end);
            assert(SL_RESULT_SUCCESS == result);
            return (bool)looping;
        }
        virtual void SetLoop(double start, double end)
        {
            if (start >= 0 && end < 0) {
                end = SL_TIME_UNKNOWN;
            } else {
                end = start * 1000.0;
            }
            SLresult result = (*playerSeekItf)->SetLoop(playerSeekItf, SL_BOOLEAN_TRUE,
                                                        (SLmillisecond)start * 1000.0,
                                                        (SLmillisecond)end);
            assert(SL_RESULT_SUCCESS == result);
        }

        virtual float GetPan() const
        {
            SLpermille pos;
            SLresult result = (*playerVolumeItf)->GetStereoPosition(playerVolumeItf, &pos);
            assert(SL_RESULT_SUCCESS == result);
            float pan = pos / 1000.0f;
            if (pan < -1.0f) pan = -1.0f;
            if (pan > 1.0f) pan = 1.0f;
            return pan;
        }
        virtual void SetPan(float pan) const
            {
                if (pan < -1.0f) pan = -1.0f;
                if (pan > 1.0f) pan = 1.0f;
                SLpermille pos = pan * 1000.0f;
                SLresult result = (*playerVolumeItf)->SetStereoPosition(playerVolumeItf, pos);
                assert(SL_RESULT_SUCCESS == result);
            }

        virtual bool IsPaused()
        {
            if (!userPaused) return false;
            SLuint32 pState;
            SLresult result = (*playerPlayItf)->GetPlayState(playerPlayItf, &pState);
            assert(SL_RESULT_SUCCESS == result);
            return (pState == SL_PLAYSTATE_PAUSED);
        }

        virtual bool IsClosed() const
        {
            return Closed;
        }

        virtual bool AtEnd() const
        {
            int dur = GetDuration();
            int pos = GetPosition();
            return (dur == pos);
        }

        virtual bool PrepareSlesPlayer(const String& path, bool asset)
        {
            SLresult result;
            const SLInterfaceID ids[2] = {SL_IID_SEEK, SL_IID_VOLUME};
            const SLboolean req[2] = {SL_BOOLEAN_FALSE, SL_BOOLEAN_TRUE};
            // configure audio source
            if (asset) {
                Xli::PlatformSpecific::AJniHelper jni;
                AAssetManager* mgr = jni.GetAssetManager();
                assert(NULL != mgr);
                AAsset* assetp = AAssetManager_open(mgr, path.Ptr(), AASSET_MODE_UNKNOWN);
                if (NULL == assetp) {
                    throw Exception("****Cant find asset****");
                    return false;
                }
                off_t start, length;
                int fd = AAsset_openFileDescriptor(assetp, &start, &length);
                assert(0 <= fd);
                AAsset_close(assetp);
                SLDataLocator_AndroidFD locatorIn = {SL_DATALOCATOR_ANDROIDFD, fd, start, length};
                SLDataFormat_MIME dataFormat = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
                SLDataSource audioSrc = {&locatorIn, &dataFormat};
                SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, GlobalAAudioEngine->OutputMixObject};
                SLDataSink audioSnk = {&loc_outmix, NULL};

                result = (*GlobalAAudioEngine->EngineEngine)->CreateAudioPlayer(GlobalAAudioEngine->EngineEngine, &playerObject, &audioSrc, &audioSnk, 2, ids, req);
                if (result != SL_RESULT_SUCCESS) throw Exception("Failed to create audio player");
            } else {

                SLDataLocator_URI locatorIn = {SL_DATALOCATOR_URI, (SLchar *)path.Ptr()};
                SLDataFormat_MIME dataFormat = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
                SLDataSource audioSrc = {&locatorIn, &dataFormat};
                SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, GlobalAAudioEngine->OutputMixObject};
                SLDataSink audioSnk = {&loc_outmix, NULL};

                result = (*GlobalAAudioEngine->EngineEngine)->CreateAudioPlayer(GlobalAAudioEngine->EngineEngine, &playerObject, &audioSrc, &audioSnk, 2, ids, req);
                if (result != SL_RESULT_SUCCESS) throw Exception("Failed to create audio player");
            }

            (void)result;
            // realize the player
            result = (*playerObject)->Realize(playerObject, SL_BOOLEAN_FALSE);
            assert(SL_RESULT_SUCCESS == result);
            (void)result;
            // get prefetch interface
            //{TODO} we need to add prefetch callbacks
            //       http://www.srombauts.fr/android-ndk-r5b/docs/opensles/
            //       read the prefetch section
            // get the play interface
            result = (*playerObject)->GetInterface(playerObject, SL_IID_PLAY, &playerPlayItf);
            assert(SL_RESULT_SUCCESS == result);
            (void)result;

            // get the seek interface
            result = (*playerObject)->GetInterface(playerObject, SL_IID_SEEK,
                                                   &playerSeekItf);
            assert(SL_RESULT_SUCCESS == result);
            (void)result;
            // get the volume interface
            result = (*playerObject)->GetInterface(playerObject, SL_IID_VOLUME,
                                                   &playerVolumeItf);
            (*playerVolumeItf)->EnableStereoPosition(playerVolumeItf, SL_BOOLEAN_TRUE);
            assert(SL_RESULT_SUCCESS == result);
            (void)result;
            (*playerPlayItf)->SetPlayState(playerPlayItf, SL_PLAYSTATE_PAUSED);
            return true;
        }
    };

    class SlesSound : public Sound
    {
    private:
        double duration;
        String path;
        bool isasset;
    public:
        SlesSound(const String& path, bool asset)
        {
            duration = -1;
            this->path = path;
            this->isasset = asset;
            PopulateMetadata();
        }
        virtual double GetDuration() const
        {
            return duration;
        }
        virtual void PopulateMetadata()
        {
            SlesSoundChannel* channel = new SlesSoundChannel(this);
            duration = channel->GetDuration();
            delete(channel);
        }
        virtual String GetPath() const
        {
            return path;
        }
        virtual bool IsAsset() const
        {
            return isasset;
        }
    };

    class SlesSoundPlayer : public SoundPlayer
    {
        virtual SlesSound* CreateSoundFromAsset(const String& filename)
        {
            return new SlesSound(filename, true);
        }
        virtual SoundChannel* PlaySound(Sound* sound, bool loop)
        {
            SlesSoundChannel* result = new SlesSoundChannel(sound);
            double duration = sound->GetDuration();
            if (loop) result->SetLoop(0, -1);
            result->Play();
            return (SoundChannel*)result;
        }
    };

    SoundPlayer* SoundPlayer::Create()
    {
        AssertInit();
        return new SlesSoundPlayer();
    }
}
