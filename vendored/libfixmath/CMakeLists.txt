cmake_minimum_required(VERSION 3.10)
project(fixmath C)

file(GLOB FIXMATH_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

add_library(fixmath STATIC ${FIXMATH_SRCS})
target_include_directories(fixmath PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(fixmath PROPERTIES
    OUTPUT_NAME fixmath
)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
