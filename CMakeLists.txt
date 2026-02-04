cmake_minimum_required(VERSION 3.16)
project(hello)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendored/libfixmath)
add_subdirectory(vendored/libfixmatrix)
add_subdirectory(src/)

# If the `ray` target exists, link it to vendored libraries and SDL.
if(TARGET ray)
	target_link_libraries(ray PRIVATE SDL3::SDL3 fixmatrix fixmath)
endif()