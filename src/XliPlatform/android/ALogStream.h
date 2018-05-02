#pragma once
#include <cstdlib>
#include <errno.h>
#include <unistd.h>
#include <XliPlatform/PlatformSpecific/Android.h>
#include <uBase/Console.h>

namespace Xli
{
    namespace PlatformSpecific
    {
        using namespace uBase;

        class ALogStream: public Stream
        {
            int prio;
            Array<char> buf;
        public:
            ALogStream(int prio);
            virtual ~ALogStream();
            virtual bool CanWrite() const { return true; }
            virtual void Write(const void* src, int elmSize, int elmCount);
        };
    }
}
