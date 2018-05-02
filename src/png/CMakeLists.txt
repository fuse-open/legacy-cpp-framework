if (LINUX)
    return()
endif()

if (MSVC)
    add_definitions("/wd4996")
endif()

set(DIR ${PROJECT_SOURCE_DIR}/3rdparty/libpng)
configure_file(${DIR}/scripts/pnglibconf.h.prebuilt ${DIR}/pnglibconf.h)
file(GLOB HEADERS ${DIR}/*.h)
file(COPY ${HEADERS} DESTINATION ${PROJECT_SOURCE_DIR}/include)
add_library(png STATIC
    ${DIR}/arm/arm_init.c
    ${DIR}/arm/filter_neon_intrinsics.c
    ${DIR}/arm/filter_neon.S
    ${DIR}/png.c
    ${DIR}/pngerror.c
    ${DIR}/pngget.c
    ${DIR}/pngmem.c
    ${DIR}/pngpread.c
    ${DIR}/pngread.c
    ${DIR}/pngrio.c
    ${DIR}/pngrtran.c
    ${DIR}/pngrutil.c
    ${DIR}/pngset.c
    ${DIR}/pngtrans.c
    ${DIR}/pngwio.c
    ${DIR}/pngwrite.c
    ${DIR}/pngwtran.c
    ${DIR}/pngwutil.c)
target_link_libraries(png z)
