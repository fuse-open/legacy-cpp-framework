if (LINUX)
    return()
endif()

if (MSVC)
    add_definitions("/wd4018 /wd4267 /wd4244 /wd4996")
endif()

set(DIR ${PROJECT_SOURCE_DIR}/3rdparty/freetype2)
include_directories(${DIR}/lib)
add_definitions(-DFT2_BUILD_LIBRARY -DDARWIN_NO_CARBON -DFT_CONFIG_OPTION_USE_PNG)
file(COPY ${DIR}/include/freetype DESTINATION ${PROJECT_SOURCE_DIR}/include)
file(COPY ${DIR}/include/ft2build.h DESTINATION ${PROJECT_SOURCE_DIR}/include)
add_library(freetype STATIC
    ${DIR}/src/autofit/autofit.c
    ${DIR}/src/base/ftapi.c
    ${DIR}/src/base/ftbase.c
    ${DIR}/src/base/ftbdf.c
    ${DIR}/src/base/ftbitmap.c
    ${DIR}/src/base/ftdebug.c
    ${DIR}/src/base/ftgasp.c
    ${DIR}/src/base/ftglyph.c
    ${DIR}/src/base/ftgxval.c
    ${DIR}/src/base/ftinit.c
    ${DIR}/src/base/ftlcdfil.c
    ${DIR}/src/base/ftmm.c
    ${DIR}/src/base/ftotval.c
    ${DIR}/src/base/ftpatent.c
    ${DIR}/src/base/ftpfr.c
    ${DIR}/src/base/ftstroke.c
    ${DIR}/src/base/ftsynth.c
    ${DIR}/src/base/ftsystem.c
    ${DIR}/src/base/fttype1.c
    ${DIR}/src/base/ftwinfnt.c
    ${DIR}/src/bdf/bdf.c
    ${DIR}/src/cache/ftcache.c
    ${DIR}/src/cff/cff.c
    ${DIR}/src/cid/type1cid.c
    ${DIR}/src/gxvalid/gxvalid.c
    ${DIR}/src/gzip/ftgzip.c
    ${DIR}/src/lzw/ftlzw.c
    ${DIR}/src/otvalid/otvalid.c
    ${DIR}/src/pcf/pcf.c
    ${DIR}/src/pfr/pfr.c
    ${DIR}/src/pfr/pfrgload.c
    ${DIR}/src/pfr/pfrload.c
    ${DIR}/src/pfr/pfrobjs.c
    ${DIR}/src/pfr/pfrsbit.c
    ${DIR}/src/psaux/psaux.c
    ${DIR}/src/pshinter/pshinter.c
    ${DIR}/src/psnames/psnames.c
    ${DIR}/src/raster/raster.c
    ${DIR}/src/sfnt/pngshim.c
    ${DIR}/src/sfnt/sfnt.c
    ${DIR}/src/smooth/smooth.c
    ${DIR}/src/truetype/truetype.c
    ${DIR}/src/type1/type1.c
    ${DIR}/src/type42/t42drivr.c
    ${DIR}/src/type42/t42objs.c
    ${DIR}/src/type42/t42parse.c
    ${DIR}/src/winfonts/winfnt.c)
