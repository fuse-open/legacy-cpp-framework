#include <XliPlatform/GL.h>
#include <uBase/Unicode.h>
#include <uBase/Console.h>

#include <XliPlatform/PlatformLib.h>
#include <XliPlatform/PlatformSpecific/SDL2.h>
#include "SDL2Window.h"
#include <SDL2/SDL_syswm.h>

Xli::PlatformSpecific::SDL2Window* GlobalWindow = 0;

namespace Xli
{
    static bool QuitRecieved = false;
    static int CancelCount = 0;

    namespace PlatformSpecific
    {
        SDL_Window* SDL2::GetWindowHandle(Window* wnd)
        {
            return wnd != 0 && wnd->GetImplementation() == WindowImplementationSDL2 ? ((PlatformSpecific::SDL2Window*)wnd)->window : NULL;
        }

#ifdef IOS

        static int HandleAppEvents(void *userdata, SDL_Event *event)
        {
            switch (event->type)
            {
            case SDL_APP_TERMINATING:
                if (GlobalWindow && GlobalWindow->GetEventHandler())
                {
                    GlobalWindow->GetEventHandler()->OnAppTerminating(GlobalWindow);

                    GlobalWindow->closed = true;
                    GlobalWindow->GetEventHandler()->OnClosed(GlobalWindow);
                }

                return 0;

            case SDL_APP_LOWMEMORY:
                if (GlobalWindow && GlobalWindow->GetEventHandler())
                {
                    GlobalWindow->GetEventHandler()->OnAppLowMemory(GlobalWindow);
                }

                return 0;

            case SDL_APP_WILLENTERBACKGROUND:
                if (GlobalWindow && GlobalWindow->GetEventHandler())
                {
                    GlobalWindow->GetEventHandler()->OnAppWillEnterBackground(GlobalWindow);
                }

                return 0;

            case SDL_APP_DIDENTERBACKGROUND:
                if (GlobalWindow && GlobalWindow->GetEventHandler())
                {
                    GlobalWindow->GetEventHandler()->OnAppDidEnterBackground(GlobalWindow);
                }

                return 0;

            case SDL_APP_WILLENTERFOREGROUND:
                if (GlobalWindow && GlobalWindow->GetEventHandler())
                {
                    GlobalWindow->GetEventHandler()->OnAppWillEnterForeground(GlobalWindow);
                }

                return 0;

            case SDL_APP_DIDENTERFOREGROUND:
                if (GlobalWindow && GlobalWindow->GetEventHandler())
                {
                    GlobalWindow->GetEventHandler()->OnAppDidEnterForeground(GlobalWindow);
                }

                return 0;
            }

            return 1;
        }

#endif

        SDL2Window::SDL2Window(int width, int height, const String& title, int flags)
        {
            if (GlobalWindow == 0)
                GlobalWindow = this;

            closed = false;
            fullscreen = false;

            if ((flags & WindowFlagsFullscreen) == WindowFlagsFullscreen)
            {
                flags &= ~WindowFlagsFullscreen;
                fullscreen = true;
            }

            x = SDL_WINDOWPOS_UNDEFINED;
            y = SDL_WINDOWPOS_UNDEFINED;
            w = width;
            h = height;
            buttons = 0;

            Uint32 sdlFlags = SDL_WINDOW_OPENGL;

            if (flags & WindowFlagsBorderless)
                sdlFlags |= SDL_WINDOW_BORDERLESS;

            if (flags & WindowFlagsResizeable)
                sdlFlags |= SDL_WINDOW_RESIZABLE;

#ifdef OSX

            // Enable support for Retina display
            sdlFlags |= SDL_WINDOW_ALLOW_HIGHDPI;

            // Enable OS X Lion native fullscreen feature
            //SDL_SetHint(SDL_HINT_VIDEO_FULLSCREEN_SPACES, "1");

#endif

#ifdef IOS

            if (flags & WindowFlagsDisablePowerSaver && !SDL_SetHint(SDL_HINT_IDLE_TIMER_DISABLED, "1"))
                Error->WriteLine("SDL WARNING: Failed to disable idle timer");

            if (fullscreen)
                sdlFlags |= SDL_WINDOW_BORDERLESS;

            sdlFlags |= SDL_WINDOW_RESIZABLE;

            SDL_SetEventFilter(HandleAppEvents, NULL);

            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#else

            sdlFlags |= SDL_WINDOW_OPENGL;

#endif

            window = SDL_CreateWindow(title.Ptr(), x, y, width, height, sdlFlags);

            SDL_SetWindowData(window, "SDL2Window", this);

#ifndef IOS

            if (fullscreen)
            {
                fullscreen = false;
                SetFullscreen(true);
            }

#else
            this->SDL2WindowInit();
#endif
        }

