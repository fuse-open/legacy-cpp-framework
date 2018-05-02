#pragma once
#include <XliSoundPlayer/SoundPlayer.h>
#include <AudioToolbox/AudioToolbox.h>
#import <AVFoundation/AVFoundation.h>

namespace Xli
{
    class CoreSoundChannel;
}

@interface AudioDelegate : NSObject <AVAudioPlayerDelegate> {
    Xli::CoreSoundChannel* m_handlerInstance;
}

- (id) initWithCPPInstance:(Xli::CoreSoundChannel*)cppInstance;
- (void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag;
- (void)didReceiveMemoryWarning;

@end

namespace Xli
{
    class CoreSoundChannel : public SoundChannel
    {
    public:
        AVAudioPlayer* player;
        AudioDelegate* audioDelegate;
        bool _finished;

        CoreSoundChannel(const char* filename, bool loop, bool play);

        virtual ~CoreSoundChannel();

        virtual void Pause();
        virtual void UnPause();

        virtual double GetPosition() const;
        virtual void SetPosition(double value);

        virtual float GetVolume() const;
        virtual void SetVolume(float value) const;
        virtual bool IsPlaying();
        virtual bool IsFinished();

        double GetDuration() const;
        virtual float GetPan() const;
        virtual void SetPan(float value) const;

        virtual void Play();
        virtual void Stop();

        virtual bool IsPaused();
    };
}
