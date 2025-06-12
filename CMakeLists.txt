cmake_minimum_required(VERSION 3.10)
project(MyQxGLProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

add_subdirectory(lib/qxgl)

file(GLOB_RECURSE PROJECT_SOURCES
    src/*.cpp
    src/*.h
    src/*.hpp
)

add_executable(MyQxGLProject ${PROJECT_SOURCES})

target_link_libraries(MyQxGLProject
    PRIVATE
        qxgl
        GLU
)
