file(GLOB SRC *.cpp *.h)

if (MSVC)
    add_definitions("/wd4267 /wd4715 /wd4005 /wd4018 /wd4090 /wd4244 /wd4273 /wd4291 /wd4351 /wd4800 /wd4805 /wd4996")
endif()

add_library(uPInvoke SHARED ${SRC})
target_link_libraries(uPInvoke uBase uImage freetype jpeg png z)

if (LINUX)
    target_link_libraries(uPInvoke pthread)
    if (ARM)
        target_link_libraries(uPInvoke rt)
    endif()
elseif (OSX)
    target_link_libraries(uPInvoke bz2)
endif()
