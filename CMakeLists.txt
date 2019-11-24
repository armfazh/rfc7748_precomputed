cmake_minimum_required(VERSION 3.0.2)
project(rfc7748_precomputed C)

set(EXECUTABLE_OUTPUT_PATH ../bin)
set(LIBRARY_OUTPUT_PATH ../lib)
set(TARGET rfc7748_precomputed)


add_subdirectory(src)
add_subdirectory(samples)
add_subdirectory(tests EXCLUDE_FROM_ALL)
add_subdirectory(bench EXCLUDE_FROM_ALL)
add_subdirectory(fuzz)
