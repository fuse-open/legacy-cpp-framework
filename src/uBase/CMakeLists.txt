file(GLOB SRC *.cpp
    ../../3rdparty/ConvertUTF/ConvertUTF.h
    ../../3rdparty/ConvertUTF/ConvertUTF.c)
file(GLOB HEADERS *.h)
file(COPY ${HEADERS} DESTINATION ${PROJECT_SOURCE_DIR}/include/uBase)
file(COPY ../../3rdparty/ConvertUTF/ConvertUTF.h DESTINATION ${PROJECT_SOURCE_DIR}/include)

if (WIN32)
    file(GLOB MORE_SRC win32/*.cpp generic/*.cpp)
    set(SRC ${SRC} ${MORE_SRC})
else()
    file(GLOB MORE_SRC unix/*.cpp)
    set(SRC ${SRC} ${MORE_SRC})
endif()

if (LINUX)
    file(GLOB MORE_SRC linux/*.cpp generic/*.cpp)
    set(SRC ${SRC} ${MORE_SRC})
elseif (APPLE)
    file(GLOB MORE_SRC apple/*.mm)
    set(SRC ${SRC} ${MORE_SRC})
elseif (ANDROID)
    file(GLOB MORE_SRC android/*.cpp)
    set(SRC ${SRC} ${MORE_SRC})
endif()

add_library(uBase STATIC ${SRC} ${HEADERS})
