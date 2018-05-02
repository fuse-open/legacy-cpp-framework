#include <XliPlatform/Window.h>

namespace Xli
{
    namespace PlatformSpecific
    {
        class DummyWindow: public Window
        {
        public:
            virtual WindowImplementation GetImplementation() { return (WindowImplementation)0; }
            virtual void SetEventHandler(WindowEventHandler* handler) {}
            virtual WindowEventHandler* GetEventHandler() { return NULL; }
            virtual void Close() {}
            virtual bool IsClosed() { return false; }
            virtual bool IsVisible() { return true; }
            virtual void SetFullscreen(bool fullscreen) {}
            virtual bool IsFullscreen() { return true; }
            virtual void Minimize() {}
            virtual void Maximize() {}
            virtual void Restore() {}
            virtual bool IsMinimized() { return false; }
            virtual bool IsMaximized() { return true; }
            virtual void SetTitle(const String& title) {}
            virtual String GetTitle() { return "DummyWindow"; }
            virtual void SetPosition(Vector2i pos) {}
            virtual Vector2i GetPosition() { return Vector2i(0, 0); }
            virtual void SetClientSize(Vector2i size) {}
            virtual Vector2i GetClientSize() { return Vector2i(1280, 720); }
            virtual int GetDisplayIndex() { return 0; }
            virtual void* GetNativeHandle() { return NULL; }
            virtual bool GetKeyState(Key key) { return false; }
            virtual bool GetMouseButtonState(MouseButton button) { return false; }
            virtual void SetMousePosition(Vector2i position) {}
            virtual Vector2i GetMousePosition() { return Vector2i(0, 0); }
            virtual void SetSystemCursor(SystemCursor cursor) {}
            virtual SystemCursor GetSystemCursor() { return (SystemCursor)0; }
        };
    }

    void Window::SetMainWindow(Window* wnd) {}
    Window* Window::GetMainWindow() { return NULL; }
    Vector2i Window::GetScreenSize() { return Vector2i(1280, 720); }
    Window* Window::Create(int width, int height, const Xli::String& title, int flags) { return new PlatformSpecific::DummyWindow(); }
    Window* Window::CreateFrom(void* nativeWindowHandle) { return new PlatformSpecific::DummyWindow(); }
    void Window::ProcessMessages() {}
    void InitWindow() {}
    void TerminateWindow() {}
}
