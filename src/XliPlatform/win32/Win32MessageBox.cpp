#include <XliPlatform/MessageBox.h>
#include <XliPlatform/Window.h>
#include <XliPlatform/PlatformSpecific/Win32.h>
#include <uBase/Unicode.h>

namespace Xli
{
    DialogResult MessageBox::Show(Window* wnd, const String& message, const String& caption, DialogButtons buttons, int hints)
    {
        UINT type = 0;

        switch (buttons)
        {
        case DialogButtonsOK: type |= MB_OK; break;
        case DialogButtonsOKCancel: type |= MB_OKCANCEL; break;
        case DialogButtonsYesNo: type |= MB_YESNO; break;
        case DialogButtonsYesNoCancel: type |= MB_YESNOCANCEL; break;
        case DialogButtonsCancelTryContinue: type |= MB_CANCELTRYCONTINUE; break;
        default:
            throw Exception("Invalid button combination for MessageDialog");
        }

        if (hints & DialogHintsError) type |= MB_ICONERROR;
        else if (hints & DialogHintsInformation) type |= MB_ICONINFORMATION;
        else if (hints & DialogHintsQuestion) type |= MB_ICONQUESTION;
        else if (hints & DialogHintsWarning) type |= MB_ICONWARNING;

        if (hints & DialogHintsButton1Default) type |= MB_DEFBUTTON1;
        else if (hints & DialogHintsButton2Default) type |= MB_DEFBUTTON2;
        else if (hints & DialogHintsButton3Default) type |= MB_DEFBUTTON3;

        StringU16 messageW = Unicode::Utf8To16(message);
        StringU16 captionW = Unicode::Utf8To16(caption);
        int result = MessageBoxW(PlatformSpecific::Win32::GetWindowHandle(wnd), messageW.Ptr(), captionW.Ptr(), type);

        switch (result)
        {
        case IDOK: return DialogResultOK;
        case IDCANCEL: return DialogResultCancel;
        case IDCONTINUE: return DialogResultContinue;
        case IDIGNORE: return DialogResultContinue;
        case IDNO: return DialogResultNo;
        case IDTRYAGAIN: return DialogResultTryAgain;
        case IDYES: return DialogResultYes;
        default:
            throw Exception("MessageDialog failed: " + PlatformSpecific::Win32::GetLastErrorString());
        }
    }
}
