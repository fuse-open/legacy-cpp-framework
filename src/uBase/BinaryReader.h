#pragma once
#include <uBase/Stream.h>
#include <uBase/Vector4.h>

namespace uBase
{
    /** \ingroup uIO */
    class BinaryReader : public StreamReader
    {
    public:
        BinaryReader(Stream* stream);
        virtual ~BinaryReader();

        template <typename T>
        void Read(T& elm)
        {
            _stream->ReadSafe((void*)&elm, sizeof(T), 1);
        }

        template <typename T>
        void Read(Vector2t<T>& vec) { _stream->ReadSafe(vec.Ptr(), sizeof(T), 2); }
        template <typename T>
        void Read(Vector3t<T>& vec) { _stream->ReadSafe(vec.Ptr(), sizeof(T), 3); }
        template <typename T>
        void Read(Vector4t<T>& vec) { _stream->ReadSafe(vec.Ptr(), sizeof(T), 4); }

        template <typename T>
        T Read()
        {
            T elm;
            Read(elm);
            return elm;
        }

        float ReadFloat() { return Read<float>(); }
        double ReadDouble() { return Read<double>(); }
        int64_t ReadInt64() { return Read<int64_t>(); }
        int32_t ReadInt32() { return Read<int32_t>(); }
        int16_t ReadInt16() { return Read<int16_t>(); }
        int8_t ReadInt8() { return Read<int8_t>(); }
        uint64_t ReadUInt64() { return Read<uint64_t>(); }
        uint32_t ReadUInt32() { return Read<uint32_t>(); }
        uint16_t ReadUInt16() { return Read<uint16_t>(); }
        uint8_t ReadUInt8() { return Read<uint8_t>(); }
        bool ReadBool() { return Read<uint8_t>() != 0; }

        template <typename T>
        Vector2t<T> ReadVector2t() { Vector2t<T> vec; Read(vec); return vec; }
        template <typename T>
        Vector3t<T> ReadVector3t() { Vector3t<T> vec; Read(vec); return vec; }
        template <typename T>
        Vector4t<T> ReadVector4t() { Vector4t<T> vec; Read(vec); return vec; }

        void Read(void* dst, int elmSize, int elmCount);
        String ReadCStr(int len);

        Buffer* ReadAll();

        int32_t Read7BitEncodedInt32();

        /// .NET compatible string reader
        String ReadString();
    };
}
