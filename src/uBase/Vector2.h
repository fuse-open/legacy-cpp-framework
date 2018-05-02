#pragma once
#include <stdint.h>
#include <uBase/Exception.h>
#include <uBase/String.h>

namespace uBase
{
    /** \ingroup uValue */
    template <typename T> class Vector2t
    {
    public:
        union
        {
            struct { T X, Y; };
            struct { T R, G; };
            T Data[2];
        };

        String ToString() const
        {
            return String(X) + ", " + Y;
        }

        Vector2t()
        {
        }

        Vector2t(const T& x, const T& y)
        {
            X = x;
            Y = y;
        }

        Vector2t(const Vector2t& v)
        {
            X = v.X;
            Y = v.Y;
        }

        template <typename U>
        explicit Vector2t(const Vector2t<U>& v)
        {
            X = (T)v.X;
            Y = (T)v.Y;
        }

        Vector2t& operator =(const Vector2t& v)
        {
            X = v.X;
            Y = v.Y;
            return *this;
        }

        bool operator ==(const Vector2t& v) const
        {
            return X == v.X && Y == v.Y;
        }

        bool operator !=(const Vector2t& v) const
        {
            return !(*this == v);
        }

        Vector2t operator -() const
        {
            Vector2t r;
            r.X = -X;
            r.Y = -Y;
            return r;
        }

        Vector2t operator +(const Vector2t& v) const
        {
            Vector2t r;
            r.X = X + v.X;
            r.Y = Y + v.Y;
            return r;
        }

        Vector2t operator -(const Vector2t& v) const
        {
            Vector2t r;
            r.X = X - v.X;
            r.Y = Y - v.Y;
            return r;
        }

        Vector2t operator *(const Vector2t& v) const
        {
            Vector2t r;
            r.X = X * v.X;
            r.Y = Y * v.Y;
            return r;
        }

        Vector2t operator /(const Vector2t& v) const
        {
            Vector2t r;
            r.X = X / v.X;
            r.Y = Y / v.Y;
            return r;
        }

        Vector2t operator +(const T& s) const
        {
            Vector2t r;
            r.X = X + s;
            r.Y = Y + s;
            return r;
        }

        Vector2t operator -(const T& s) const
        {
            Vector2t r;
            r.X = X - s;
            r.Y = Y - s;
            return r;
        }

        Vector2t operator *(const T& s) const
        {
            Vector2t r;
            r.X = X * s;
            r.Y = Y * s;
            return r;
        }

        Vector2t operator /(const T& s) const
        {
            Vector2t r;
            r.X = X / s;
            r.Y = Y / s;
            return r;
        }

        Vector2t& operator +=(const Vector2t& v)
        {
            X += v.X;
            Y += v.Y;
            return *this;
        }

        Vector2t& operator -=(const Vector2t& v)
        {
            X -= v.X;
            Y -= v.Y;
            return *this;
        }

        Vector2t& operator *=(const Vector2t& v)
        {
            X *= v.X;
            Y *= v.Y;
            return *this;
        }

        Vector2t& operator /=(const Vector2t& v)
        {
            X /= v.X;
            Y /= v.Y;
            return *this;
        }

        Vector2t& operator +=(const T& s)
        {
            X += s;
            Y += s;
            return *this;
        }

        Vector2t& operator -=(const T& s)
        {
            X -= s;
            Y -= s;
            return *this;
        }

        Vector2t& operator *=(const T& s)
        {
            X *= s;
            Y *= s;
            return *this;
        }

        Vector2t& operator /=(const T& s)
        {
            X /= s;
            Y /= s;
            return *this;
        }
    };

    /** \addtogroup uValue */
    /** @{ */
    typedef Vector2t<float> Vector2;
    typedef Vector2t<double> Vector2d;
    typedef Vector2t<int> Vector2i;
    typedef Vector2t<unsigned int> Vector2u;

    typedef Vector2t<int64_t> Vector2i64;
    typedef Vector2t<int32_t> Vector2i32;
    typedef Vector2t<int16_t> Vector2i16;
    typedef Vector2t<int8_t> Vector2i8;
    typedef Vector2t<uint64_t> Vector2u64;
    typedef Vector2t<uint32_t> Vector2u32;
    typedef Vector2t<uint16_t> Vector2u16;
    typedef Vector2t<uint8_t> Vector2u8;
    /** @} */
}
