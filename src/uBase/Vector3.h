#pragma once
#include <uBase/Vector2.h>

namespace uBase
{
    /** \ingroup uValue */
    template <typename T> class Vector3t
    {
    public:
        union
        {
            struct { T X, Y, Z; };
            struct { T R, G, B; };
            T Data[3];
        };

        String ToString() const
        {
            return String(X) + ", " + Y + ", " + Z;
        }

        Vector2t<T> XY() const
        {
            return Vector2t<T>(X, Y);
        }

        Vector3t(const Vector2t<T>& xy, T z)
        {
            X = xy.X;
            Y = xy.Y;
            Z = z;
        }

        Vector3t()
        {
        }

        Vector3t(const T& x, const T& y, const T& z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        Vector3t(const Vector3t& v)
        {
            X = v.X;
            Y = v.Y;
            Z = v.Z;
        }

        template <typename U>
        explicit Vector3t(const Vector3t<U>& v)
        {
            X = (T)v.X;
            Y = (T)v.Y;
            Z = (T)v.Z;
        }

        bool operator ==(const Vector3t& v) const
        {
            return (X == v.X) && (Y == v.Y) && (Z == v.Z);
        }

        bool operator !=(const Vector3t& v) const
        {
            return !(*this == v);
        }

        Vector3t& operator =(const Vector3t& v)
        {
            X = v.X;
            Y = v.Y;
            Z = v.Z;
            return *this;
        }

        Vector3t operator -() const
        {
            Vector3t r;
            r.X = -X;
            r.Y = -Y;
            r.Z = -Z;
            return r;
        }

        Vector3t operator +(const Vector3t& v) const
        {
            Vector3t r;
            r.X = X + v.X;
            r.Y = Y + v.Y;
            r.Z = Z + v.Z;
            return r;
        }

        Vector3t operator -(const Vector3t& v) const
        {
            Vector3t r;
            r.X = X - v.X;
            r.Y = Y - v.Y;
            r.Z = Z - v.Z;
            return r;
        }

        Vector3t operator *(const Vector3t& v) const
        {
            Vector3t r;
            r.X = X * v.X;
            r.Y = Y * v.Y;
            r.Z = Z * v.Z;
            return r;
        }

        Vector3t operator /(const Vector3t& v) const
        {
            Vector3t r;
            r.X = X / v.X;
            r.Y = Y / v.Y;
            r.Z = Z / v.Z;
            return r;
        }

        Vector3t operator +(const T& s) const
        {
            Vector3t r;
            r.X = X + s;
            r.Y = Y + s;
            r.Z = Z + s;
            return r;
        }

        Vector3t operator -(const T& s) const
        {
            Vector3t r;
            r.X = X - s;
            r.Y = Y - s;
            r.Z = Z - s;
            return r;
        }

        Vector3t operator *(const T& s) const
        {
            Vector3t r;
            r.X = X * s;
            r.Y = Y * s;
            r.Z = Z * s;
            return r;
        }

        Vector3t operator /(const T& s) const
        {
            Vector3t r;
            r.X = X / s;
            r.Y = Y / s;
            r.Z = Z / s;
            return r;
        }

        Vector3t& operator +=(const Vector3t& v)
        {
            X += v.X;
            Y += v.Y;
            Z += v.Z;
            return *this;
        }

        Vector3t& operator -=(const Vector3t& v)
        {
            X -= v.X;
            Y -= v.Y;
            Z -= v.Z;
            return *this;
        }

        Vector3t& operator *=(const Vector3t& v)
        {
            X *= v.X;
            Y *= v.Y;
            Z *= v.Z;
            return *this;
        }

        Vector3t& operator /=(const Vector3t& v)
        {
            X /= v.X;
            Y /= v.Y;
            Z /= v.Z;
            return *this;
        }

        Vector3t& operator +=(const T& s)
        {
            X += s;
            Y += s;
            Z += s;
            return *this;
        }

        Vector3t& operator -=(const T& s)
        {
            X -= s;
            Y -= s;
            Z -= s;
            return *this;
        }

        Vector3t& operator *=(const T& s)
        {
            X *= s;
            Y *= s;
            Z *= s;
            return *this;
        }

        Vector3t& operator /=(const T& s)
        {
            X /= s;
            Y /= s;
            Z /= s;
            return *this;
        }
    };

    /** \addtogroup uValue */
    /** @{ */
    typedef Vector3t<float> Vector3;
    typedef Vector3t<double> Vector3d;
    typedef Vector3t<int> Vector3i;
    typedef Vector3t<unsigned int> Vector3u;

    typedef Vector3t<int64_t> Vector3i64;
    typedef Vector3t<int32_t> Vector3i32;
    typedef Vector3t<int16_t> Vector3i16;
    typedef Vector3t<int8_t> Vector3i8;
    typedef Vector3t<uint64_t> Vector3u64;
    typedef Vector3t<uint32_t> Vector3u32;
    typedef Vector3t<uint16_t> Vector3u16;
    typedef Vector3t<uint8_t> Vector3u8;
    /** @} */
}
