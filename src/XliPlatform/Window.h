#pragma once
#include <uBase/Object.h>
#include <uBase/Vector2.h>

namespace Xli
{
    using namespace uBase;

    class Window;

    /** \ingroup XliPlatform */
    enum Key
    {
        KeyUnknown = 0,

        KeyBackspace = 8,
        KeyTab = 9,

        KeyEnter = 13,

        KeyShift = 16,
        KeyCtrl = 17,
        KeyAlt = 18,
        KeyBreak = 19,
        KeyCapsLock = 20,

        KeyEscape = 27,

        KeySpace = 32,
        KeyPageUp = 33,
        KeyPageDown = 34,
        KeyEnd = 35,
        KeyHome = 36,
        KeyLeft = 37,
        KeyUp = 38,
        KeyRight = 39,
        KeyDown = 40,
        KeyInsert = 45,
        KeyDelete = 46,

        Key0 = 48,
        Key1 = 49,
        Key2 = 50,
        Key3 = 51,
        Key4 = 52,
        Key5 = 53,
        Key6 = 54,
        Key7 = 55,
        Key8 = 56,
        Key9 = 57,

        KeyA = 65,
        KeyB = 66,
        KeyC = 67,
        KeyD = 68,
        KeyE = 69,
        KeyF = 70,
        KeyG = 71,
        KeyH = 72,
        KeyI = 73,
        KeyJ = 74,
        KeyK = 75,
        KeyL = 76,
        KeyM = 77,
        KeyN = 78,
        KeyO = 79,
        KeyP = 80,
        KeyQ = 81,
        KeyR = 82,
        KeyS = 83,
        KeyT = 84,
        KeyU = 85,
        KeyV = 86,
        KeyW = 87,
        KeyX = 88,
        KeyY = 89,
        KeyZ = 90,

        KeyNumPad0 = 96,
        KeyNumPad1 = 97,
        KeyNumPad2 = 98,
        KeyNumPad3 = 99,
        KeyNumPad4 = 100,
        KeyNumPad5 = 101,
        KeyNumPad6 = 102,
        KeyNumPad7 = 103,
        KeyNumPad8 = 104,
        KeyNumPad9 = 105,

        KeyF1 = 112,
        KeyF2 = 113,
        KeyF3 = 114,
        KeyF4 = 115,
        KeyF5 = 116,
        KeyF6 = 117,
        KeyF7 = 118,
        KeyF8 = 119,
        KeyF9 = 120,
        KeyF10 = 121,
        KeyF11 = 122,
        KeyF12 = 123,

        KeyMenu = 200,
        BackButton = 201,
    };

    /** \ingroup XliPlatform
        Enumerates the standard mouse buttons. */
    enum MouseButton
    {
        MouseButtonUnknown = 0,

        /** Left mouse button */
        MouseButtonLeft = 1,

        /** Middle mouse button */
        MouseButtonMiddle = 2,

        /** Right mouse button */
        MouseButtonRight = 3,

        /** Extra mouse button 1 (aka Mouse4) */
        MouseButtonX1 = 4,

        /** Extra mouse button 2 (aka Mouse5) */
        MouseButtonX2 = 5
    };

    /** \ingroup XliPlatform */
    class WindowEventHandler: public Object
    {
    public:
        virtual bool OnKeyDown(Window* wnd, Key key);
        virtual bool OnKeyUp(Window* wnd, Key key);
        virtual bool OnTextInput(Window* wnd, const String& text);
        virtual bool OnKeyboardResized(Window* wnd);

        virtual bool OnMouseDown(Window* wnd, Vector2i pos, MouseButton button);
        virtual bool OnMouseUp(Window* wnd, Vector2i pos, MouseButton button);
        virtual bool OnMouseMove(Window* wnd, Vector2i pos);
        virtual bool OnMouseWheel(Window* wnd, Vector2i delta);

        virtual bool OnTouchDown(Window* wnd, Vector2 pos, int id);
        virtual bool OnTouchMove(Window* wnd, Vector2 pos, int id);
        virtual bool OnTouchUp(Window* wnd, Vector2 pos, int id);

        virtual void OnSizeChanged(Window* wnd);
        virtual void OnNativeHandleChanged(Window* wnd);

        virtual bool OnClosing(Window* wnd);
        virtual void OnClosed(Window* wnd);

        virtual void OnAppLowMemory(Window* wnd);
        virtual void OnAppTerminating(Window* wnd);
        virtual void OnAppWillEnterForeground(Window* wnd);
        virtual void OnAppDidEnterForeground(Window* wnd);
        virtual void OnAppWillEnterBackground(Window* wnd);
        virtual void OnAppDidEnterBackground(Window* wnd);
    };

    /** \ingroup XliPlatform */
    enum SystemCursor
    {
        SystemCursorNone = 0,
        SystemCursorArrow = 1,
        SystemCursorIBeam = 2,
        SystemCursorWait = 3,
        SystemCursorCrosshair = 4,
        SystemCursorWaitArrow = 5,
        SystemCursorSizeNWSE = 6,
        SystemCursorSizeNESW = 7,
        SystemCursorSizeWE = 8,
        SystemCursorSizeNS = 9,
        SystemCursorSizeAll = 10,
        SystemCursorNo = 11,
        SystemCursorHand = 12,
    };

    /** \ingroup XliPlatform */
    enum TextInputHint
    {
        TextInputHintDefault,
        TextInputHintEmail,
        TextInputHintURL,
        TextInputHintPhone,
        TextInputHintNumber
    };

