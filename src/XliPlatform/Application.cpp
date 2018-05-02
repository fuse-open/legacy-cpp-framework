#include <XliPlatform/Application.h>
#include <XliPlatform/Display.h>
#include <XliPlatform/PlatformLib.h>
#include <uBase/Memory.h>
#include <uBase/Thread.h>
#include <uBase/Time.h>

namespace Xli
{
    void Application::Run(Application* app, int flags)
    {
        Auto<Window> wnd = Window::Create(app->GetInitSize(), app->GetInitTitle(), flags);

        app->OnInit(wnd);
        wnd->SetEventHandler(app);
        Window::ProcessMessages();

        app->OnLoad(wnd);

        while (!wnd->IsClosed())
        {
            double startTime = GetSeconds();

            Window::ProcessMessages();
            app->OnUpdate(wnd);

            if (wnd->IsVisible())
                app->OnDraw(wnd);

            if (app->_maxFps > 0)
            {
                double targetTime = 1.0 / (double)app->_maxFps;
                double renderTime = GetSeconds() - startTime;

                int msTimeout = (int)((targetTime - renderTime) * 1000.0 + 0.5);

                if (msTimeout > 0)
                    Sleep(msTimeout);
            }
        }
    }

    Application::Application()
    {
        PlatformLib::Init();

        DisplaySettings settings;
        if (Display::GetCount() > 0 &&
            Display::GetCurrentSettings(0, settings) &&
            settings.RefreshRate > 0)
            _maxFps = settings.RefreshRate;
        else
            _maxFps = 60;
    }

    void Application::SetMaxFps(int value)
    {
        _maxFps = value;
    }

    String Application::GetInitTitle()
    {
        return "Xli Application";
    }

    Vector2i Application::GetInitSize()
    {
#if IOS || ANDROID
        return Window::GetScreenSize();
#else
        return Vector2i(1280, 720);
#endif
    }

    void Application::OnInit(Window* wnd)
    {
    }

    void Application::OnLoad(Window* wnd)
    {
    }

    void Application::OnUpdate(Window* wnd)
    {
    }

    void Application::OnDraw(Window* wnd)
    {
    }

    void Application::OnSizeChanged(Window* wnd)
    {
#if WIN32 || OSX
        if (wnd->GetMouseButtonState(MouseButtonLeft))
        {
            OnUpdate(wnd);
            OnDraw(wnd);
        }
#endif
    }
}
