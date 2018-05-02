#pragma once
#include <uBase/Vector3.h>

namespace uBase
{
    /** \ingroup uValue */
    template <typename T> class Vector4t
    {
    public:
        union
        {
            struct { T X, Y, Z, W; };
            struct { T R, G, B, A; };
            T Data[4];
        };

        String ToString() const
        {
            return String(X) + ", " + Y + ", " + Z + ", " + W;
        }

        Vector2t<T> XY() const
        {
            return Vector2t<T>(X, Y);
        }

        Vector3t<T> XYZ() const
        {
            return Vector3t<T>(X, Y, Z);
        }

        Vector4t(const Vector2t<T>& xy, T z, T w)
        {
            X = xy.X;
            Y = xy.Y;
            Z = z;
            W = w;
        }

        Vector4t(const Vector3t<T>& xyz, T w)
        {
            X = xyz.X;
            Y = xyz.Y;
            Z = xyz.Z;
            W = w;
        }

        Vector4t()
        {
        }

        Vector4t(const T& x, const T& y, const T& z, const T& w)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
        }

        Vector4t(const Vector4t& v)
        {
            X = v.X;
            Y = v.Y;
            Z = v.Z;
            W = v.W;
        }

        template <typename U>
        explicit Vector4t(const Vector4t<U>& v)
        {
            X = (T)v.X;
            Y = (T)v.Y;
            Z = (T)v.Z;
            W = (T)v.W;
        }

        Vector4t& operator =(const Vector4t& v)
        {
            X = v.X;
            Y = v.Y;
            Z = v.Z;
            W = v.W;
            return *this;
        }

        bool operator ==(const Vector4t& v) const
        {
            return (X == v.X) && (Y == v.Y) && (Z == v.Z) && (W == v.W);
        }

        bool operator !=(const Vector4t& v) const
        {
            return !(*this == v);
        }

        Vector4t operator -() const
        {
            Vector4t r;
            r.X = -X;
            r.Y = -Y;
            r.Z = -Z;
            r.W = -W;
            return r;
        }

        Vector4t operator +(const Vector4t& v) const
        {
            Vector4t r;
            r.X = X + v.X;
            r.Y = Y + v.Y;
            r.Z = Z + v.Z;
            r.W = W + v.W;
            return r;
        }

        Vector4t operator -(const Vector4t& v) const
        {
            Vector4t r;
            r.X = X - v.X;
            r.Y = Y - v.Y;
            r.Z = Z - v.Z;
            r.W = W - v.W;
            return r;
        }
        Vector4t operator *(const Vector4t& v) const
        {
            Vector4t r;
            r.X = X * v.X;
            r.Y = Y * v.Y;
            r.Z = Z * v.Z;
            r.W = W * v.W;
            return r;
        }

        Vector4t operator /(const Vector4t& v) const
        {
            Vector4t r;
            r.X = X / v.X;
            r.Y = Y / v.Y;
            r.Z = Z / v.Z;
            r.W = W / v.W;
            return r;
        }

        Vector4t operator +(const T& s) const
        {
            Vector4t r;
            r.X = X + s;
            r.Y = Y + s;
            r.Z = Z + s;
            r.W = W + s;
            return r;
        }

        Vector4t operator -(const T& s) const
        {
            Vector4t r;
            r.X = X - s;
            r.Y = Y - s;
            r.Z = Z - s;
            r.W = W - s;
            return r;
        }

        Vector4t operator *(const T& s) const
        {
            Vector4t r;
            r.X = X * s;
            r.Y = Y * s;
            r.Z = Z * s;
            r.W = W * s;
            return r;
        }

        Vector4t operator /(const T& s) const
        {
            Vector4t r;
            r.X = X / s;
            r.Y = Y / s;
            r.Z = Z / s;
            r.W = W / s;
            return r;
        }

        Vector4t& operator +=(const Vector4t& v)
        {
            X += v.X;
            Y += v.Y;
            Z += v.Z;
            W += v.W;
            return *this;
        }

        Vector4t& operator -=(const Vector4t& v)
        {
            X -= v.X;
            Y -= v.Y;
            Z -= v.Z;
            W -= v.W;
            return *this;
        }

        Vector4t& operator *=(const Vector4t& v)
        {
            X *= v.X;
            Y *= v.Y;
            Z *= v.Z;
            W *= v.W;
            return *this;
        }

        Vector4t& operator /=(const Vector4t& v)
        {
            X /= v.X;
            Y /= v.Y;
            Z /= v.Z;
            W /= v.W;
            return *this;
        }

        Vector4t& operator +=(const T& s)
        {
            X += s;
            Y += s;
            Z += s;
            W += s;
            return *this;
        }

        Vector4t& operator -=(const T& s)
        {
            X -= s;
            Y -= s;
            Z -= s;
            W -= s;
            return *this;
        }

        Vector4t& operator *=(const T& s)
        {
            X *= s;
            Y *= s;
            Z *= s;
            W *= s;
            return *this;
        }

        Vector4t& operator /=(const T& s)
        {
            X /= s;
            Y /= s;
            Z /= s;
            W /= s;
            return *this;
        }
    };

    /** \addtogroup uValue */
    /** @{ */
    typedef Vector4t<float> Vector4;
    typedef Vector4t<double> Vector4d;
    typedef Vector4t<int> Vector4i;
    typedef Vector4t<unsigned int> Vector4u;
    typedef Vector4t<bool> Vector4b;

    typedef Vector4t<int64_t> Vector4i64;
    typedef Vector4t<int32_t> Vector4i32;
    typedef Vector4t<int16_t> Vector4i16;
    typedef Vector4t<int8_t> Vector4i8;
    typedef Vector4t<uint64_t> Vector4u64;
    typedef Vector4t<uint32_t> Vector4u32;
    typedef Vector4t<uint16_t> Vector4u16;
    typedef Vector4t<uint8_t> Vector4u8;
    /** @} */
}
