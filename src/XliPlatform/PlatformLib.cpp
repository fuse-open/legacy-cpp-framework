#include <uBase/BaseLib.h>
#include <uBase/StringBuilder.h>
#include <XliPlatform/MessageBox.h>
#include <XliPlatform/PlatformLib.h>
#include <XliPlatform/Window.h>
#include <cstdlib>
#include <cstring>

namespace Xli
{
    void InitWindow();
    void TerminateWindow();
    static bool LibInited;

    static void ExceptionCallback(const Exception& e, const String& where)
    {
        StringBuilder sb(2048);
        sb.Append(e.GetMessage());
        sb.Append("\n\nFunction: ");
        sb.Append(e.GetFunction());
        sb.Append("\nLine: ");
        sb.Append(String(e.GetLine()));

        if (where.Length() > 0)
            sb.Append("\n\n(from: " + where + ")");

        MessageBox::Show(Window::GetMainWindow(), sb.ToString(), "Unhandled Exception", DialogButtonsOK, DialogHintsError);
    }

    static void Terminate()
    {
#if !IOS
        TerminateWindow();
#endif
        LibInited = false;
    }

    void PlatformLib::Init()
    {
        if (BaseLib::EnterCriticalIfFalse(&LibInited))
        {
            BaseLib::SetUnhandledExceptionCallback(ExceptionCallback);
#if !IOS
            InitWindow();
#endif
            BaseLib::ExitCritical();
            atexit(Terminate);
        }
    }
}
