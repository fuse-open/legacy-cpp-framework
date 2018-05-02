file(GLOB HEADERS *.h)
file(COPY ${HEADERS} DESTINATION ${PROJECT_SOURCE_DIR}/include/XliHttpClient)

if (WIN32)
    add_definitions(-DCURL_STATICLIB)
    set(SRC curl/CurlHttpClient.cpp)
else()
    set(SRC dummy/DummyHttp.cpp)
endif()

add_library(XliHttpClient ${SRC})
