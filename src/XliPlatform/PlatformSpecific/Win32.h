#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// Undefine conflicting names
#undef MessageBox
#undef GetSystemDirectory
#undef GetCurrentDirectory
#undef SetCurrentDirectory
#undef CreateDirectory
#undef ChangeDirectory
#undef DeleteFile
#undef MoveFile
#undef CopyFile
#undef CreateMutex
#undef GetCurrentThread

#include <uBase/String.h>

namespace Xli
{
    using namespace uBase;
    class Window;

    namespace PlatformSpecific
    {
        /** \ingroup XliPlatform */
        class Win32
        {
        public:
            /**     Returns a String describing the error code returned from ::GetLastError()
            */
            static String GetLastErrorString();

            static void RegisterTouchWindow(Window* wnd);
            static HWND GetWindowHandle(Window* wnd);
            static void SetWindowIconByID(Window* wnd, int id);
        };
    }
}
