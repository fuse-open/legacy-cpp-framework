if (LINUX)
    return()
endif()

if (MSVC)
    add_definitions("/wd4013 /wd4267 /wd4996")
endif()

set(DIR ${PROJECT_SOURCE_DIR}/3rdparty/libjpeg)
configure_file(${DIR}/jconfig.txt ${DIR}/jconfig.h)
file(GLOB HEADERS ${DIR}/*.h)
file(COPY ${HEADERS} DESTINATION ${PROJECT_SOURCE_DIR}/include)
add_library(jpeg STATIC
    ${DIR}/jaricom.c
    ${DIR}/jcapimin.c
    ${DIR}/jcapistd.c
    ${DIR}/jcarith.c
    ${DIR}/jccoefct.c
    ${DIR}/jccolor.c
    ${DIR}/jcdctmgr.c
    ${DIR}/jchuff.c
    ${DIR}/jcinit.c
    ${DIR}/jcmainct.c
    ${DIR}/jcmarker.c
    ${DIR}/jcmaster.c
    ${DIR}/jcomapi.c
    ${DIR}/jcparam.c
    ${DIR}/jchuff.c
    ${DIR}/jcprepct.c
    ${DIR}/jcsample.c
    ${DIR}/jctrans.c
    ${DIR}/jdapimin.c
    ${DIR}/jdapistd.c
    ${DIR}/jdarith.c
    ${DIR}/jdatadst.c
    ${DIR}/jdatasrc.c
    ${DIR}/jdcoefct.c
    ${DIR}/jdcolor.c
    ${DIR}/jddctmgr.c
    ${DIR}/jdhuff.c
    ${DIR}/jdinput.c
    ${DIR}/jdmainct.c
    ${DIR}/jdmarker.c
    ${DIR}/jdmaster.c
    ${DIR}/jdmerge.c
    ${DIR}/jdpostct.c
    ${DIR}/jdsample.c
    ${DIR}/jdtrans.c
    ${DIR}/jerror.c
    ${DIR}/jfdctflt.c
    ${DIR}/jfdctfst.c
    ${DIR}/jfdctint.c
    ${DIR}/jidctflt.c
    ${DIR}/jidctfst.c
    ${DIR}/jidctint.c
    ${DIR}/jmemmgr.c
    ${DIR}/jmemnobs.c
    ${DIR}/jquant1.c
    ${DIR}/jquant2.c
    ${DIR}/jutils.c)
