cmake_minimum_required(VERSION 3.0.2)

include_directories(../third_party)

add_executable(sample_x25519 sample_x25519.c ../third_party/random.c)
add_executable(sample_x448 sample_x448.c ../third_party/random.c)
add_dependencies(sample_x25519 ${TARGET})
add_dependencies(sample_x448 ${TARGET})
target_link_libraries(sample_x25519	${TARGET})
target_link_libraries(sample_x448 ${TARGET})
