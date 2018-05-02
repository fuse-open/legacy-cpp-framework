if (CMAKE_SYSTEM_NAME MATCHES "Darwin")

    if (NOT IOS)
        set(OSX TRUE)
    endif()

elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")

    set(LINUX TRUE)
    execute_process(COMMAND uname -m OUTPUT_VARIABLE MACHINE)

    if (${MACHINE} MATCHES "arm*")
        set(ARM TRUE)
    endif()

endif()
