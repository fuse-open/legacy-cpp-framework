if (NOT WIN32)
    return()
endif()

set(DIR ${PROJECT_SOURCE_DIR}/3rdparty/curl)
configure_file(${DIR}/include/curl/curlbuild.h.dist
               ${DIR}/include/curl/curlbuild.h)

file(GLOB SRC
    ${DIR}/lib/*.c
    ${DIR}/lib/vtls/*.c)
file(GLOB HEADERS ${DIR}/include/curl/*.h)
file(COPY ${HEADERS} DESTINATION ${PROJECT_SOURCE_DIR}/include/curl)

include_directories(${DIR}/lib)
add_definitions(-DCURL_STATICLIB -DUSE_IPV6 -DUSE_WINDOWS_SSPI -DUSE_SCHANNEL)
add_library(curl STATIC ${SRC})