        SDL2Window::SDL2Window(const void* nativeHandle)
        {
            if (GlobalWindow == 0)
                GlobalWindow = this;

            window = SDL_CreateWindowFrom(nativeHandle);

            SDL_GetWindowSize(window, &w, &h);
            SDL_GetWindowPosition(window, &x, &y);

            closed = false;
            fullscreen = false;
        }

        SDL2Window::~SDL2Window()
        {
            if (GlobalWindow == this)
                GlobalWindow = 0;

            SDL_DestroyWindow(window);
        }

#ifndef IOS
        void SDL2Window::SDL2WindowInit()
        {
        }
#endif

        WindowImplementation SDL2Window::GetImplementation()
        {
            return WindowImplementationSDL2;
        }

        void SDL2Window::SetEventHandler(WindowEventHandler* handler)
        {
            eventHandler = handler;
        }

        WindowEventHandler* SDL2Window::GetEventHandler()
        {
            return eventHandler;
        }

        void SDL2Window::Close()
        {
            if (!closed)
            {
                if (eventHandler &&
                    eventHandler->OnClosing(this))
                    return;

                SDL_HideWindow(window);
                closed = true;
                eventHandler->OnClosed(this);
            }
        }

        bool SDL2Window::IsClosed()
        {
            return closed || QuitRecieved;
        }

        bool SDL2Window::IsVisible()
        {
            return true;
        }

        bool SDL2Window::IsFullscreen()
        {
            return fullscreen;
        }

        bool SDL2Window::IsMinimized()
        {
            return (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED) == SDL_WINDOW_MINIMIZED;
        }

        bool SDL2Window::IsMaximized()
        {
            return (SDL_GetWindowFlags(window) & SDL_WINDOW_MAXIMIZED) == SDL_WINDOW_MAXIMIZED;
        }

        int SDL2Window::GetDisplayIndex()
        {
            return SDL_GetWindowDisplayIndex(window);
        }

        String SDL2Window::GetTitle()
        {
            return SDL_GetWindowTitle(window);
        }

        Vector2i SDL2Window::GetPosition()
        {
            Vector2i pos;
            SDL_GetWindowPosition(window, &pos.X, &pos.Y);
            return pos;
        }

        Vector2i SDL2Window::GetClientSize()
        {
            Vector2i size;
            SDL_GetWindowSize(window, &size.X, &size.Y);
            return size;
        }

        void* SDL2Window::GetNativeHandle()
        {
            SDL_SysWMinfo info;
            if (SDL_GetWindowWMInfo(window, &info))
            {
                // TODO: This may need to be changed for X11 platforms
                return *(void**)&info.info;
            }

            return 0;
        }

        void SDL2Window::SetTitle(const String& title)
        {
            SDL_SetWindowTitle(window, title.Ptr());
        }

        void SDL2Window::SetFullscreen(bool fullscreen)
        {
            if (fullscreen != this->fullscreen)
            {
                this->fullscreen = fullscreen;

#ifdef IOS

                SDL_SetWindowFullscreen(window, fullscreen ? SDL_TRUE : SDL_FALSE);

#else

                if (fullscreen)
                {
                    SDL_GetWindowPosition(window, &x, &y);
                    SDL_GetWindowSize(window, &w, &h);

                    int i = SDL_GetWindowDisplayIndex(window);
                    if (i == -1) i = 0;

                    SDL_Rect rect;
                    SDL_GetDisplayBounds(i, &rect);
                    SDL_SetWindowPosition(window, rect.x, rect.y);
                    SDL_SetWindowSize(window, rect.w, rect.h);

                    SDL_SetWindowFullscreen(window, SDL_TRUE);
                }
                else
                {
                    SDL_SetWindowFullscreen(window, SDL_FALSE);
                    SDL_SetWindowPosition(window, x, y);
                    SDL_SetWindowSize(window, w, h);
                }

#endif
            }
        }

