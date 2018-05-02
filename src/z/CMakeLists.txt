if (NOT WIN32)
    return()
endif()

set(DIR ${PROJECT_SOURCE_DIR}/3rdparty/zlib)
file(GLOB HEADERS ${DIR}/*.h)
file(COPY ${HEADERS} DESTINATION ${PROJECT_SOURCE_DIR}/include)
add_library(z STATIC
    ${DIR}/adler32.c
    ${DIR}/compress.c
    ${DIR}/crc32.c
    ${DIR}/deflate.c
    ${DIR}/infback.c
    ${DIR}/inffast.c
    ${DIR}/inflate.c
    ${DIR}/inftrees.c
    ${DIR}/trees.c
    ${DIR}/uncompr.c
    ${DIR}/zutil.c)
