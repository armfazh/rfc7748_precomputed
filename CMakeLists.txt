cmake_minimum_required(VERSION 2.8.4)
project(rfc7748_precomputed C)

set(EXECUTABLE_OUTPUT_PATH ../bin)
set(PROJECT_FLAGS "-Wall -Wextra -O3 -pedantic -std=c99 -mbmi -mbmi2 -march=native -mtune=native")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  ${PROJECT_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}  ${PROJECT_FLAGS}")
set(TARGET rfc7748_precomputed)

include_directories(./include)
set(LIBRARY_OUTPUT_PATH ../lib)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(bench)
add_subdirectory(fuzz)
add_subdirectory(samples)