        void SDL2Window::SetPosition(Vector2i pos)
        {
            SDL_SetWindowPosition(window, pos.X, pos.Y);
        }

        void SDL2Window::SetClientSize(Vector2i size)
        {
            SDL_SetWindowSize(window, size.X, size.Y);
        }

        void SDL2Window::Minimize()
        {
            SDL_MinimizeWindow(window);
        }

        void SDL2Window::Maximize()
        {
            SDL_MinimizeWindow(window);
        }

        void SDL2Window::Restore()
        {
            SDL_RestoreWindow(window);
        }

        bool SDL2Window::GetKeyState(Key key)
        {
#ifndef IOS

            const Uint8* keys = SDL_GetKeyboardState(0);

            switch (key)
            {
                case KeyBackspace: return keys[SDL_SCANCODE_BACKSPACE];
                case KeyTab: return keys[SDL_SCANCODE_TAB];
                case KeyEnter: return keys[SDL_SCANCODE_RETURN] || keys[SDL_SCANCODE_RETURN2];
                case KeyShift: return keys[SDL_SCANCODE_LSHIFT] || keys[SDL_SCANCODE_RSHIFT];
                case KeyCtrl: return keys[SDL_SCANCODE_LCTRL] || keys[SDL_SCANCODE_RCTRL];
                case KeyAlt: return keys[SDL_SCANCODE_LALT] || keys[SDL_SCANCODE_RALT];
                case KeyBreak: return keys[SDL_SCANCODE_PAUSE];
                case KeyCapsLock: return keys[SDL_SCANCODE_CAPSLOCK];
                case KeyEscape: return keys[SDL_SCANCODE_ESCAPE];
                case KeySpace: return keys[SDL_SCANCODE_SPACE];
                case KeyPageUp: return keys[SDL_SCANCODE_PAGEUP];
                case KeyPageDown: return keys[SDL_SCANCODE_PAGEDOWN];
                case KeyEnd: return keys[SDL_SCANCODE_END];
                case KeyHome: return keys[SDL_SCANCODE_HOME];
                case KeyLeft: return keys[SDL_SCANCODE_LEFT];
                case KeyUp: return keys[SDL_SCANCODE_UP];
                case KeyRight: return keys[SDL_SCANCODE_RIGHT];
                case KeyDown: return keys[SDL_SCANCODE_DOWN];
                case KeyInsert: return keys[SDL_SCANCODE_INSERT];
                case KeyDelete: return keys[SDL_SCANCODE_DELETE];
                case Key0: return keys[SDL_SCANCODE_0];
                case Key1: return keys[SDL_SCANCODE_1];
                case Key2: return keys[SDL_SCANCODE_2];
                case Key3: return keys[SDL_SCANCODE_3];
                case Key4: return keys[SDL_SCANCODE_4];
                case Key5: return keys[SDL_SCANCODE_5];
                case Key6: return keys[SDL_SCANCODE_6];
                case Key7: return keys[SDL_SCANCODE_7];
                case Key8: return keys[SDL_SCANCODE_8];
                case Key9: return keys[SDL_SCANCODE_9];
                case KeyA: return keys[SDL_SCANCODE_A];
                case KeyB: return keys[SDL_SCANCODE_B];
                case KeyC: return keys[SDL_SCANCODE_C];
                case KeyD: return keys[SDL_SCANCODE_D];
                case KeyE: return keys[SDL_SCANCODE_E];
                case KeyF: return keys[SDL_SCANCODE_F];
                case KeyG: return keys[SDL_SCANCODE_G];
                case KeyH: return keys[SDL_SCANCODE_H];
                case KeyI: return keys[SDL_SCANCODE_I];
                case KeyJ: return keys[SDL_SCANCODE_J];
                case KeyK: return keys[SDL_SCANCODE_K];
                case KeyL: return keys[SDL_SCANCODE_L];
                case KeyM: return keys[SDL_SCANCODE_M];
                case KeyN: return keys[SDL_SCANCODE_N];
                case KeyO: return keys[SDL_SCANCODE_O];
                case KeyP: return keys[SDL_SCANCODE_P];
                case KeyQ: return keys[SDL_SCANCODE_Q];
                case KeyR: return keys[SDL_SCANCODE_R];
                case KeyS: return keys[SDL_SCANCODE_S];
                case KeyT: return keys[SDL_SCANCODE_T];
                case KeyU: return keys[SDL_SCANCODE_U];
                case KeyV: return keys[SDL_SCANCODE_V];
                case KeyW: return keys[SDL_SCANCODE_W];
                case KeyX: return keys[SDL_SCANCODE_X];
                case KeyY: return keys[SDL_SCANCODE_Y];
                case KeyZ: return keys[SDL_SCANCODE_Z];
                case KeyNumPad0: return keys[SDL_SCANCODE_KP_0];
                case KeyNumPad1: return keys[SDL_SCANCODE_KP_1];
                case KeyNumPad2: return keys[SDL_SCANCODE_KP_2];
                case KeyNumPad3: return keys[SDL_SCANCODE_KP_3];
                case KeyNumPad4: return keys[SDL_SCANCODE_KP_4];
                case KeyNumPad5: return keys[SDL_SCANCODE_KP_5];
                case KeyNumPad6: return keys[SDL_SCANCODE_KP_6];
                case KeyNumPad7: return keys[SDL_SCANCODE_KP_7];
                case KeyNumPad8: return keys[SDL_SCANCODE_KP_8];
                case KeyNumPad9: return keys[SDL_SCANCODE_KP_9];
                case KeyF1: return keys[SDL_SCANCODE_F1];
                case KeyF2: return keys[SDL_SCANCODE_F2];
                case KeyF3: return keys[SDL_SCANCODE_F3];
                case KeyF4: return keys[SDL_SCANCODE_F4];
                case KeyF5: return keys[SDL_SCANCODE_F5];
                case KeyF6: return keys[SDL_SCANCODE_F6];
                case KeyF7: return keys[SDL_SCANCODE_F7];
                case KeyF8: return keys[SDL_SCANCODE_F8];
                case KeyF9: return keys[SDL_SCANCODE_F9];
                case KeyF10: return keys[SDL_SCANCODE_F10];
                case KeyF11: return keys[SDL_SCANCODE_F11];
                case KeyF12: return keys[SDL_SCANCODE_F12];

                case KeyMenu:
                case KeyUnknown: break;
            }

#endif

            return false;
        }

