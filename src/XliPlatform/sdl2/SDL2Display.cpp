#include <XliPlatform/Display.h>
#include <uBase/Array.h>
#include <SDL2/SDL.h>

namespace Xli
{
    int Display::GetCount()
    {
        return SDL_GetNumVideoDisplays();
    }

    Recti Display::GetRect(int index)
    {
        SDL_Rect rect;
        SDL_GetDisplayBounds(index, &rect);
        return Recti(Vector2i(rect.x, rect.y), Vector2i(rect.w, rect.h));
    }

    bool Display::GetCurrentSettings(int index, DisplaySettings& settings)
    {
        SDL_DisplayMode mode;
        if (SDL_GetCurrentDisplayMode(index, &mode) == 0)
        {
            settings.Resolution.X = mode.w;
            settings.Resolution.Y = mode.h;
            settings.BitsPerPixel = mode.format;
            settings.RefreshRate = mode.refresh_rate;
            settings.DriverData = mode.driverdata;
            return true;
        }

        return false;
    }

    void Display::GetSupportedSettings(int index, Array<DisplaySettings>& settings)
    {
        SDL_DisplayMode mode;
        for (int i = 0, l = SDL_GetNumDisplayModes(index); i < l; i++)
        {
            int j = settings.Add();
            settings[j].Resolution.X = mode.w;
            settings[j].Resolution.Y = mode.h;
            settings[j].BitsPerPixel = mode.format;
            settings[j].RefreshRate = mode.refresh_rate;
            settings[j].DriverData = mode.driverdata;
        }
    }

    bool Display::ChangeSettings(int index, const DisplaySettings& settings)
    {
        SDL_DisplayMode mode;
        mode.w = settings.Resolution.X;
        mode.h = settings.Resolution.Y;
        mode.format = settings.BitsPerPixel;
        mode.refresh_rate = settings.RefreshRate;
        mode.driverdata = settings.DriverData;
        // TODO
        return false;
    }

#if !OSX && !IOS
    float Display::GetDensity(int displayIndex)
    {
        // TODO
        return 1.0f;
    }

    Vector2 Display::GetDpi(int displayIndex)
    {
        // TODO
        return Vector2(72, 72);
    }
#endif
}
