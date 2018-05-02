include(${CMAKE_CURRENT_LIST_DIR}/config.cmake)

function(link_project PROJECT_DIR)

    include_directories(${PROJECT_DIR}/include)

    if (MSVC)
        if (MSVC12)
            link_directories("${PROJECT_DIR}/lib/vs2013/$(PlatformShortName)")
        elseif (MSVC11)
            link_directories("${PROJECT_DIR}/lib/vs2012/$(PlatformShortName)")
        elseif (MSVC10)
            link_directories("${PROJECT_DIR}/lib/vs2010/$(PlatformShortName)")
        else()
            message(FATAL_ERROR "Unsupported MSVC version")
        endif()
    else()
        link_directories(${PROJECT_DIR}/lib)
    endif()

endfunction()
