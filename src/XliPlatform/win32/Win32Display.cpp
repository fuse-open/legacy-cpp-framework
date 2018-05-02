#include <XliPlatform/Display.h>
#include <XliPlatform/PlatformLib.h>
#include <XliPlatform/PlatformSpecific/Win32.h>
#include <uBase/Array.h>
#include <cstdlib>

namespace Xli
{
    static Array<HMONITOR>* Monitors = 0;

    static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
    {
        ((Array<HMONITOR>*)dwData)->Add(hMonitor);
        return TRUE;
    }

    void InitDisplay()
    {
        Monitors = new Array<HMONITOR>();
        EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)Monitors);
    }

    void TerminateDisplay()
    {
        delete Monitors;
    }

    int Display::GetCount()
    {
        PlatformLib::Init();
        return Monitors->Length();
    }

    Recti Display::GetRect(int index)
    {
        PlatformLib::Init();
        HMONITOR hMonitor = Monitors->Get(index);

        MONITORINFO info;
        info.cbSize = sizeof(MONITORINFO);
        GetMonitorInfo(hMonitor, &info);

        return Recti(info.rcMonitor.left, info.rcMonitor.top, info.rcMonitor.right, info.rcMonitor.bottom);
    }

    bool Display::GetCurrentSettings(int index, DisplaySettings& settings)
    {
        PlatformLib::Init();
        HMONITOR hMonitor = Monitors->Get(index);

        MONITORINFOEX info;
        info.cbSize = sizeof(MONITORINFOEX);
        GetMonitorInfo(hMonitor, &info);

        DEVMODE mode;
        mode.dmSize = sizeof(DEVMODE);
        mode.dmDriverExtra = 0;

        if (EnumDisplaySettings(info.szDevice, ENUM_CURRENT_SETTINGS, &mode))
        {
            settings.Resolution.X = mode.dmPelsWidth;
            settings.Resolution.Y = mode.dmPelsHeight;
            settings.RefreshRate = mode.dmDisplayFrequency;
            settings.BitsPerPixel = mode.dmBitsPerPel;
            return true;
        }

        return false;
    }

    void Display::GetSupportedSettings(int index, Array<DisplaySettings>& settings)
    {
        PlatformLib::Init();
        HMONITOR hMonitor = Monitors->Get(index);

        MONITORINFOEX info;
        info.cbSize = sizeof(MONITORINFOEX);
        GetMonitorInfo(hMonitor, &info);

        DEVMODE mode;
        mode.dmSize = sizeof(DEVMODE);
        mode.dmDriverExtra = 0;

        for (int i = 0; EnumDisplaySettings(info.szDevice, i, &mode); i++)
        {
            int j = settings.Add();
            settings[j].Resolution.X = mode.dmPelsWidth;
            settings[j].Resolution.Y = mode.dmPelsHeight;
            settings[j].RefreshRate = mode.dmDisplayFrequency;
            settings[j].BitsPerPixel = mode.dmBitsPerPel;
        }
    }

    bool Display::ChangeSettings(int index, const DisplaySettings& settings)
    {
        PlatformLib::Init();
        HMONITOR hMonitor = Monitors->Get(index);

        MONITORINFOEX info;
        info.cbSize = sizeof(MONITORINFOEX);
        GetMonitorInfo(hMonitor, &info);

        DEVMODE mode;
        mode.dmSize = sizeof(DEVMODE);
        mode.dmDriverExtra = 0;
        mode.dmPelsWidth = settings.Resolution.X;
        mode.dmPelsHeight = settings.Resolution.Y;
        mode.dmDisplayFrequency = settings.RefreshRate;
        mode.dmBitsPerPel = settings.BitsPerPixel;
        mode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;

        return ChangeDisplaySettingsEx(info.szDevice, &mode, NULL, CDS_FULLSCREEN, NULL) == DISP_CHANGE_SUCCESSFUL;
    }

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
}
