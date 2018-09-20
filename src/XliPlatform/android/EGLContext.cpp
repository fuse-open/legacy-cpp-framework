#include <XliPlatform/GLContext.h>
#include <uBase/Console.h>
#include <uBase/Memory.h>
#include <XliPlatform/Window.h>
#include <EGL/egl.h>
#include <uBase/Console.h>
#include <stdlib.h>
#include <string.h>

#ifdef ANDROID
# include <android/native_window.h>
# define NATIVE_HANDLE ANativeWindow*
#endif

namespace Xli
{
    class EglContext: public GLContext
    {
        Shared<Window> window;
        NATIVE_HANDLE handle;
        EGLDisplay display;
        EGLSurface surface;
        EGLContext context;
        EGLConfig config;
        int swapInterval;

    public:
        EglContext(Window* wnd, const GLContextAttributes& attribs)
        {
            swapInterval = -1;
            handle = NULL;
            context = EGL_NO_CONTEXT;
            surface = EGL_NO_SURFACE;

            display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
            eglInitialize(display, 0, 0);

            const EGLint iattribs[] =
            {
                EGL_RED_SIZE, 5,
                EGL_GREEN_SIZE, 6,
                EGL_BLUE_SIZE, 5,
                EGL_ALPHA_SIZE, 0,
                EGL_DEPTH_SIZE, 16,
                EGL_STENCIL_SIZE, 0,
                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                //EGL_RENDER_BUFFER, attribs.Buffers <= 1 ? EGL_SINGLE_BUFFER : EGL_BACK_BUFFER,
                EGL_NONE
            };

            EGLint numConfigs;
            EGLConfig configs[128];

            if (!eglChooseConfig(display, iattribs, configs, 128, &numConfigs) || numConfigs == 0)
                throw Exception("Unable to choose EGL config");

            EGLint cs = 0, cd = 0, cb = 0;
            int cc = 0;

            for (int i = 0; i < numConfigs; i++)
            {
                EGLint samples, depth, stencil, buffer, r, g, b, a, render;
                eglGetConfigAttrib(display, configs[i], EGL_RED_SIZE, &r);
                eglGetConfigAttrib(display, configs[i], EGL_GREEN_SIZE, &g);
                eglGetConfigAttrib(display, configs[i], EGL_BLUE_SIZE, &b);
                eglGetConfigAttrib(display, configs[i], EGL_ALPHA_SIZE, &a);
                eglGetConfigAttrib(display, configs[i], EGL_BUFFER_SIZE, &buffer);
                eglGetConfigAttrib(display, configs[i], EGL_DEPTH_SIZE, &depth);
                eglGetConfigAttrib(display, configs[i], EGL_STENCIL_SIZE, &stencil);
                eglGetConfigAttrib(display, configs[i], EGL_SAMPLES, &samples);

#ifdef DEBUG
                Error->WriteLine(String::Format("DEBUG: EGLConfig[%d]:  M %d  D %d  S %d  B %d  R %d  G %d  B %d  A %d", i, samples, depth, stencil, buffer, r, g, b, a));
#endif

                if (samples >= cs && depth >= cd && buffer >= cb &&
                    samples <= attribs.Samples && r <= attribs.ColorBits.R && g <= attribs.ColorBits.G && b <= attribs.ColorBits.B && a <= attribs.ColorBits.A)
                {
                    cs = samples;
                    cd = depth;
                    cb = buffer;
                    cc = i;
                }
            }

            config = configs[cc];

#ifdef DEBUG
            Error->WriteLine((String)"DEBUG: Selected EGLConfig[" + (int)cc + "]");
#endif

            MakeCurrent(wnd);
        }

        virtual ~EglContext()
        {
            if (display != EGL_NO_DISPLAY)
            {
                eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

                if (context != EGL_NO_CONTEXT)
                    eglDestroyContext(display, context);

                if (surface != EGL_NO_SURFACE)
                    eglDestroySurface(display, surface);

                eglTerminate(display);
            }
        }

        virtual GLContext* CreateSharedContext()
        {
            throw NotSupportedException(U_FUNCTION);
        }

        virtual void MakeCurrent(Window* wnd)
        {
            if (wnd)
                window = wnd;

            if (wnd && (NATIVE_HANDLE)wnd->GetNativeHandle() != handle)
            {
#ifdef ANDROID
                if (wnd->GetImplementation() == WindowImplementationAndroid)
                {
                    EGLint format;
                    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
                    ANativeWindow_setBuffersGeometry((ANativeWindow*)wnd->GetNativeHandle(), 0, 0, format);
                }
#endif

                if (surface != EGL_NO_SURFACE)
                    eglDestroySurface(display, surface);

                surface = eglCreateWindowSurface(display, config, (NATIVE_HANDLE)wnd->GetNativeHandle(), NULL);

                if (surface == EGL_NO_SURFACE)
                    throw Exception("Unable to create EGL Surface");

                if (context == EGL_NO_CONTEXT)
                {
                    const EGLint context_attribs[] =
                    {
                        EGL_CONTEXT_CLIENT_VERSION, 2,
                        EGL_NONE
                    };

                    context = eglCreateContext(display, config, EGL_NO_CONTEXT, context_attribs);

                    if (context == EGL_NO_CONTEXT)
                        throw Exception("Unable to create EGL Context");
                }
            }

            if (eglMakeCurrent(display, surface, surface, wnd != 0 ? context : 0) == EGL_FALSE)
            {
                throw Exception("Unable to make EGL context current");
            }
        }

        virtual bool IsCurrent()
        {
            return eglGetCurrentContext() == context;
        }

        virtual void SwapBuffers()
        {
            eglSwapBuffers(display, surface);
        }

        virtual void SetSwapInterval(int value)
        {
            if (eglSwapInterval(display, value))
                swapInterval = value;
        }

        virtual int GetSwapInterval()
        {
            return swapInterval;
        }

        virtual Vector2i GetDrawableSize()
        {
            return window->GetClientSize();
        }

        virtual void GetAttributes(GLContextAttributes& result)
        {
            memset(&result, 0, sizeof(GLContextAttributes));
            eglGetConfigAttrib(display, config, EGL_RED_SIZE, &result.ColorBits.R);
            eglGetConfigAttrib(display, config, EGL_GREEN_SIZE, &result.ColorBits.G);
            eglGetConfigAttrib(display, config, EGL_BLUE_SIZE, &result.ColorBits.B);
            eglGetConfigAttrib(display, config, EGL_ALPHA_SIZE, &result.ColorBits.A);
            eglGetConfigAttrib(display, config, EGL_DEPTH_SIZE, &result.DepthBits);
            eglGetConfigAttrib(display, config, EGL_STENCIL_SIZE, &result.StencilBits);
            eglGetConfigAttrib(display, config, EGL_SAMPLES, &result.Samples);
/*
            EGLint renderBuffer;
            eglGetConfigAttrib(display, config, EGL_RENDER_BUFFER, &renderBuffer);
            result.Buffers = renderBuffer == EGL_SINGLE_BUFFER ? 1 : 2;
*/
            result.Buffers = 2;
        }
    };

    GLContext* GLContext::Create(Window* window, const GLContextAttributes& attribs)
    {
        return new EglContext(window, attribs);
    }
}
