#pragma once
#include <uBase/Config.h>
#include <stdint.h>

#ifdef ANDROID
# include <stdarg.h>
#else
# include <cstdarg>
#endif

namespace uBase
{
    /** \addtogroup uContainer */
    /** @{ */
    template <typename T> class Array;
    /** @} */

    /** \ingroup uText */
    class U_DLLEXPORT String
    {
        friend class Unicode;
        static const int BufSize = 8;

        char* _data;
        char _buf[BufSize];
        int _length;

        void InitLength(int len);
        void Init(const char* str, int len);
        void Init(int i);
        void Init(float f);
        void Init(double d);
        void Deinit();
        char* TrimLeft(char* str) const;
        void ValidateParsingResult(uBase::String typeName, char* str, char* endptr) const;
        bool IsParsingResultValid(char* str, char* endptr) const;

    public:
        String();
        String(const String& copy);
        String(const char* str);
        String(const char* str, int len);
        String(int i);
        String(float f);
        String(double d);
        ~String();

        char* CopyPtr();
        char* SwapPtr();

        char* Ptr();
        const char* Ptr() const;

        int Length() const;

        char& operator [](int index);
        const char& operator [](int index) const;

        const char& First() const;
        const char& Last() const;

        int IndexOf(char c, int start) const;
        int IndexOf(char c) const;

        int LastIndexOf(char c, int start) const;
        int LastIndexOf(char c) const;

        String Substring(int start, int len) const;
        String Substring(int start) const;

        String Trim() const;
        String Trim(char c) const;

        String ToLower() const;
        String ToUpper() const;

        void Split(char c, Array<String>& parts) const;
        static String Join(char c, const Array<String>& list);

        bool StartsWith(const String& str) const;
        bool EndsWith(const String& str) const;

        static String Create(int length);
        static String HexFromInt(int value);
        static String FromChar(char value);
        static String FromBool(bool value);

        static String Format(const char* format, va_list argList);
        static String Format(const char* format, ...);

        int HexToInt() const;
        int ToInt() const;
        bool TryToInt(int* res) const;
        unsigned long long ToULong() const;
        bool TryToULong(unsigned long long* res) const;
        long long ToLong() const;
        bool TryToLong(long long* res) const;
        float ToFloat() const;
        bool TryToFloat(float* res) const;
        double ToDouble() const;
        bool TryToDouble(double* res) const;

        bool Equals(const char* str, int len) const;
        int CompareTo(const char* str, int len) const;
        String Add(const char* str, int len) const;
        void Append(const char* str, int len);

        bool operator ==(const String& str) const;
        bool operator ==(const char* str) const;

        bool operator !=(const String& str) const;
        bool operator !=(const char* str) const;

        String operator +(const String& str) const;
        String operator +(const char* str) const;

        bool operator <(const String& str) const;
        bool operator <(const char* str) const;

        bool operator <=(const String& str) const;
        bool operator <=(const char* str) const;

        bool operator >(const String& str) const;
        bool operator >(const char* str) const;

        bool operator >=(const String& str) const;
        bool operator >=(const char* str) const;

        String& operator +=(const String& str);
        String& operator +=(const char* str);

        String& operator =(const String& str);
        String& operator =(const char* str);
        String& operator =(int value);
        String& operator =(float value);
        String& operator =(double value);
    };
}

/** \addtogroup uText */
/** @{ */
uBase::String operator +(const char* a, const uBase::String& b);
/** @} */
