#include <XliPlatform/Window.h>
#include <uBase/Thread.h>
#include <uBase/StringBuilder.h>
#include <cstring>

namespace Xli
{
    bool WindowEventHandler::OnKeyDown(Window* wnd, Key key)
    {
        return false;
    }

    bool WindowEventHandler::OnKeyUp(Window* wnd, Key key)
    {
        return false;
    }

    bool WindowEventHandler::OnTextInput(Window* wnd, const String& text)
    {
        return false;
    }

    bool WindowEventHandler::OnKeyboardResized(Window* wnd)
    {
        return false;
    }

    bool WindowEventHandler::OnMouseDown(Window* wnd, Vector2i pos, MouseButton button)
    {
        return false;
    }

    bool WindowEventHandler::OnMouseUp(Window* wnd, Vector2i pos, MouseButton button)
    {
        return false;
    }

    bool WindowEventHandler::OnMouseMove(Window* wnd, Vector2i pos)
    {
        return false;
    }

    bool WindowEventHandler::OnMouseWheel(Window* wnd, Vector2i delta)
    {
        return false;
    }

    bool WindowEventHandler::OnTouchDown(Window* wnd, Vector2 pos, int id)
    {
        return false;
    }

    bool WindowEventHandler::OnTouchMove(Window* wnd, Vector2 pos, int id)
    {
        return false;
    }

    bool WindowEventHandler::OnTouchUp(Window* wnd, Vector2 pos, int id)
    {
        return false;
    }

    void WindowEventHandler::OnNativeHandleChanged(Window* wnd)
    {
    }

    void WindowEventHandler::OnSizeChanged(Window* wnd)
    {
    }

    bool WindowEventHandler::OnClosing(Window* wnd)
    {
        return false;
    }

    void WindowEventHandler::OnClosed(Window* wnd)
    {
    }

    void WindowEventHandler::OnAppLowMemory(Window* wnd)
    {
    }

    void WindowEventHandler::OnAppTerminating(Window* wnd)
    {
    }

    void WindowEventHandler::OnAppWillEnterForeground(Window* wnd)
    {
    }

    void WindowEventHandler::OnAppDidEnterForeground(Window* wnd)
    {
    }

    void WindowEventHandler::OnAppWillEnterBackground(Window* wnd)
    {
    }

    void WindowEventHandler::OnAppDidEnterBackground(Window* wnd)
    {
    }
}
