#pragma once
#include <uBase/Stream.h>
#include <uBase/Vector4.h>

namespace uBase
{
    /** \ingroup uIO */
    class BinaryWriter : public StreamWriter
    {
    public:
        BinaryWriter(Stream* stream);
        virtual ~BinaryWriter();

        template <typename T>
        void Write(const T& elm)
        {
            _stream->Write((const void*)&elm, sizeof(T), 1);
        }

        template <typename T>
        void Write(const Vector2t<T>& vec) { _stream->Write(vec.Ptr(), sizeof(T), 2); }
        template <typename T>
        void Write(const Vector3t<T>& vec) { _stream->Write(vec.Ptr(), sizeof(T), 3); }
        template <typename T>
        void Write(const Vector4t<T>& vec) { _stream->Write(vec.Ptr(), sizeof(T), 4); }

        void WriteFloat(const float& elm) { Write<float>(elm); }
        void WriteDouble(const double& elm) { Write<double>(elm); }
        void WriteInt64(const int64_t& elm) { Write<int64_t>(elm); }
        void WriteInt32(const int32_t& elm) { Write<int32_t>(elm); }
        void WriteInt16(const int16_t& elm) { Write<int16_t>(elm); }
        void WriteInt8(const int8_t& elm) { Write<int8_t>(elm); }
        void WriteUInt64(const uint64_t& elm) { Write<uint64_t>(elm); }
        void WriteUInt32(const uint32_t& elm) { Write<uint32_t>(elm); }
        void WriteUInt16(const uint16_t& elm) { Write<uint16_t>(elm); }
        void WriteUInt8(const uint8_t& elm) { Write<uint8_t>(elm); }
        void WriteBool(const bool& elm) { Write<uint8_t>(elm? 1: 0); }

        void Write(const void* data, int elmSize, int elmCount);
        void WriteCStr(const char* str);

        void Write7BitEncodedInt32(int32_t value);

        /// .NET compatible string writer
        void WriteString(const String& str);
    };
}
