#pragma once
#include <uBase/Vector2.h>
#include <uBase/Exception.h>

namespace uImage
{
    /** \ingroup uValue
        Represents the coordinates of a rectangle.
        The rectangle is represented in a Left, Top, Right, Bottom-notation, where the X-axis increases
        in the right direction, where the Y-axis increases in the down direction.
        The width of the rectangle is defined to be (Right-Left), and the height is (Bottom-Top). Note that
        the elements at column Right and row Bottom is not included in the rectangle. */
    template <typename T> class Rectanglet
    {
    public:
        union
        {
            struct
            {
                // The lowest X-coordinate of the rectangle
                T Left;

                // The lowest Y-coordinate of the rectangle
                T Top;

                // One plus the highest X-coordinate of the interior of the rectangle
                T Right;

                // One plus the highest Y-coordinate of the interior of the rectangle
                T Bottom;
            };

            T Corners[4];
        };

        operator T*()
        {
            return Corners;
        }

        T& operator [](int i)
        {
#ifdef DEBUG
            if (i >= 4 || i < 0)
                throw uBase::IndexOutOfRangeException();
#endif
            return Corners[i];
        }

        uBase::String ToString() const
        {
            return uBase::String(Left) + ", " + Top + ", " + Right + ", " + Bottom;
        }

        Rectanglet()
        {
        }

        Rectanglet(T left, T top, T right, T bottom)
        {
            Left = left;
            Top = top;
            Right = right;
            Bottom = bottom;
        }

        Rectanglet(const Rectanglet& v)
        {
            Left = v.Left;
            Top = v.Top;
            Right = v.Right;
            Bottom = v.Bottom;
        }

        template <typename U> explicit Rectanglet(const Rectanglet<U>& v)
        {
            Left = (T)v.Left;
            Top = (T)v.Top;
            Right = (T)v.Right;
            Bottom = (T)v.Bottom;
        }

        Rectanglet& operator =(const Rectanglet& v)
        {
            Left = v.Left;
            Top = v.Top;
            Right = v.Right;
            Bottom = v.Bottom;
            return *this;
        }

        Rectanglet(const uBase::Vector2t<T>& position, const uBase::Vector2t<T>& size)
        {
            Left = position.X;
            Top = position.Y;
            Right = Left + size.X;
            Bottom = Top + size.Y;
        }

        /** Returns the i'th corner of the rectangle.
            The corners are enumerated as follows: 0=upper left, 1=lower left, 2=lower right, 3=upper right.
            If the index is out of range, an IndexOutOfRangeException is thrown. */
        uBase::Vector2t<T> GetCorner(int i)
        {
            switch (i)
            {
            case 0: return uBase::Vector2t<T>(Left, Top);
            case 1: return uBase::Vector2t<T>(Left, Bottom);
            case 2: return uBase::Vector2t<T>(Right, Bottom);
            case 3: return uBase::Vector2t<T>(Right, Top);
            }

            throw uBase::IndexOutOfRangeException();
        }

        T Width() const
        {
            return Right - Left;
        }

        T Height() const
        {
            return Bottom - Top;
        }

        uBase::Vector2t<T> Position() const
        {
            return uBase::Vector2t<T>(Left, Top);
        }

        uBase::Vector2t<T> Size() const
        {
            return uBase::Vector2t<T>(Width(), Height());
        }

        uBase::Vector2t<T> TopLeft() const
        {
            return uBase::Vector2t<T>(Left, Top);
        }

        uBase::Vector2t<T> TopRight() const
        {
            return uBase::Vector2t<T>(Right, Top);
        }

        uBase::Vector2t<T> BottomLeft() const
        {
            return uBase::Vector2t<T>(Left, Bottom);
        }

        uBase::Vector2t<T> BottomRight() const
        {
            return uBase::Vector2t<T>(Right, Bottom);
        }

        uBase::Vector2t<T> Center() const
        {
            return uBase::Vector2t<T>(Left + Width() / 2, Top + Height() / 2);
        }

        T Area() const
        {
            return Width() * Height();
        }

        void Move(const uBase::Vector2t<T>& dp)
        {
            Move(dp.X, dp.Y);
        }

        void Move(const T& dX, const T& dY)
        {
            Left += dX;
            Top += dY;
            Right += dX;
            Bottom += dY;
        }

        void MoveTo(const uBase::Vector2t<T>& dp)
        {
            MoveTo(dp.X, dp.Y);
        }

        void MoveTo(const T& x, const T& y)
        {
            Move(x - Left, y - Top);
        }

        void Resize(const T& width, const T& height)
        {
            Right = Left + width;
            Bottom = Top + height;
        }

        bool Intersects(const T& x, const T& y) const
        {
            if (x < Left) return false;
            if (x > Right) return false;
            if (y < Top) return false;
            if (y > Bottom) return false;
            return true;
        }

        bool Intersects(const Rectanglet& rect) const
        {
            return (! (Left > rect.Right
                    || Right < rect.Left
                    || Top > rect.Bottom
                    || Bottom < rect.Top));
        }

        /** Return this rectangle clipped against a scissor rectangle. */
        Rectanglet Clip(const Rectanglet& scissor) const
        {
            Rectanglet r;
            r.Left = Max(Left, scissor.Left);
            r.Right = Min(Right, scissor.Right);
            r.Top = Max(Top, scissor.Top);
            r.Bottom = Min(Bottom, scissor.Bottom);
            return r;
        }

        Rectanglet operator +(const uBase::Vector2t<T>& offset) const
        {
            Rectanglet r;
            r.Left = Left + offset.X;
            r.Right = Right + offset.X;
            r.Top = Top + offset.Y;
            r.Bottom = Bottom + offset.Y;
            return r;
        }

        Rectanglet operator -(const uBase::Vector2t<T>& offset) const
        {
            return *this + (-offset);
        }
    };
    /** \addtogroup uValue */
    /** @{ */
    typedef Rectanglet<float> Rect;
    typedef Rectanglet<double> Rectd;
    typedef Rectanglet<int> Recti;
    /** @} */
}
