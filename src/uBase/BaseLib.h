#pragma once
#include <uBase/Exception.h>

namespace uBase
{
    /** \ingroup uBase */
    class BaseLib
    {
    public:
        static void Init();

        static void SetUnhandledExceptionCallback(void(*callback)(const Exception&, const String&));
        static void OnUnhandledException(const Exception& exception, const String& where);

        static bool EnterCriticalIfFalse(bool* flag);
        static void EnterCritical();
        static void ExitCritical();
    };
}
