cmake_minimum_required(VERSION 2.8.4)

include_directories(../util)

add_executable(sample_x25519 sample_x25519.c ../util/random.c)
add_executable(sample_x448 sample_x448.c ../util/random.c)
add_dependencies(sample_x25519 ${TARGET})
add_dependencies(sample_x448 ${TARGET})
target_link_libraries(sample_x25519	${TARGET})
target_link_libraries(sample_x448 ${TARGET})
