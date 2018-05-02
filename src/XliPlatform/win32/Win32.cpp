#include <XliPlatform/PlatformSpecific/Win32.h>
#include <XliPlatform/Window.h>
#include <uBase/Unicode.h>

namespace Xli
{
    namespace PlatformSpecific
    {
        String Win32::GetLastErrorString()
        {
            LPWSTR lpMsgBuf;
            DWORD dw = GetLastError();

            FormatMessageW(
                FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                dw,
                MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
                (LPWSTR)&lpMsgBuf, // Cast because callee is allocating buffer
                0, NULL);

            String msg = Unicode::Utf16To8(lpMsgBuf);
            LocalFree(lpMsgBuf);

            return msg;
        }

        HWND Win32::GetWindowHandle(Window* wnd)
        {
            if (!wnd || wnd->GetImplementation() != WindowImplementationWin32)
                return NULL;

            return (HWND)wnd->GetNativeHandle();
        }

        void Win32::RegisterTouchWindow(Window* wnd)
        {
            ::RegisterTouchWindow(GetWindowHandle(wnd), 0);
        }

        void Win32::SetWindowIconByID(Window* wnd, int id)
        {
            SendMessage(GetWindowHandle(wnd), WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(id)));
            SendMessage(GetWindowHandle(wnd), WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(id)));
        }
    }
}
