#include <uBase/Array.h>
#include <uBase/BaseLib.h>
#include <uBase/Unicode.h>
#include <cstdlib>

#if IOS
#include <XliPlatform/PlatformSpecific/iOS.h>
#endif

#if WIN32
#include <XliPlatform/PlatformSpecific/Win32.h>
#include <Shellapi.h>
#endif

extern int Main(const uBase::Array<uBase::String>& args);

extern "C" int main(int argc, char** argv)
{
    int result = EXIT_FAILURE;

    try
    {
        uBase::BaseLib::Init();

#if IOS
        Xli::PlatformSpecific::iOS::Init();
#endif

#if WIN32
        int numArgs;
        LPWSTR* cmdArgs = CommandLineToArgvW(GetCommandLineW(), &numArgs);
        uBase::Array<uBase::String> args(numArgs);
        for (int i = 0; i < numArgs; i++)
            args[i] = uBase::Unicode::Utf16To8(cmdArgs[i]);
#else
        uBase::Array<uBase::String> args(argc);
        for (int i = 0; i < argc; i++)
            args[i] = argv[i];
#endif
        result = Main(args);
    }
    catch (const uBase::Exception& e)
    {
        uBase::BaseLib::OnUnhandledException(e, "main");
    }
    catch (...)
    {
        uBase::Exception e("An unsupported C++ exception was thrown");
        uBase::BaseLib::OnUnhandledException(e, "main");
    }

    return result;
}

#if WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    return main(__argc, __argv);
}
#endif