    /** \ingroup XliPlatform */
    enum WindowFlags
    {
        WindowFlagsFixed = 0, ///< Fixed size window with a title bar and minimize and close buttons
        WindowFlagsBorderless = 1 << 0, ///< Borderless window without titlebar and buttons
        WindowFlagsResizeable = 1 << 1, ///< Resizeable window with a title bar and minimize, maximize and close buttons
        WindowFlagsFullscreen = 1 << 2, ///< Fullscreen window which should always be on top

        /// Disables power saver on mobile platforms.
        /// This is useful to avoid the screen from going black for apps not using touch input
        /// -- such as sensor based apps, demos, video players, etc. On desktop platforms this is
        /// the default behaviour when in fullscreen mode, but on mobile it must be explicitly
        /// enabled for apps that needs it because it can potentially drain the battery.
        WindowFlagsDisablePowerSaver = 1 << 3,

        /// Disables running as background process on applicable platforms (i.e. Android)
        WindowFlagsDisableBackgroundProcess = 1 << 4,
    };

    /** \ingroup XliPlatform */
    enum WindowImplementation
    {
        WindowImplementationUnknown,
        WindowImplementationWin32,
        WindowImplementationAndroid,
        WindowImplementationSDL2,
    };

    /** \ingroup XliPlatform
        Represents a window in the native window system. */
    class Window: public Object
    {
    public:
        virtual ~Window() {}

        /** Returns the window implementation type */
        virtual WindowImplementation GetImplementation() = 0;

        /** Sets the handler that should recieve events from the window */
        virtual void SetEventHandler(WindowEventHandler* handler) = 0;

        /** Returns the current event handler for the window */
        virtual WindowEventHandler* GetEventHandler() = 0;

        /** Closes the window */
        virtual void Close() = 0;

        /** Returns wether or not the window has been closed by the user or operating system. */
        virtual bool IsClosed() = 0;

        /** Returns wether or not the window is currently visible (i.e. not minimized or hidden). */
        virtual bool IsVisible() = 0;

        /** Sets the fullscreen mode of the window.
            The window will go fullscreen on the display which contains the windows centre coordinate and stretched to that displays resolution. */
        virtual void SetFullscreen(bool fullscreen) = 0;

        /** Returns if this is a fullscreen window */
        virtual bool IsFullscreen() = 0;

        /** Minimizes the window */
        virtual void Minimize() = 0;

        /** Maximizes the window */
        virtual void Maximize() = 0;

        /** Restores the window from maximized/minimized state to normal position */
        virtual void Restore() = 0;

        /** Returns if this window is minimized */
        virtual bool IsMinimized() = 0;

        /** Returns if this window is maximized */
        virtual bool IsMaximized() = 0;

        /** Sets the windows title */
        virtual void SetTitle(const String& title) = 0;

        /** Returns the windows title */
        virtual String GetTitle() = 0;

        /** Sets the position of the window */
        virtual void SetPosition(Vector2i pos) = 0;

        /** Returns the position of the window */
        virtual Vector2i GetPosition() = 0;

        /** Sets the size of the client area of the window */
        virtual void SetClientSize(Vector2i size) = 0;

        /** Returns the size of the client area of the window */
        virtual Vector2i GetClientSize() = 0;

        /** Returns the index for the display containing this windows center coordinate */
        virtual int GetDisplayIndex() = 0;

        /** Returns the native windows handle */
        virtual void* GetNativeHandle() = 0;

        /** Returns true if the specified Key is currently pressed */
        virtual bool GetKeyState(Key key) = 0;

        /** Returns true if the specified MouseButton is currently pressed */
        virtual bool GetMouseButtonState(MouseButton button) = 0;

        /** Sets the current mouse position relative to the client area of the window */
        virtual void SetMousePosition(Vector2i position) = 0;

        /** Returns the current mouse position relative to the client area of the window */
        virtual Vector2i GetMousePosition() = 0;

        /** Sets the system cursor to be used in the window */
        virtual void SetSystemCursor(SystemCursor cursor) = 0;

        /** Returns the current system cursor used in the window */
        virtual SystemCursor GetSystemCursor() = 0;

        virtual void BeginTextInput(TextInputHint hint) { }
        virtual void EndTextInput() { }
        virtual bool IsTextInputActive() { return false; }

        virtual bool HasOnscreenKeyboardSupport() { return false; }
        virtual bool IsOnscreenKeyboardVisible() { return false; }

        virtual void SetOnscreenKeyboardPosition(Vector2i position) { }
        virtual Vector2i GetOnscreenKeyboardPosition() { return Vector2i(0, 0); }
        virtual Vector2i GetOnscreenKeyboardSize() { return Vector2i(0, 0); }

        virtual bool IsStatusBarVisible() { return false; }
        virtual Vector2i GetStatusBarPosition() { return Vector2i(0, 0); }
        virtual Vector2i GetStatusBarSize() { return Vector2i(0, 0); }

        /** Sets the window that should be used as main window */
        static void SetMainWindow(Window* wnd);

        /** Returns the first window created or the window currently set using SetMainWindow().
            If no window is created or the main window is destroyed this function will return NULL. */
        static Window* GetMainWindow();

        /** Returns the size of the primary monitor */
        static Vector2i GetScreenSize();

        /** Creates a window */
        static Window* Create(int width, int height, const Xli::String& title, int flags = 0);

        /** Creates a window */
        static Window* Create(const Vector2i& size, const Xli::String& title, int flags = 0)
        {
            return Create(size.X, size.Y, title, flags);
        }

        /** Adopts a native window */
        static Window* CreateFrom(void* nativeWindowHandle);

        /** Process messages for the application to keep user interface responsive. */
        static void ProcessMessages();
    };
}