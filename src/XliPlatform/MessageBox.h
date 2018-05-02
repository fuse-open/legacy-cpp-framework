#pragma once
#include <uBase/Exception.h>

namespace Xli
{
    using namespace uBase;

    /** \ingroup XliPlatform
        Defines button combinations for dialog boxes. */
    enum DialogButtons
    {
        /** The dialog contains only an OK button */
        DialogButtonsOK = 0,

        /** The dialog contains OK and a Cancel button */
        DialogButtonsOKCancel = 1,

        /** The dialog contains Yes and a No button */
        DialogButtonsYesNo = 2,

        /** The dialog contains Yes, No and a Cancel button */
        DialogButtonsYesNoCancel = 3,

        /** The dialog contains a Cancel, Try Again and a Continue button.
            This can also be displayed as Abort, Retry, Ignore on platforms where
            this is more appropriate. */
        DialogButtonsCancelTryContinue = 4
    };

    /** \ingroup XliPlatform
        Defines button combinations for dialog boxes. */
    enum DialogResult
    {
        /** The OK button was selected. */
        DialogResultOK = 0,

        /** The Cancel button was selected. */
        DialogResultCancel = 1,

        /** The Yes button was selected. */
        DialogResultYes = 2,

        /** The No button was selected. */
        DialogResultNo = 3,

        /** The Try Again (or Retry) button was selected. */
        DialogResultTryAgain = 4,

        /** The Continue (or Ignore) button was selected. */
        DialogResultContinue = 5
    };

    /** \ingroup XliPlatform
        Defines appearance hints for dialog boxes.
        The window manager chooses how to interpret these hints */
    enum DialogHints
    {
        /** The dialog displays an error message. */
        DialogHintsError = (1<<0),

        /** The dialog displays information. */
        DialogHintsInformation = (1<<1),

        /** The dialog displays a warning message. */
        DialogHintsWarning = (1<<2),

        /** The dialog displays a question. */
        DialogHintsQuestion = (1<<3),

        /** The first button is the default choice. */
        DialogHintsButton1Default = (1<<4),

        /** The second button is the default choice. */
        DialogHintsButton2Default = (1<<5),

        /** The third button is the default choice. */
        DialogHintsButton3Default = (1<<6)
    };

    class Window;

    /** \ingroup XliPlatform */
    class MessageBox
    {
    public:
        /** Displays a message box. */
        static DialogResult Show(Window* parent, const String& message, const String& caption, DialogButtons buttons = DialogButtonsOK, int hints = 0);
    };
}