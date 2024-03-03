cmake_minimum_required(VERSION 3.22)
project(algo_animator)

# Set the path to the FreeType library
set(FREETYPE_DIR /usr/local/Cellar/freetype/2.13.2)

# Find OpenGL package
find_package(OpenGL REQUIRED)

# Include directories for headers
include_directories(${FREETYPE_DIR}/include/freetype2)

# Link directories for libraries
link_directories(${FREETYPE_DIR}/lib)

# Add your source files
add_executable(algo_animator 
    src/main.c
    src/algorithms.c
    src/algorithms.h
    src/utils.c
    src/utils.h
)

# Link libraries
target_link_libraries(algo_animator
    ${OPENGL_LIBRARIES}
    "-framework OpenGL"
    "-framework GLUT"
    "-framework Cocoa"
    freetype
)

# Copy the 'fonts' directory to the build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/fonts DESTINATION out/)
