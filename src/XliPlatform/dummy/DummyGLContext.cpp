#include <XliPlatform/GLContext.h>

namespace Xli
{
    namespace PlatformSpecific
    {
        class DummyGLContext: public GLContext
        {
        public:
            virtual GLContext* CreateSharedContext() { return new DummyGLContext(); }
            virtual void GetAttributes(GLContextAttributes& result) {}
            virtual Vector2i GetDrawableSize() { return Vector2i(1280, 720); }
            virtual void MakeCurrent(Window* window) {}
            virtual bool IsCurrent() { return true; }
            virtual void SwapBuffers() {}
            virtual void SetSwapInterval(int value) {}
            virtual int GetSwapInterval() { return 0; }
        };
    }

    GLContext* GLContext::Create(Window* window, const GLContextAttributes& attribs)
    {
        return new PlatformSpecific::DummyGLContext();
    }
}
