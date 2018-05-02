#include <uBase/BinaryWriter.h>
#include <cstring>

namespace uBase
{
    BinaryWriter::BinaryWriter(Stream* stream)
        : StreamWriter(stream)
    {
    }

    BinaryWriter::~BinaryWriter()
    {
    }

    void BinaryWriter::Write(const void* data, int elmSize, int elmCount)
    {
        _stream->Write(data, elmSize, elmCount);
    }

    void BinaryWriter::WriteCStr(const char* str)
    {
        _stream->Write((const void*)str, 1, (int)strlen(str));
    }

    void BinaryWriter::Write7BitEncodedInt32(int32_t value)
    {
        while (value >= 0x80)
        {
            WriteUInt8((value | 0x80));
            value >>= 7;
        }

        WriteUInt8(value);
    }

    void BinaryWriter::WriteString(const String& str)
    {
        Write7BitEncodedInt32(str.Length());
        _stream->Write(str.Ptr(), 1, str.Length());
    }
}
