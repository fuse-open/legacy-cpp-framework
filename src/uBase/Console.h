#pragma once
#include <uBase/TextReader.h>
#include <uBase/TextWriter.h>

namespace uBase
{
    class StdOutAccessor
    {
    public:
        TextWriter* operator ->();
        operator TextWriter*();
    };

    class StdErrAccessor
    {
    public:
        TextWriter* operator ->();
        operator TextWriter*();
    };

    class StdInAccessor
    {
    public:
        TextReader* operator ->();
        operator TextReader*();
    };

    /** \addtogroup uConsole */
    /** @{ */
    extern StdOutAccessor Out;
    extern StdErrAccessor Error;
    extern StdInAccessor In;

    template <typename T>
    inline void Print(const T& t)
    {
        Out->Write(t);
    }

    template <typename T>
    inline void PrintLine(const T& t)
    {
        Out->WriteLine(t);
    }

    inline void PrintLine()
    {
        Out->WriteLine();
    }
    /** @} */
}
