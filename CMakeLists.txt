cmake_minimum_required(VERSION 3.5)
project(OpenGLExample)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG latest
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
        GIT_TAG master
)
FetchContent_MakeAvailable(glad)

add_executable(${PROJECT_NAME} main.cpp
        Vector.cpp
        Vector.h
        Matrix4x4.h
        Matrix4x4.cpp
        Quaternion.cpp
        Quaternion.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)
elseif(APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OpenGL_LIBRARY OpenGL)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${COCOA_LIBRARY} ${OpenGL_LIBRARY})
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
endif()
