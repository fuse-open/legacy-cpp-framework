#include <XliPlatform/PlatformSpecific/Android.h>
#include "AJniHelper.h"
#include <XliPlatform/MessageBox.h>
#include <uBase/Console.h>

namespace Xli
{
    DialogResult MessageBox::Show(Window* parent, const String& message, const String& caption, DialogButtons buttons, int hints)
    {
        // Also print to log
        PrintLine("MessageBox [" + caption + "]: " + message);

        PlatformSpecific::AJniHelper jni;

        //setup for call
        jclass activityClass = jni.GetActivityClass();
        static jmethodID showMessageBox = jni->GetStaticMethodID(activityClass, "ShowMessageBox", "(Ljava/lang/CharSequence;Ljava/lang/CharSequence;II)I");

        //vars for call
        jstring jcaption = jni->NewStringUTF(caption.Ptr());
        jstring jmessage = jni->NewStringUTF(message.Ptr());

        //call
        int result = (int)jni->CallStaticIntMethod(activityClass, showMessageBox, jcaption, jmessage, (jint)buttons, (jint)hints);

        return (DialogResult)result;
    }
}
