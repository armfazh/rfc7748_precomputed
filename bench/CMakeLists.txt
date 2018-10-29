cmake_minimum_required(VERSION 3.0.2)

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
target_link_libraries(bench	${TARGET})
