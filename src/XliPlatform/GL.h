#pragma once
#include <uBase/Config.h>

#if WIN32
# ifndef GLEW_STATIC
#   define GLEW_STATIC
# endif
# include <GL/glew.h>
# include <GL/gl.h>
# define U_GL_DESKTOP

#elif ANDROID
# include <GLES2/gl2.h>
# include <GLES2/gl2ext.h>
# define U_GL_ES2

#elif IOS
# include <OpenGLES/ES2/gl.h>
# include <OpenGLES/ES2/glext.h>
# define U_GL_ES2

#elif OSX
# ifndef GLEW_STATIC
#   define GLEW_STATIC
# endif
# include <GL/glew.h>
# include <OpenGL/gl.h>
# define U_GL_DESKTOP

#elif LINUX
# ifdef __arm__
#  include <GLES2/gl2.h>
#  include <GLES2/gl2ext.h>
#  define U_GL_ES2
# else
#  include <GL/glew.h>
#  include <GL/gl.h>
#  define U_GL_DESKTOP
# endif

#else
#error "Unsupported platform"

#endif
