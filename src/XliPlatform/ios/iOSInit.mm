#include <uBase/Console.h>
#include <XliPlatform/PlatformSpecific/iOS.h>
#import <Foundation/NSObjCRuntime.h>
#import <Foundation/NSString.h>

namespace Xli
{
    namespace PlatformSpecific
    {
        using namespace uBase;

        class NSLogStream: public Stream
        {
            Array<char> buf;

        public:
            virtual bool CanWrite() const
            {
                return true;
            }

            virtual void Write(const void* src, int elmSize, int elmCount)
            {
                for (int i = 0; i < elmCount; i++)
                {
                    char c = ((char*)src)[i];

                    if (c == '\n')
                    {
                        buf.Add('\0');
                        NSLog(@"%@", [NSString stringWithUTF8String:buf.Ptr()]);
                        buf.Clear();
                        continue;
                    }
                    else if (c == '\t')
                    {
                        buf.AddRange("    ", 4 - buf.Length() % 4);
                        continue;
                    }
                    else if (c == '\r')
                    {
                        continue;
                    }

                    buf.Add(c);
                }
            }

            virtual void Flush()
            {
                if (buf.Length() > 0)
                {
                    buf.Add('\0');
                    NSLog(@"%@", [NSString stringWithUTF8String:buf.Ptr()]);
                    buf.Clear();
                }
            }
        };

        void iOS::Init()
        {
#ifndef TARGET_IPHONE_SIMULATOR
            Auto<Stream> log = new NSLogStream();
            Out->SetStream(log);
            Error->SetStream(log);
#endif
        }
    }
}