        bool SDL2Window::GetMouseButtonState(MouseButton button)
        {
            return buttons & SDL_BUTTON((int)button);
        }

        Vector2i SDL2Window::GetMousePosition()
        {
            int x, y;
            SDL_GetMouseState(&x, &y);
            return Vector2i(x, y);
        }

        void SDL2Window::SetMousePosition(Vector2i position)
        {
            SDL_WarpMouseInWindow(window, position.X, position.Y);
        }

        SystemCursor SDL2Window::GetSystemCursor()
        {
            return SystemCursorNone;
        }

        void SDL2Window::SetSystemCursor(SystemCursor cursor)
        {
#ifndef IOS



#endif
        }

        void SDL2Window::BeginTextInput(TextInputHint hint)
        {
            SDL_StartTextInput();
            if (GlobalWindow && GlobalWindow->GetEventHandler())
            {
                GlobalWindow->GetEventHandler()->OnKeyboardResized(GlobalWindow);
            }
        }

        void SDL2Window::EndTextInput()
        {
            SDL_StopTextInput();
            if (GlobalWindow && GlobalWindow->GetEventHandler())
            {
                GlobalWindow->GetEventHandler()->OnKeyboardResized(GlobalWindow);
            }
        }

        bool SDL2Window::IsTextInputActive()
        {
            return SDL_IsTextInputActive();
        }

