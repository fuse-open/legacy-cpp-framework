#pragma once
#include <XliPlatform/Window.h>
#include <uBase/Memory.h>
#include <SDL2/SDL.h>

namespace Xli
{
    namespace PlatformSpecific
    {
        /** \ingroup XliPlatform */
        class SDL2Window: public Window
        {
        public:
            Shared<WindowEventHandler> eventHandler;
            SDL_Window* window;
            bool closed, fullscreen;
            int x, y, w, h, buttons;
            Vector2i keyboardSize;

            SDL2Window(int width, int height, const Xli::String& title, int flags);
            SDL2Window(const void* nativeHandle);
            virtual ~SDL2Window();

            virtual void SDL2WindowInit();

            virtual WindowImplementation GetImplementation();

            virtual void SetEventHandler(WindowEventHandler* handler);
            virtual WindowEventHandler* GetEventHandler();

            virtual bool IsClosed();
            virtual bool IsVisible();
            virtual bool IsFullscreen();
            virtual bool IsMinimized();
            virtual bool IsMaximized();

            virtual int GetDisplayIndex();
            virtual String GetTitle();
            virtual Vector2i GetPosition();
            virtual Vector2i GetClientSize();
            virtual void* GetNativeHandle();

            virtual void SetTitle(const String& title);
            virtual void SetFullscreen(bool fullscreen);
            virtual void SetPosition(Vector2i pos);
            virtual void SetClientSize(Vector2i size);

            virtual void Close();

            virtual void Minimize();
            virtual void Maximize();
            virtual void Restore();

            virtual bool GetKeyState(Key key);
            virtual bool GetMouseButtonState(MouseButton button);
            virtual Vector2i GetMousePosition();
            virtual void SetMousePosition(Vector2i position);

            virtual SystemCursor GetSystemCursor();
            virtual void SetSystemCursor(SystemCursor cursor);

            virtual void BeginTextInput(TextInputHint hint);
            virtual void EndTextInput();
            virtual bool IsTextInputActive();

            virtual bool HasOnscreenKeyboardSupport();
            virtual bool IsOnscreenKeyboardVisible();

            virtual void SetOnscreenKeyboardPosition(Vector2i position);
            virtual Vector2i GetOnscreenKeyboardPosition();
            virtual Vector2i GetOnscreenKeyboardSize();

            virtual bool IsStatusBarVisible();
            virtual Vector2i GetStatusBarPosition();
            virtual Vector2i GetStatusBarSize();
        };
    }
}
