project("pinvoke")
cmake_minimum_required(VERSION 2.8.9)
include(../cmake/modules/library.cmake)
include(../cmake/modules/link_project.cmake)
link_project(..)

if (APPLE)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -lobjc -Wl,-framework,Cocoa")
elseif (MSVC)
    set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/$(PlatformShortName))

    # Copy VC runtime DLLs
    file(COPY $ENV{WINDIR}/System32/msvcr120.dll DESTINATION ${PROJECT_SOURCE_DIR}/lib/x86/Release)
    file(COPY $ENV{WINDIR}/SysWOW64/msvcr120.dll DESTINATION ${PROJECT_SOURCE_DIR}/lib/x64/Release)
endif()

add_subdirectory(uPInvoke)