        bool SDL2Window::HasOnscreenKeyboardSupport()
        {
            return SDL_HasScreenKeyboardSupport();
        }

        bool SDL2Window::IsOnscreenKeyboardVisible()
        {
            return SDL_IsScreenKeyboardShown(window);
        }

#ifndef IOS

        Vector2i SDL2Window::GetStatusBarSize()
        {
            return Vector2i(0,0);
        }

        Vector2i SDL2Window::GetStatusBarPosition()
        {
            return Vector2i(0,0);
        }

        bool SDL2Window::IsStatusBarVisible()
        {
            return !this->fullscreen;
        }

        void SDL2Window::SetOnscreenKeyboardPosition(Vector2i position)
        {
        }

        Vector2i SDL2Window::GetOnscreenKeyboardPosition()
        {
            return Vector2i(0,0);
        }

        Vector2i SDL2Window::GetOnscreenKeyboardSize()
        {
            return Vector2i(0,0);
        }

#endif
    }

    void InitWindow()
    {
        SDL_VideoInit(0);
    }

    void TerminateWindow()
    {
        SDL_VideoQuit();
    }

    Window* Window::Create(int width, int height, const String& title, int flags)
    {
        PlatformLib::Init();
        return new PlatformSpecific::SDL2Window(width, height, title, flags);
    }

    Window* Window::CreateFrom(void* nativeWindowHandle)
    {
        PlatformLib::Init();
        return new PlatformSpecific::SDL2Window(nativeWindowHandle);
    }

    void Window::SetMainWindow(Window* wnd)
    {
        GlobalWindow = wnd && wnd->GetImplementation() == WindowImplementationSDL2 ? (PlatformSpecific::SDL2Window*)wnd : NULL;
    }

    Window* Window::GetMainWindow()
    {
        return GlobalWindow;
    }

    Vector2i Window::GetScreenSize()
    {
        SDL_DisplayMode mode;
        SDL_GetDesktopDisplayMode(0, &mode);
        return Vector2i(mode.w, mode.h);
    }

