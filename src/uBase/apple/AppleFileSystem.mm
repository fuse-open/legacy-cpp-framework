#import <Foundation/Foundation.h>
#include <uBase/Path.h>
#include "../unix/UnixFileSystem.h"

// We use this class with `bundleForClass:` instead of `mainBundle` to get a
// bundle path that works both when building a framework and building an
// application.
@interface uBaseBundleClass : NSObject
@end

@implementation uBaseBundleClass
@end

namespace uBase
{
    class AppleFileSystem: public UnixFileSystem
    {
    public:
        // TODO: Should return shared dir accessible from global filesystem
        virtual String GetSystemDirectory(SystemDirectory dir)
        {
            NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
            NSString* documentsDirectory = [paths objectAtIndex:0];
            return [documentsDirectory UTF8String];
        }

        virtual String GetBaseDirectory()
        {
            NSArray* arguments = [[NSProcessInfo processInfo] arguments];
            NSString* exe = [arguments objectAtIndex:0];
            return Path::GetDirectoryName([exe UTF8String]);
        }

        virtual String GetTempDirectory()
        {
            return [NSTemporaryDirectory() UTF8String];
        }
    };

    NativeFileSystem* CreateNativeFileSystem()
    {
        return new AppleFileSystem();
    }

    class AppleBundleFileSystem: public AppleFileSystem
    {
    public:
        virtual Stream* OpenFile(const String& filename, FileMode mode)
        {
            return new File(Path::Combine(GetBaseDirectory(), filename), mode);
        }

        virtual String GetBaseDirectory()
        {
            const char* bundlePath = [[[NSBundle bundleForClass:[uBaseBundleClass class]] resourcePath] UTF8String];
            return Path::Combine(bundlePath, "data");
        }
    };

    FileSystem* CreateBundleFileSystem()
    {
        return new AppleBundleFileSystem();
    }
}
