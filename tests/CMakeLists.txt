cmake_minimum_required(VERSION 3.0.2)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 11)

include(ExternalProject)
include("GNUInstallDirs")
set(GTEST_VERSION "release-1.10.0")
ExternalProject_Add(googletest-download
  URL            "https://api.github.com/repos/google/googletest/tarball/${GTEST_VERSION}"
  CMAKE_ARGS     -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}
                 -DCMAKE_POSITION_INDEPENDENT_CODE=ON
                 -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
  TEST_COMMAND   ""
  UPDATE_COMMAND ""
)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR})
link_directories(${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})

find_package(GMP QUIET)
include_directories(../include)
include_directories(../third_party)
set(c_files
    runTests.cpp
    test_fp25519_x64.cpp
    test_fp448_x64.cpp
    test_x25519.cpp
    test_x448.cpp
)

add_executable(tests ${c_files} ../third_party/random.c)
add_dependencies(tests ${TARGET} googletest-download)
target_link_libraries(tests ${TARGET} gtest  pthread gmp)
