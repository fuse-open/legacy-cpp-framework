#include <uBase/StringU16.h>
#include <uBase/Exception.h>
#include <cstring>

namespace uBase
{
    int Utf16StrLen(const Utf16* str)
    {
        int len = 0;

        while (*str++)
            len++;

        return len;
    }

    StringU16::StringU16()
    {
        _data = 0;
        _length = 0;
    }

    StringU16::StringU16(const StringU16& copy)
    {
        _length = copy._length;
        _data = new Utf16[copy._length + 1];
        _data[copy._length] = 0;
        memcpy(_data, copy._data, copy._length * 2);
    }

    StringU16& StringU16::operator =(const StringU16& copy)
    {
        if (_data != 0) delete [] _data;
        _length = copy._length;
        _data = new Utf16[copy._length + 1];
        _data[copy._length] = 0;
        memcpy(_data, copy._data, copy._length * 2);
        return *this;
    }

    StringU16::StringU16(const Utf16* str, int len)
    {
        _length = len;
        _data = new Utf16[len + 1];
        _data[len] = 0;
        memcpy(_data, str, len * 2);
    }

    StringU16::StringU16(const Utf16* str)
    {
        _length = Utf16StrLen(str);
        _data = new Utf16[_length + 1];
        _data[_length] = 0;
        memcpy(_data, str, _length * 2);
    }

    StringU16::~StringU16()
    {
        if (_data != 0) delete [] _data;
    }

    StringU16 StringU16::Create(int len)
    {
        StringU16 r;
        r._length = len;
        r._data = new Utf16[len + 1];
        r._data[len] = 0;
        return r;
    }

    int StringU16::Length() const
    {
        return _length;
    }

    Utf16& StringU16::operator[] (int index)
    {
        return _data[index];
    }

    const Utf16& StringU16::operator[] (int index) const
    {
        return _data[index];
    }

    Utf16* StringU16::Ptr()
    {
        return _data;
    }

    const Utf16* StringU16::Ptr() const
    {
        return _data;
    }

    StringU16 StringU16::operator +(const StringU16& str) const
    {
        StringU16 r = StringU16::Create(_length + str._length);
        memcpy(r._data, _data, _length * sizeof(Utf16));
        memcpy(r._data + _length, str._data, str._length * sizeof(Utf16));
        return r;
    }
}
