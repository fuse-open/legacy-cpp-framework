#include <XliSoundPlayer/SoundPlayer.h>

namespace Xli
{
    class DummySoundChannel: public SoundChannel
    {
    public:
        virtual void Pause() { }
        virtual void UnPause() { }
        virtual double GetPosition() const { return 0; }
        virtual void SetPosition(double position) { }
        virtual float GetVolume() const { return 0; }
        virtual void SetVolume(float volume) const { }
        virtual bool IsPlaying() { return false; }
        virtual bool IsFinished() { return false; }

        virtual double GetDuration() const { return 0; }
        virtual float GetPan() const  { return 0; }
        virtual void SetPan(float pan) const { }
        virtual void Play() { }
        virtual void Stop() { }
        virtual bool IsPaused()  { return false; }
    };

    /** \ingroup XliSoundPlayer */
    class DummySound: public Sound
    {
    public:
        virtual double GetDuration() const { return 0; }
        virtual String GetPath() const { return ""; }
        virtual bool IsAsset() const { return false; }
    };

    /** \ingroup XliSoundPlayer */
    class DummySoundPlayer: public SoundPlayer
    {
    public:
        virtual Sound* CreateSoundFromAsset(const String& filename) { return new DummySound(); }
        virtual SoundChannel* PlaySound(Sound* sound, bool loop) { return new DummySoundChannel(); }
    };

    SoundPlayer* SoundPlayer::Create()
    {
        return new DummySoundPlayer();
    }
}
