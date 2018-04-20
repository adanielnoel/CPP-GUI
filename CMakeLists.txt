cmake_minimum_required(VERSION 2.8)

project(Simulator)
include(cmake/Common.cmake)
include(cmake/DoxygenTarget.cmake)

option(BUILD_DOC "Build documentation" ON)
PrepareDocTarget()
#option(BUILD_DOCUMENTATION "Use Doxygen to create the HTML based API documentation" OFF)
#if(BUILD_DOCUMENTATION)
#    add_custom_target(doc)
#endif()

include_directories(./extern)

add_subdirectory(graphics)
add_subdirectory(PDESolver)