    static Xli::Key SDLKeyToXliKey(const SDL_Keysym& key)
    {
        switch (key.sym)
        {
        case SDLK_BACKSPACE: return KeyBackspace;
        case SDLK_TAB: return KeyTab;
        case SDLK_RETURN:
        case SDLK_RETURN2: return KeyEnter;
        case SDLK_LSHIFT:
        case SDLK_RSHIFT: return KeyShift;
        case SDLK_LCTRL:
        case SDLK_RCTRL: return KeyCtrl;
        case SDLK_LALT:
        case SDLK_RALT: return KeyAlt;
        case SDLK_PAUSE: return KeyBreak;
        case SDLK_CAPSLOCK: return KeyCapsLock;
        case SDLK_ESCAPE: return KeyEscape;
        case SDLK_SPACE: return KeySpace;
        case SDLK_PAGEUP: return KeyPageUp;
        case SDLK_PAGEDOWN: return KeyPageDown;
        case SDLK_END: return KeyEnd;
        case SDLK_HOME: return KeyHome;
        case SDLK_LEFT: return KeyLeft;
        case SDLK_UP: return KeyUp;
        case SDLK_RIGHT: return KeyRight;
        case SDLK_DOWN: return KeyDown;
        case SDLK_INSERT: return KeyInsert;
        case SDLK_DELETE: return KeyDelete;
        case SDLK_0: return Key0;
        case SDLK_1: return Key1;
        case SDLK_2: return Key2;
        case SDLK_3: return Key3;
        case SDLK_4: return Key4;
        case SDLK_5: return Key5;
        case SDLK_6: return Key6;
        case SDLK_7: return Key7;
        case SDLK_8: return Key8;
        case SDLK_9: return Key9;
        case SDLK_a: return KeyA;
        case SDLK_b: return KeyB;
        case SDLK_c: return KeyC;
        case SDLK_d: return KeyD;
        case SDLK_e: return KeyE;
        case SDLK_f: return KeyF;
        case SDLK_g: return KeyG;
        case SDLK_h: return KeyH;
        case SDLK_i: return KeyI;
        case SDLK_j: return KeyJ;
        case SDLK_k: return KeyK;
        case SDLK_l: return KeyL;
        case SDLK_m: return KeyM;
        case SDLK_n: return KeyN;
        case SDLK_o: return KeyO;
        case SDLK_p: return KeyP;
        case SDLK_q: return KeyQ;
        case SDLK_r: return KeyR;
        case SDLK_s: return KeyS;
        case SDLK_t: return KeyT;
        case SDLK_u: return KeyU;
        case SDLK_v: return KeyV;
        case SDLK_w: return KeyW;
        case SDLK_x: return KeyX;
        case SDLK_y: return KeyY;
        case SDLK_z: return KeyZ;
        case SDLK_KP_0: return KeyNumPad0;
        case SDLK_KP_1: return KeyNumPad1;
        case SDLK_KP_2: return KeyNumPad2;
        case SDLK_KP_3: return KeyNumPad3;
        case SDLK_KP_4: return KeyNumPad4;
        case SDLK_KP_5: return KeyNumPad5;
        case SDLK_KP_6: return KeyNumPad6;
        case SDLK_KP_7: return KeyNumPad7;
        case SDLK_KP_8: return KeyNumPad8;
        case SDLK_KP_9: return KeyNumPad9;
        case SDLK_F1: return KeyF1;
        case SDLK_F2: return KeyF2;
        case SDLK_F3: return KeyF3;
        case SDLK_F4: return KeyF4;
        case SDLK_F5: return KeyF5;
        case SDLK_F6: return KeyF6;
        case SDLK_F7: return KeyF7;
        case SDLK_F8: return KeyF8;
        case SDLK_F9: return KeyF9;
        case SDLK_F10: return KeyF10;
        case SDLK_F11: return KeyF11;
        case SDLK_F12: return KeyF12;

        case SDLK_LGUI:
        case SDLK_RGUI:
        case SDLK_MENU: return KeyMenu;
        }

        //Error->WriteLine("SDL WARNING: Unknown key: " + CharString::HexFromInt((int)key.sym));
        return KeyUnknown;
    }

