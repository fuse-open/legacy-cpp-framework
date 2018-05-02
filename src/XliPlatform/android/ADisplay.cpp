#include <android/window.h>
#include <XliPlatform/PlatformSpecific/Android.h>
#include <XliPlatform/Display.h>
#include <XliPlatform/Window.h>

namespace Xli
{
    int Display::GetCount()
    {
        return 1;
    }

    Recti Display::GetRect(int index)
    {
        throw Exception("XLI ANDROID IS DEPRECATED");
        return Recti(0, 0, 0, 0);
    }

    bool Display::GetCurrentSettings(int index, DisplaySettings& settings)
    {
        return false;
    }

    void Display::GetSupportedSettings(int index, Array<DisplaySettings>& settings)
    {
    }

    bool Display::ChangeSettings(int index, const DisplaySettings& settings)
    {
        return false;
    }

    float Display::GetDensity(int displayIndex)
    {
        return 0.0f;
    }
    Vector2 Display::GetDpi(int displayIndex)
    {
        return Vector2(0.0f, 0.0f);
    }
}
