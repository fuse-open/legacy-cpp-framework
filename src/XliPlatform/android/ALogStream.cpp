#include "ALogStream.h"
#include "AInternal.h"

namespace Xli
{
    namespace PlatformSpecific
    {
        ALogStream::ALogStream(int prio)
        {
            this->prio = prio;
        }

        ALogStream::~ALogStream()
        {
            if (buf.Length())
            {
                buf.Add(0);
                __android_log_write(prio, AGetAppName(), buf.Ptr());
                buf.Clear();
            }
        }

        void ALogStream::Write(const void* src, int elmSize, int elmCount)
        {
            for (int i = 0; i < elmSize * elmCount; i++)
            {
                char c = ((const char*)src)[i];

                switch (c)
                {
                case '\n':
                    buf.Add(0);
                    __android_log_write(prio, AGetAppName(), buf.Ptr());
                    buf.Clear();
                    continue;

                case 0:
                    buf.Add((char)(unsigned char)0xC0);
                    buf.Add((char)(unsigned char)0x80);
                    continue;
                }

                buf.Add(c);
            }
        }
    }
}
