#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "../unix/UnixFileSystem.h"
#include "../../XliPlatform/android/AJniHelper.h" // TODO: Ugh

namespace uBase
{
    using namespace Xli::PlatformSpecific;

    class AndroidAssetStream: public Stream
    {
        AAsset* asset;

    public:
        AndroidAssetStream(String filename, FileMode mode)
        {
            if (mode != FileModeRead && mode != FileModeReadRandom)
                throw Exception("Unsupported asset file mode: " + FileModeInfo::ToString(mode));

            AJniHelper jni;
            asset = AAssetManager_open(jni.GetAssetManager(), filename.Ptr(), ((mode & FileModeRandom) != 0) ? AASSET_MODE_RANDOM : AASSET_MODE_STREAMING);

            if (asset == 0)
                throw FileCantOpenException(filename);
        }

        virtual ~AndroidAssetStream()
        {
            AAsset_close(asset);
        }

        virtual bool CanRead() const
        {
            return true;
        }

        virtual bool CanSeek() const
        {
            return true;
        }

        int GetPosition() const
        {
            return (int)(AAsset_getLength(asset) - AAsset_getRemainingLength(asset));
        }

        int GetLength() const
        {
            return AAsset_getLength(asset);
        }

        int Read(void* data, int elmSize, int elmCount)
        {
            return AAsset_read(asset, data, elmSize * elmCount) / elmSize;
        }

        void Seek(int offset, SeekOrigin origin)
        {
            switch (origin)
            {
            case SeekOriginBegin:
                AAsset_seek(asset, offset, SEEK_SET);
                return;

            case SeekOriginCurrent:
                AAsset_seek(asset, offset, SEEK_CUR);
                return;

            case SeekOriginEnd:
                AAsset_seek(asset, offset, SEEK_END);
                return;
            }

            throw StreamCantSeekException();
        }
    };

    class AndroidAssetBuffer: public DataAccessor
    {
        AAsset* asset;

    public:
        AndroidAssetBuffer(String filename)
        {
            AJniHelper jni;
            asset = AAssetManager_open(jni.GetAssetManager(), filename.Ptr(), AASSET_MODE_BUFFER);

            if (asset == 0)
                throw FileCantOpenException(filename);
        }

        virtual ~AndroidAssetBuffer()
        {
            AAsset_close(asset);
        }

        virtual const uint8_t* GetPtr() const
        {
            return (const uint8_t*)AAsset_getBuffer(asset);
        }

        virtual int GetSizeInBytes() const
        {
            return AAsset_getLength(asset);
        }
    };

    class AndroidBundle: public FileSystem
    {
    public:
        virtual Stream* OpenFile(const String& filename, FileMode mode)
        {
            return new AndroidAssetStream(filename, mode);
        }

        virtual DataAccessor* OpenFileAsBuffer(const String& filename)
        {
            return new AndroidAssetBuffer(filename);
        }
    };

    class AndroidFileSystem: public UnixFileSystem
    {
        jmethodID Context_getCacheDir;
        jmethodID Context_getFilesDir;
        jmethodID Context_getExternalFilesDir;
        jmethodID File_getAbsolutePath;

    public:
        AndroidFileSystem()
        {
            AJniHelper jni;
            jclass contextCls = jni->FindClass("android/content/Context");
            Context_getCacheDir = jni->GetMethodID(contextCls, "getCacheDir", "()Ljava/io/File;");
            Context_getFilesDir = jni->GetMethodID(contextCls, "getFilesDir", "()Ljava/io/File;");
            Context_getExternalFilesDir = jni->GetMethodID(contextCls, "getExternalFilesDir", "(Ljava/lang/String;)Ljava/io/File;");

            jclass fileCls = jni->FindClass("java/io/File");
            File_getAbsolutePath = jni->GetMethodID(fileCls, "getAbsolutePath", "()Ljava/lang/String;");
        }

        virtual String GetBaseDirectory()
        {
            return "";
        }

        String GetDir(jmethodID mtd, bool nullArg)
        {
            AJniHelper jni;
            jobject cacheDir;
            if (nullArg) {
                cacheDir = jni->CallObjectMethod(jni.GetActivity(), mtd, NULL);
            } else {
                cacheDir = jni->CallObjectMethod(jni.GetActivity(), mtd);
            }
            jobject absolutePath = jni->CallObjectMethod(cacheDir, File_getAbsolutePath);

            const char* utf8 = jni->GetStringUTFChars((jstring)absolutePath, NULL);
            String result = utf8;
            jni->ReleaseStringUTFChars((jstring)absolutePath, utf8);
            jni->DeleteLocalRef(cacheDir);
            jni->DeleteLocalRef(absolutePath);
            return result;
        }

        virtual String GetTempDirectory()
        {
            return GetDir(Context_getCacheDir, false);
        }

        virtual String GetSystemDirectory(SystemDirectory dir)
        {
            switch (dir)
            {
            case SystemDirectoryConfig:
                return GetDir(Context_getExternalFilesDir, true);

            case SystemDirectoryData:
                return GetDir(Context_getFilesDir, false);

            // TODO: Conform to Android specifications on proper handling of system directories
            default:
                return "/sdcard";
            }
        }
    };

    FileSystem* CreateBundleFileSystem()
    {
        return new AndroidBundle();
    }

    NativeFileSystem* CreateNativeFileSystem()
    {
        return new AndroidFileSystem();
    }
}
