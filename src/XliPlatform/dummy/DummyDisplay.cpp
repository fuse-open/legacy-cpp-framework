#include <XliPlatform/Display.h>

namespace Xli
{
    int Display::GetCount() { return 0; }
    Recti Display::GetRect(int index) { return Recti(0, 0, 1280, 720); }
    bool Display::GetCurrentSettings(int index, DisplaySettings& settings) { return false; }
    void Display::GetSupportedSettings(int index, Array<DisplaySettings>& settings) {}
    bool Display::ChangeSettings(int index, const DisplaySettings& settings) { return false; }
#if !OSX && !IOS
    float Display::GetDensity(int displayIndex) { return 1; }
    Vector2 Display::GetDpi(int displayIndex) { return Vector2i(96, 96); }
#endif
    void InitDisplay() {}
    void TerminateDisplay() {}
}