    void Window::ProcessMessages()
    {
        SDL_PumpEvents();

        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            //Error->WriteLine("SDL EVENT (" + String::HexFromInt((int)e.type) + ")");

            switch (e.type)
            {
                case SDL_FINGERDOWN:
                case SDL_FINGERMOTION:
                case SDL_FINGERUP:
#ifdef IOS
                    if (GlobalWindow != 0 && GlobalWindow->GetEventHandler() != 0)
                    {
                        int w, h;
                        SDL_GetWindowSize(GlobalWindow->window, &w, &h);

                        float x = e.tfinger.x * w;
                        float y = e.tfinger.y * h;
                        int id = (int)e.tfinger.fingerId;

                        //Error->WriteLine(String::HexFromInt((int)e.type) + " " + (String)x + " " + y + " " + (String)(int)e.tfinger.fingerId);

                        switch (e.type)
                        {
                            case SDL_FINGERDOWN:
                                GlobalWindow->GetEventHandler()->OnTouchDown(GlobalWindow, Vector2(x, y), id);
                                break;

                            case SDL_FINGERMOTION:
                                GlobalWindow->GetEventHandler()->OnTouchMove(GlobalWindow, Vector2(x, y), id);
                                break;

                            case SDL_FINGERUP:
                                GlobalWindow->GetEventHandler()->OnTouchUp(GlobalWindow, Vector2(x, y), id);
                                break;
                        }
                    }
#endif

                    continue;

                case SDL_MULTIGESTURE:
                    continue;

                case SDL_QUIT:
                    if (CancelCount > 0)
                    {
                        CancelCount--;
                        continue;
                    }

                    QuitRecieved = true;
                    //exit(1);
                    continue;
            }

            PlatformSpecific::SDL2Window* wnd = 0;
            SDL_Window* sdlwnd = SDL_GetWindowFromID(e.window.windowID);
            if (sdlwnd != 0)
                wnd = (PlatformSpecific::SDL2Window*)SDL_GetWindowData(sdlwnd, "SDL2Window");

            if (wnd == 0)
            {
                //Error->WriteLine("SDL WARNING: wnd pointer was NULL in Window::ProcessMessages (" + String::HexFromInt((int)e.type) + ")");
                continue;
            }

            if (wnd->closed)
                continue;

            switch (e.type)
            {
                case SDL_WINDOWEVENT:
                    switch (e.window.event)
                    {
                        case SDL_WINDOWEVENT_CLOSE:
                            if (!wnd->closed)
                            {
                                if (wnd->eventHandler &&
                                    wnd->eventHandler->OnClosing(wnd))
                                {
                                    CancelCount++;
                                    continue;
                                }

                                SDL_HideWindow(wnd->window);
                                wnd->closed = true;
                                wnd->eventHandler->OnClosed(wnd);
                            }
                            break;

                        case SDL_WINDOWEVENT_RESIZED:
                        case SDL_WINDOWEVENT_SIZE_CHANGED:
                            if (wnd->GetEventHandler() != 0)
                                wnd->GetEventHandler()->OnSizeChanged(wnd);

                            break;

                        case SDL_WINDOWEVENT_HIDDEN:
                        case SDL_WINDOWEVENT_MINIMIZED:
                            break;

                        case SDL_WINDOWEVENT_SHOWN:
                        case SDL_WINDOWEVENT_MAXIMIZED:
                        case SDL_WINDOWEVENT_RESTORED:
                            break;
                    }
                    break;

                case SDL_KEYDOWN:
                    if (wnd->GetEventHandler() != 0)
                    {
                        Key key = SDLKeyToXliKey(e.key.keysym);

                        if (key)
                            wnd->GetEventHandler()->OnKeyDown(wnd, key);
                        else
                            Error->WriteLine("SDL_KEYDOWN: " + (String)*(int*)&e.key.keysym);
                    }
                    break;

                case SDL_KEYUP:
                    if (wnd->GetEventHandler() != 0)
                    {
                        Key key = SDLKeyToXliKey(e.key.keysym);

                        if (key)
                            wnd->GetEventHandler()->OnKeyUp(wnd, key);
                        else
                            Error->WriteLine("SDL_KEYUP: " + (String)*(int*)&e.key.keysym);
                    }
                    break;

                case SDL_TEXTINPUT:
                    if (wnd->GetEventHandler())
                        wnd->GetEventHandler()->OnTextInput(wnd, e.text.text);

                    break;

                case SDL_TEXTEDITING:
                    if (wnd->GetEventHandler())
                        Error->WriteLine("SDL_TEXTEDITING: " + (String)e.edit.text + " " + e.edit.start + " " + e.edit.length);

                    break;

#ifndef IOS

                case SDL_MOUSEBUTTONDOWN:
                    wnd->buttons |= SDL_BUTTON(e.button.button);

                    if (wnd->GetEventHandler() != 0)
                        wnd->GetEventHandler()->OnMouseDown(wnd, Vector2i(e.button.x, e.button.y), (MouseButton)e.button.button); // TODO: Right/middle are swapped

                    break;

                case SDL_MOUSEBUTTONUP:
                    wnd->buttons &= ~SDL_BUTTON(e.button.button);

                    if (wnd->GetEventHandler() != 0)
                        wnd->GetEventHandler()->OnMouseUp(wnd, Vector2i(e.button.x, e.button.y), (MouseButton)e.button.button);

                    break;

                case SDL_MOUSEMOTION:
                    if (wnd->GetEventHandler() != 0)
                        wnd->GetEventHandler()->OnMouseMove(wnd, Vector2i(e.button.x, e.button.y));

                    break;

                case SDL_MOUSEWHEEL:
                    if (wnd->GetEventHandler() != 0)
                        wnd->GetEventHandler()->OnMouseWheel(wnd, Vector2i(e.wheel.x, e.wheel.y));

                    break;
#endif
            }
        }
    }
}
