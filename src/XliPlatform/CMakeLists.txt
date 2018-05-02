file(GLOB SRC *.cpp)
file(GLOB HEADERS *.h)
file(GLOB PLATFORM_HEADERS PlatformSpecific/*.h)
file(COPY ${HEADERS} DESTINATION ${PROJECT_SOURCE_DIR}/include/XliPlatform)
file(COPY ${PLATFORM_HEADERS} DESTINATION ${PROJECT_SOURCE_DIR}/include/XliPlatform/PlatformSpecific)

set(SDL2_SRC
    generic/GenericDialogs.cpp
    sdl2/SDL2Window.cpp
    sdl2/SDL2Display.cpp
    sdl2/SDL2GLContext.cpp
    sdl2/SDL2MessageBox.cpp)

if (ANDROID)

    set(SRC ${SRC}
        android/AJniHelper.cpp
        android/AMessageBox.cpp
        android/ALogStream.cpp
        android/AWindow.cpp
        android/ADisplay.cpp
        android/EGLContext.cpp
        generic/GenericDialogs.cpp)

elseif (WIN32)

    set(SRC ${SRC}
        win32/Win32Dialogs.cpp
        win32/Win32MessageBox.cpp
        win32/Win32Window.cpp
        win32/Win32Display.cpp
        win32/Win32.cpp
        win32/Win32GLContext.cpp)
    add_definitions(-DU_HAS_GLEW)

    if (MSVC)
        set(CMAKE_STATIC_LINKER_FLAGS opengl32.lib)
    endif()

elseif (LINUX)

    add_definitions(`sdl2-config --cflags`)
    set(SRC ${SRC} ${SDL2_SRC})

    if (NOT ARM)
        add_definitions(-DU_HAS_GLEW)
    endif()

elseif (IOS)

    set(SRC ${SRC}
        ios/iOSInit.mm
        ios/iOSDisplay.mm
        dummy/DummyDisplay.cpp
        dummy/DummyGLContext.cpp
        dummy/DummyWindow.cpp
        generic/GenericMessageBox.cpp
        generic/GenericDialogs.cpp)

elseif (OSX)

    add_definitions(-DU_HAS_GLEW)
    set(SRC ${SRC} ${SDL2_SRC} sdl2/SDL2Display_OSX.mm)

endif()

add_library(XliPlatform STATIC ${SRC} ${HEADERS} ${PLATFORM_HEADERS})
