#pragma once
#include <XliPlatform/Window.h>
#include <SDL2/SDL.h>

namespace Xli
{
    namespace PlatformSpecific
    {
        /** \ingroup XliPlatform */
        class SDL2
        {
        public:
            static SDL_Window* GetWindowHandle(Window* wnd);
        };
    }
}
