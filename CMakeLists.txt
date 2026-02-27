cmake_minimum_required(VERSION 3.16)
project(RayTracer)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# Create your game executable target as usual
add_executable(RayTracer WIN32
    RayTracer.c
    lib/Fix.c
    lib/Vec3.c
    lib/RayRenderer.c
)

target_compile_options(RayTracer PRIVATE
    -O2
    -Wall
    -Werror
)

# Link to the actual SDL3 library.
target_link_libraries(RayTracer PRIVATE SDL3::SDL3)