cmake_minimum_required(VERSION 2.8.4)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 11)
include_directories(../util)

add_library(all_tests OBJECT
		test_fp25519_x64.cpp
		test_fp448_x64.cpp
		test_x25519.cpp
		test_x448.cpp )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3 -pedantic -march=native -mtune=native")

find_package(GMP QUIET)
# Locate GTest
find_package(GTest REQUIRED)
link_directories(${GTEST_LIBRARY_DIRS})

if(GTest_FOUND)
	include_directories(${GTEST_INCLUDE_DIRS})
	add_executable(tests runTests.cpp $<TARGET_OBJECTS:all_tests> ../util/random.c)
	add_dependencies(tests ${TARGET})
	target_link_libraries(tests ${GTEST_LIBRARIES} ${TARGET} gmp)
endif()
