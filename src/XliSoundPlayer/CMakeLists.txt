file(GLOB HEADERS *.h)
file(COPY ${HEADERS} DESTINATION ${PROJECT_SOURCE_DIR}/include/XliSoundPlayer)

if (IOS)
    set(SRC ${SRC} coreaudio/CoreSoundPlayer.mm)
elseif (OSX)
    set(SRC ${SRC} coreaudio/CoreSoundPlayer.mm)
elseif (ANDROID)
    set(SRC ${SRC} opensles/SlesSoundPlayer.cpp)
else()
    set(SRC ${SRC} dummy/DummySoundPlayer.cpp)
endif()

add_library(XliSoundPlayer STATIC ${SRC} ${HEADERS})
