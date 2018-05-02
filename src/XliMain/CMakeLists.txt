if (IOS)
    include_directories(../../3rdparty/SDL2/include)
endif()

add_library(XliMain STATIC Main.cpp)
