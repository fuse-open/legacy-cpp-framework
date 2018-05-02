#include <XliPlatform/PlatformSpecific/Android.h>
#include <XliPlatform/Window.h>
#include <XliHttpClient/HttpClient.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <pthread.h>
#include <stdarg.h>
#include "AJniHelper.h"

#define DEBUG_JNI

extern Xli::WindowEventHandler* GlobalEventHandler;
extern Xli::Window* GlobalWindow;

namespace Xli
{
    namespace PlatformSpecific
    {
        static pthread_key_t JniThreadKey;
        static void JniDestroyThread(void* value)
        {
            LOGD("JNI: Detaching current thread");

            JNIEnv* env = (JNIEnv*)value;
            AJniHelper::GetVM()->DetachCurrentThread();
            pthread_setspecific(JniThreadKey, NULL);
        }

        JavaVM* AJniHelper::vm;
        jclass AJniHelper::activityClass;
        jobject AJniHelper::activity;
        jobject AJniHelper::jAssetManager;
        AAssetManager* AJniHelper::assetManager;
        void AJniHelper::Init(JavaVM* jvm, JNIEnv* env, jclass globalRefdClass, jobject optionalActivity)
        {
            vm = jvm;
            if (pthread_key_create(&JniThreadKey, JniDestroyThread))
                LOGE("JNI ERROR: Unable to create pthread key"); // Not fatal
            jAssetManager = 0;
            assetManager = 0;
            activity = 0;
            SetActivityClass(env, globalRefdClass);
        }
        AJniHelper::AJniHelper()
        {
            if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK)
            {
                if (vm->AttachCurrentThread(&env, NULL) != JNI_OK)
                    throw Exception("JNI ERROR: Failed to attach current thread");
                pthread_setspecific(JniThreadKey, (void*)env);
            }
        }

        JNIEnv* AJniHelper::GetEnv()
        {
            // TODO: Check jni exceptions
            return env;
        }
        JNIEnv* AJniHelper::operator->()
        {
            // TODO: Check jni exceptions
            return env;
        }

        void AJniHelper::SetActivityClass(JNIEnv* env, jclass globalRefdClass)
        {
            activityClass = globalRefdClass;
        }
        jclass AJniHelper::GetActivityClass()
        {
            return activityClass;
        }
        JavaVM* AJniHelper::GetVM()
        {
            return vm;
        }
        jobject AJniHelper::GetActivity()
        {
            //activity
            if (!activity)
            {
                jmethodID getActivity = env->GetStaticMethodID(activityClass, "GetRootActivity", "()Landroid/app/Activity;");
                activity = reinterpret_cast<jclass>(env->NewGlobalRef(env->CallStaticObjectMethod(activityClass, getActivity)));
                if (activity==0) { throw Exception("JNI ERROR: Failed to grab activity object"); }
            }
            return activity;
        }
        AAssetManager* AJniHelper::GetAssetManager()
        {
            if (!assetManager)
            {
                // assetManager
                jmethodID getAssetManager = env->GetStaticMethodID(activityClass, "GetAssetManager", "()Landroid/content/res/AssetManager;");
                jobject jAssetManager = env->NewGlobalRef(env->CallStaticObjectMethod(activityClass, getAssetManager));
                assetManager = AAssetManager_fromJava(env, jAssetManager);
                if (assetManager==0) { throw Exception("JNI ERROR: Failed to grab assetManager object"); }
            }
            return assetManager;
        }
    }
}
