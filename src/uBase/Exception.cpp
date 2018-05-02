#include <uBase/Exception.h>

namespace uBase
{
    Exception::Exception()
    {
        _func = "<unknown>";
        _line = 0;
    }

    Exception::Exception(const String& message)
    {
        _message = message;
        _func = "<unknown>";
        _line = 0;
    }

    Exception::Exception(const String& message, const char* func, int line)
    {
        _message = message;
        _func = func;
        _line = line;
    }

    Exception::~Exception() throw()
    {
        // empty
    }

    const String& Exception::GetMessage() const throw()
    {
        return _message;
    }

    const char* Exception::GetFunction() const throw()
    {
        return _func;
    }

    int Exception::GetLine() const throw()
    {
        return _line;
    }

    const char* Exception::what() const throw()
    {
        return "uBase::Exception";
    }
}
