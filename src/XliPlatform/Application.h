#pragma once
#include <XliPlatform/Window.h>

namespace Xli
{
    using namespace uBase;

    /** \ingroup XliPlatform */
    class Application: public WindowEventHandler
    {
        int _maxFps;

    protected:
        void SetMaxFps(int value);

    public:
        static void Run(Application* app, int flags = WindowFlagsResizeable);

        Application();

        virtual String GetInitTitle();
        virtual Vector2i GetInitSize();

        virtual void OnInit(Window* wnd);
        virtual void OnLoad(Window* wnd);

        virtual void OnUpdate(Window* wnd);
        virtual void OnDraw(Window* wnd);

        virtual void OnSizeChanged(Window* wnd);
    };
}