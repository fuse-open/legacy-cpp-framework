#include <android/window.h>
#include <cstdlib>
#include <errno.h>
#include <unistd.h>

#include <XliPlatform/PlatformSpecific/Android.h>
#include <XliPlatform/Window.h>
#include <uBase/Console.h>
#include "AJniHelper.h"
#include "ALogStream.h"

namespace Xli
{
    namespace PlatformSpecific
    {
        void Android::PreInit(JavaVM* jvm, JNIEnv* env, jclass globalRefdClass)
        {
            AJniHelper::Init(jvm, env, globalRefdClass, 0);
        }

        void Android::PostInit()
        {
            Out->SetStream(AutoPtr(new ALogStream(ANDROID_LOG_INFO)));
            Error->SetStream(AutoPtr(new ALogStream(ANDROID_LOG_WARN)));
        }

        void Android::SetLogTag(const char* tag)
        {
            setenv("U_APP_NAME", tag, 1);
        }

        class AWindow: public Window
        {
        public:
            AWindow()
            {
                LOGE("XLI ANDROID IS DEPRECATED");
                throw Exception("XLI ANDROID IS DEPRECATED");
            }

            virtual ~AWindow() {}

            virtual WindowImplementation GetImplementation() { return WindowImplementationAndroid; }
            virtual void SetEventHandler(WindowEventHandler* handler) {}
            virtual WindowEventHandler* GetEventHandler() { return NULL; }
            virtual void Close() {}
            virtual bool IsClosed() {}
            virtual bool IsVisible() { return false; }
            virtual bool IsFullscreen() { return true; }
            virtual bool IsMinimized() { return false; }
            virtual bool IsMaximized() { return true; }
            virtual bool IsStatusBarVisible() { return true; }
            virtual Vector2i GetStatusBarPosition() { return Vector2i(0, 0); }
            virtual Vector2i GetStatusBarSize() { return Vector2i(0, 0); }
            virtual int GetDisplayIndex() { return 0; }
            virtual String GetTitle() { return ""; }
            virtual Vector2i GetPosition() { return Vector2i(0, 0); }
            virtual Vector2i GetClientSize() { return Vector2i(0, 0); }
            virtual void* GetNativeHandle() {}
            virtual void SetTitle(const String& title) {}
            virtual void SetFullscreen(bool fullscreen) {}
            virtual void SetPosition(Vector2i pos) {}
            virtual void SetClientSize(Vector2i size) {}
            virtual void Minimize() {}
            virtual void Maximize() {}
            virtual void Restore() {}
            virtual void ShowCursor(bool show) {}
            virtual void BeginTextInput(TextInputHint hint) {}
            virtual void EndTextInput() {}
            virtual bool IsTextInputActive() {}
            virtual bool HasOnscreenKeyboardSupport() { return true; }
            virtual bool IsOnscreenKeyboardVisible() { return false; }
            virtual Vector2i GetOnscreenKeyboardPosition() { return Vector2i(0, 0); }
            virtual Vector2i GetOnscreenKeyboardSize() { return Vector2i(0, 0); }
            virtual bool GetKeyState(Key key) { return false; }
            virtual bool GetMouseButtonState(MouseButton button) { return false; }
            virtual Vector2i GetMousePosition() { return Vector2i(0, 0); }
            virtual void SetMousePosition(Vector2i position) {}
            virtual SystemCursor GetSystemCursor() { return SystemCursorNone; }
            virtual void SetSystemCursor(SystemCursor cursor) {}
        };
    }

    void InitWindow() {}
    void TerminateWindow() {}
    void Window::SetMainWindow(Window* wnd) {}
    void Window::ProcessMessages() {}
    Vector2i Window::GetScreenSize() { return Vector2i(0, 0); }

    Window* Window::GetMainWindow()
    {
        return NULL;
    }
    Window* Window::Create(int width, int height, const String& title, int flags)
    {
        LOGE("XLI ANDROID IS DEPRECATED");
        throw Exception("XLI ANDROID IS DEPRECATED");
    }
    Window* Window::CreateFrom(void* nativeWindowHandle)
    {
        throw NotSupportedException(U_FUNCTION);
    }

}
