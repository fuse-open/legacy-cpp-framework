#pragma once
#include <XliPlatform/PlatformSpecific/Win32.h>
#include <XliPlatform/Window.h>
#include <uBase/Memory.h>

namespace Xli
{
    namespace PlatformSpecific
    {
        using namespace uBase;

        /** \ingroup XliPlatform */
        class Win32Window : public Window
        {
        public:
            HWND hWnd;
            bool closed, ownsHwnd;

            Shared<WindowEventHandler> eventHandler;

            DWORD dwStyle;
            RECT rect;
            bool fullscreen;

            static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

            Win32Window(int width, int height, const Xli::String& title, int flags);
            Win32Window(HWND hWnd);
            virtual ~Win32Window();

            virtual WindowImplementation GetImplementation();

            virtual void SetEventHandler(WindowEventHandler* handler);
            virtual WindowEventHandler* GetEventHandler();

            virtual void Close();

            virtual bool IsClosed();
            virtual bool IsVisible();
            virtual bool IsFullscreen();
            virtual bool IsMinimized();
            virtual bool IsMaximized();

            virtual int GetDisplayIndex();

            virtual String GetTitle();
            virtual Vector2i GetPosition();
            virtual Vector2i GetClientSize();
            virtual void* GetNativeHandle() { return (void*)hWnd; }

            virtual void SetTitle(const String& title);
            virtual void SetFullscreen(bool fullscren);
            virtual void SetPosition(Vector2i pos);
            virtual void SetClientSize(Vector2i size);

            virtual void Minimize();
            virtual void Maximize();
            virtual void Restore();

            virtual bool GetKeyState(Key key);
            virtual bool GetMouseButtonState(MouseButton button);
            virtual Vector2i GetMousePosition();
            virtual void SetMousePosition(Vector2i position);

            virtual SystemCursor GetSystemCursor();
            virtual void SetSystemCursor(SystemCursor cursor);
        };
    }
}
