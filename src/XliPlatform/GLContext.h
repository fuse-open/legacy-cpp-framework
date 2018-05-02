#pragma once
#include <uBase/Object.h>
#include <uBase/Vector4.h>

namespace Xli
{
    using namespace uBase;

    class Window;

    /** \ingroup XliPlatform
     */
    class GLContextAttributes
    {
    public:
        Vector4i ColorBits;
        int DepthBits;
        int StencilBits;
        int Samples;
        Vector4i AccumBits;
        int Buffers;
        bool Stereo;

        GLContextAttributes(const Vector4i& colorBits, int depthBits, int stencilBits, int samples, const Vector4i& accumBits, int buffers, bool stereo);

        static const GLContextAttributes& Default();
    };

    /** \ingroup XliPlatform
     */
    class GLContext: public Object
    {
    public:
        static GLContext* Create(Window* window, const GLContextAttributes& attribs);

        virtual GLContext* CreateSharedContext() = 0;

        virtual void GetAttributes(GLContextAttributes& result) = 0;
        virtual Vector2i GetDrawableSize() = 0;

        virtual void MakeCurrent(Window* window) = 0;
        virtual bool IsCurrent() = 0;

        virtual void SwapBuffers() = 0;

        virtual void SetSwapInterval(int value) = 0;
        virtual int GetSwapInterval() = 0;
    };
}
