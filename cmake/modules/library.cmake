include(${CMAKE_CURRENT_LIST_DIR}/config.cmake)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fexceptions")

if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")
endif()

if (CMAKE_BUILD_TYPE MATCHES "Release")
    add_definitions(-DNDEBUG)
endif()

if (IOS)

    set(CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "7.0" CACHE STRING "Set the minimum deployment target value." FORCE)

    if (IOS_PLATFORM MATCHES OS)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${LIBRARY_OUTPUT_PATH}/Release-iphoneos)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${LIBRARY_OUTPUT_PATH}/Debug-iphoneos)
    elseif (IOS_PLATFORM MATCHES SIMULATOR OR IOS_PLATFORM MATCHES SIMULATOR64)
        set(CMAKE_OSX_ARCHITECTURES "x86_64;i386")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=core2")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=core2")
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${LIBRARY_OUTPUT_PATH}/Release-iphonesimulator)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${LIBRARY_OUTPUT_PATH}/Debug-iphonesimulator)
    endif()

elseif (OSX)

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=core2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=core2")
    set(CMAKE_OSX_ARCHITECTURES "x86_64;i386")
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9")
    set(CMAKE_MACOSX_RPATH 1)

elseif (ANDROID)

    set(LIBRARY_OUTPUT_PATH ${LIBRARY_OUTPUT_PATH}/${ANDROID_ABI})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -frtti")

elseif (LINUX)

    # Fixes linker problem on Ubuntu
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -fno-stack-protector")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fno-stack-protector")

    if (ARM)
        # GLES on RPi
        include_directories(/opt/vc/include)
        link_directories(/opt/vc/lib)
    endif()

elseif (WIN32)

    if (MSVC)
        set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Solution Configurations" FORCE)
        set(CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /DUNICODE /D_UNICODE /W3 /GR- /EHsc /MP /Fd$(TargetDir)$(TargetName).pdb" CACHE STRING "C++ Compiler Flags" FORCE)
        set(CMAKE_C_FLAGS "/DWIN32 /D_WINDOWS /DUNICODE /D_UNICODE /W3 /MP /Fd$(TargetDir)$(TargetName).pdb" CACHE STRING "C Compiler Flags" FORCE)
    endif()

else()

    message(FATAL_ERROR "Unsupported system")

endif()

if (CMAKE_GENERATOR MATCHES "Unix Makefiles")

    install(DIRECTORY ${LIBRARY_OUTPUT_PATH}/
            DESTINATION lib
            PATTERN "Debug" EXCLUDE
            PATTERN "Release" EXCLUDE)
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
            DESTINATION include)

endif()
