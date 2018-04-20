cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../cmake) # main (top) cmake dir
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake) # project specific cmake dir
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/) # project specific cmake dir
set(CMAKE_CXX_STANDARD 11)

project("graphics")
# Set the version number of your project here (format is MAJOR.MINOR.PATCHLEVEL - e.g. 1.0.0)
set(VERSION_MAJOR "0")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

# Setup subproject so that other projects can find it
include(Common)

set(LIB_HEADERS
    .
    )

set(SRC
    Logger.cpp
    Logger.h
    Shader.cpp
    Shader.h
    View.cpp
    View.h
    Window.cpp
    Window.h
    Element.h
    Element.cpp
    View3D.h
    View3D.cpp
    GeometryPrimitive.cpp
    GeometryPrimitive.h
    Event.h
    Event.cpp
    ResourceManager.cpp ResourceManager.h
    Text.cpp Text.h
    Camera.cpp Camera.h Primitives.h)


set(appSources
    main.cpp
    )

# ---------------------------------------------Create library-------------------------------------------------------
set(LIB_NAME graphics)
add_library(${LIB_NAME} ${SRC} ${INC})
set_target_properties(${LIB_NAME} PROPERTIES
                      VERSION ${VERSION}
                      FRAMEWORK FALSE
                      PUBLIC_HEADER "${LIB_HEADERS}"
                      )
# ------------------------------------------------------------------------------------------------------------------

#message(${CMAKE_SOURCE_DIR})
#target_include_directories (${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# -------------------------------------Copy resources into build----------------------------------------------------
# Copy the resources directory into the build tree. POST_BUILD makes sure this is only run if the build succeeds
add_custom_command(OUTPUT resource_copier.trx POST_BUILD
                   # First remove the directory
                   COMMAND ${CMAKE_COMMAND} -E remove_directory
                   ${CMAKE_CURRENT_BINARY_DIR}/resources/
                   # Add it again. Removing first makes sure files deleted in source also deleted in build
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                            ${CMAKE_CURRENT_SOURCE_DIR}/resources/
                            ${CMAKE_CURRENT_BINARY_DIR}/resources/
                   )
add_custom_target(resource_copier DEPENDS resource_copier.trx)
add_dependencies(graphics resource_copier)
# ------------------------------------------------------------------------------------------------------------------

# -------------------------------------Linking required libraries---------------------------------------------------
# Find required libraries
find_package(glfw3 3.2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(Freetype REQUIRED)
##find_package(SOIL REQUIRED)
#add_subdirectory(extern/SOIL)
#SET(SOIL_INCLUDE_DIRS extern/SOIL/inc/SOIL)
##SET(SOIL_LIBRARIES extern/SOIL/src)

target_include_directories(${LIB_NAME} PUBLIC
                           ${GLFW_INCLUDE_DIRS}
                           ${OPENGL_INCLUDE_DIRS}
                           ${GLEW_INCLUDE_DIRS}
                           ${FREETYPE_INCLUDE_DIRS}
#                           ${SOIL_INCLUDE_DIRS}
                           ./extern  # GLM and rapidjson are header only
                    )


# Link required libraries to the library
target_link_libraries(graphics ${GLFW_LIBRARIES}
                      ${OPENGL_LIBRARIES}
                      ${GLEW_LIBRARIES}
                      glfw
                      ${FREETYPE_LIBRARIES}
#                      ${SOIL_LIBRARIES}
#                      soil
                      )


# Make subproject test executable
# add_executable(testGraphics MACOSX_BUNDLE ${appSources})
add_executable(testGraphics ${appSources})
target_link_libraries(testGraphics ${LIB_NAME})
