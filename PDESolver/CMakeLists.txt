cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 11)


project(PDESolver)

set(APP_SOURCES
    main.cpp Viewer.cpp Viewer.h)

set(TEST_SOURCES
    Tests/GridTest.cpp)

set(COMMON_SOURCES
    Region.cpp
    Region.h
    LinearAlgebra.h
    )

include_directories(.
                    ..
                    )

add_executable(PDESolver ${APP_SOURCES} ${COMMON_SOURCES})
target_link_libraries(PDESolver graphics)
add_executable(TEST_PDESolver ${TEST_SOURCES} ${COMMON_SOURCES})