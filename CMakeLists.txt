cmake_minimum_required(VERSION 3.22)
project(algo_animator)

set(SOURCES src/main.c)
set(FREETYPE_DIR /usr/local/include/freetype2)

include_directories(${FREETYPE_DIR})

add_executable(algo_animator ${SOURCES})

target_link_libraries(algo_animator
    glut
    GL
    GLU
    m
    freetype
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/fonts DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/build)
