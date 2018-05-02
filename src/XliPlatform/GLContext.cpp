#include <XliPlatform/GLContext.h>

namespace Xli
{
    GLContextAttributes::GLContextAttributes(const Vector4i& colorBits, int depthBits, int stencilBits, int samples, const Vector4i& accumBits, int buffers, bool stereo)
    {
        ColorBits = colorBits;
        DepthBits = depthBits;
        StencilBits = stencilBits;
        Samples = samples;
        AccumBits = accumBits;
        Buffers = buffers;
        Stereo = stereo;
    }

    const GLContextAttributes& GLContextAttributes::Default()
    {
        static GLContextAttributes def(Vector4i(8, 8, 8, 8), 24, 8, 4, Vector4i(0, 0, 0, 0), 2, false);
        return def;
    }
}
