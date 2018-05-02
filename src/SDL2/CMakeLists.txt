if (OSX)
    set(SDL_SHARED OFF CACHE BOOL "Build a shared version of the library")
    set(DIR ${PROJECT_SOURCE_DIR}/3rdparty/sdl2)
    add_subdirectory(${DIR} ${CMAKE_BINARY_DIR}/SDL2)

    file(GLOB SDL2_HEADERS ${DIR}/include/*.h)
    file(COPY ${SDL2_HEADERS} DESTINATION ${PROJECT_SOURCE_DIR}/include/SDL2)
endif()
