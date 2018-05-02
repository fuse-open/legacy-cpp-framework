#pragma once
#include <jni.h>
#include "AInternal.h"

namespace Xli
{
    namespace PlatformSpecific
    {
        class AJniHelper
        {
            JNIEnv* env;
            static JavaVM* vm;
            static jclass activityClass;
            static jobject activity;
            static jobject jAssetManager;
            static AAssetManager* assetManager;

            static void SetActivityClass(JNIEnv* env, jclass globalRefdClass);
        public:

            static void Init(JavaVM* jvm, JNIEnv* env, jclass globalRefdClass, jobject optionalActivity);
            AJniHelper();

            static JavaVM* GetVM();
            JNIEnv* GetEnv();
            JNIEnv* operator->();
            jobject GetActivity();
            static jclass GetActivityClass();

            AAssetManager* GetAssetManager();

        };
    }
}