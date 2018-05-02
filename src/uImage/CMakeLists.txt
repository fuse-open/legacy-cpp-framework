file(GLOB SRC *.cpp)
file(GLOB HEADERS *.h)
file(COPY ${HEADERS} DESTINATION ${PROJECT_SOURCE_DIR}/include/uImage)

if (LINUX)
    include_directories(/usr/include/freetype2)
endif()

add_library(uImage STATIC ${SRC} ${HEADERS})
