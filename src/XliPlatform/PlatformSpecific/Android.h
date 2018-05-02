#pragma once
#include <jni.h>

struct android_app;

namespace Xli
{
    namespace PlatformSpecific
    {
        /** \ingroup XliPlatform */
        class Android
        {
        public:
            static void PreInit(JavaVM* jvm, JNIEnv* env, jclass globalRefdShim);
            static void PostInit();
            static void SetLogTag(const char* tag);
        };
    }
}
