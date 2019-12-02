cmake_minimum_required(VERSION 3.0.2)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 11)

include(ExternalProject)
include("GNUInstallDirs")
set(GBENCH_VERSION "v1.5.0")
ExternalProject_Add(benchmark-download
  URL            "https://api.github.com/repos/google/benchmark/tarball/${GBENCH_VERSION}"
  CMAKE_ARGS     -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}
                 -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
                 -DCMAKE_POSITION_INDEPENDENT_CODE=ON
                 -DCMAKE_BUILD_TYPE=Release
                 -DBENCHMARK_ENABLE_TESTING=OFF
                 -DBENCHMARK_ENABLE_GTEST_TESTS=OFF
  TEST_COMMAND   ""
  UPDATE_COMMAND ""
)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR})
link_directories(${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})

include_directories(../include)
include_directories(../third_party)

add_executable(bench
    clocks.h
    clocks.c
    bench.h
    bench_fp25519_x64.c
    bench_fp448_x64.c
    bench_x25519.c
    bench_x448.c
    bench.c
    ../third_party/random.c
)
add_dependencies(bench ${TARGET})
target_link_libraries(bench ${TARGET})

add_executable(gbench gbench.cpp ../third_party/random.c)
add_dependencies(gbench ${TARGET} benchmark-download)
target_link_libraries(gbench ${TARGET} benchmark pthread)
