#include <AudioToolbox/AudioToolbox.h>
#import <AVFoundation/AVFoundation.h>
#include <XliPlatform/PlatformSpecific/iOS.h>
#include <XliSoundPlayer/SoundPlayer.h>
#include "CoreSoundDelegate.h"
#include <uBase/Array.h>
#include <uBase/Console.h>
#include <assert.h>

@implementation AudioDelegate

- (id) initWithCPPInstance:(Xli::CoreSoundChannel*)cppInstance
{
    self = [super init];
    if (self) {
        m_handlerInstance = cppInstance;
    }
    return self;
}

- (void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag
{
    [player stop];
    [player prepareToPlay];
    m_handlerInstance->_finished = true;
}

- (void)didReceiveMemoryWarning
{
    // Dont really want to clean up here as should probably be higher system task
}

@end

namespace Xli
{
    CoreSoundChannel::CoreSoundChannel(const char* filename, bool loop, bool play)
    {
        NSString* pathString = [NSString stringWithFormat:@"%@/data/%@", [[NSBundle mainBundle] resourcePath], [NSString stringWithUTF8String:filename]];

        NSURL* url = [NSURL fileURLWithPath:pathString];
        //[pathString release];

        NSError* error;

        BOOL fileExists = [[NSFileManager defaultManager] fileExistsAtPath:pathString];
        if (!fileExists) {
            const char* wrongPath = [pathString UTF8String];
            Error->WriteLine("SoundPlayer: Audio file not found at:");
            Error->WriteLine(wrongPath);
            player = nil;
            return;
        }

        player = [[AVAudioPlayer alloc] initWithContentsOfURL:url error:&error];
        //player = [[AVAudioPlayer alloc] initWithData:[NSData dataWithContentsOfURL:url] error:&error];
        //[url release];

        if (player == nil)
        {
            Error->WriteLine("ERROR: Failed to create AVAudioPlayer: " + (String)([[error description] UTF8String]));
            return;
        }

        audioDelegate = [[AudioDelegate alloc] initWithCPPInstance:this];
        player.delegate = audioDelegate;

        _finished = false;
        if (loop)
        {
            player.numberOfLoops = -1;
        }

        [player prepareToPlay];
        if (play)
        {
            [player play];
        }
    }

    CoreSoundChannel::~CoreSoundChannel()
    {
        if (player == nil) return;
        [player stop];
        [player release];
    }

    void CoreSoundChannel::Pause()
    {
        if (player == nil) return;
        [player pause];
    }
    void CoreSoundChannel::UnPause()
    {
        if (player == nil) return;
        [player play];
    }

    double CoreSoundChannel::GetPosition() const
    {
        if (player == nil) return 0.0;
        return player.currentTime;
    }
    void CoreSoundChannel::SetPosition(double value)
    {
        if (player == nil) return;
        _finished = false;
        player.currentTime = value;
    }

    float CoreSoundChannel::GetVolume() const
    {
        if (player == nil) return 0.0f;
        return player.volume;
    }
    void CoreSoundChannel::SetVolume(float value) const
    {
        if (player == nil) return;
        player.volume = value;
    }
    bool CoreSoundChannel::IsPlaying()
    {
        if (player == nil) return false;
        return player.playing;
    }
    bool CoreSoundChannel::IsFinished()
    {
        if (player == nil) return true;
        return _finished;
    }

    double CoreSoundChannel::GetDuration() const
    {
        if (player == nil || player == 0) return 0;
        return (double)player.duration;
    }
    float CoreSoundChannel::GetPan() const
    {
        if (player == nil) return 0.0f;
        return player.pan;
    }
    void CoreSoundChannel::SetPan(float value) const
    {
        if (player == nil) return;
        player.pan = value;
    }

    void CoreSoundChannel::Play()
    {
        if (player == nil) return;
        [player play];
    }
    void CoreSoundChannel::Stop()
    {
        if (player == nil) return;
        [player stop];
        player.currentTime = 0;
        _finished = false;
        [player prepareToPlay];
    }

    bool CoreSoundChannel::IsPaused()
    {
        if (player == nil) return true;
        return !player.playing;
    }




    class CoreSound : public Sound
    {
    private:
        double duration;
        bool isasset;
        String path;
    public:
        CoreSound(const String& path, bool asset)
        {
            this->path = path;
            this->isasset = asset;

            CoreSoundChannel* c = new CoreSoundChannel(path.Ptr(), false, false);
            this->duration = c->GetDuration();
            delete c;
            this->duration = 0.0;
        }
        virtual double GetDuration() const
        {
            return duration;
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

    class CoreSoundPlayer : public SoundPlayer
    {
        virtual CoreSound* CreateSoundFromAsset(const String& filename)
        {
            return new CoreSound(filename, true);
        }
        virtual SoundChannel* PlaySound(Sound* sound, bool loop)
        {
            CoreSoundChannel* result = new CoreSoundChannel(sound->GetPath().Ptr(), loop, true);
            return result;
        }
    };

    SoundPlayer* SoundPlayer::Create()
    {
        return new CoreSoundPlayer();
    }
}
