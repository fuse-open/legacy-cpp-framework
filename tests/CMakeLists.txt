project("tests")
cmake_minimum_required(VERSION 2.6)
include(../cmake/modules/link_project.cmake)
link_project(${PROJECT_SOURCE_DIR}/..)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
file(GLOB SRC uBase/*.cpp *.cpp)

add_executable(base-test ${SRC})
target_link_libraries(base-test uBase uImage jpeg)
