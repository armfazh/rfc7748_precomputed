cmake_minimum_required(VERSION 2.8.4)

add_custom_command(
		OUTPUT new_clang
		COMMENT "Downloading and setting up the Clang compiler for fuzzing."
		COMMAND mkdir -p TMP_CLANG
		COMMAND cd TMP_CLANG && git clone https://chromium.googlesource.com/chromium/src/tools/clang
		COMMAND ./TMP_CLANG/clang/scripts/update.py
		COMMAND ln -s ./third_party/llvm-build/Release+Asserts/bin/clang new_clang
		)

add_custom_target(get_fuzzer DEPENDS new_clang)

include_directories(../include)
include_directories(./hacl)

set(CMAKE_C_COMPILER ./new_clang)
set(CMAKE_C_FLAGS "-fsanitize=fuzzer,address")

add_custom_target(fuzz_hacl)
file(GLOB FUZZ_HACL_SOURCES ./hacl_*.c)
foreach (file ${FUZZ_HACL_SOURCES})
	get_filename_component(tmp_name ${file} NAME_WE)
	add_executable(fuzz_${tmp_name} EXCLUDE_FROM_ALL ${file} ./hacl/Hacl_Curve25519.c)
	add_dependencies(fuzz_${tmp_name} get_fuzzer)
	target_link_libraries(fuzz_${tmp_name} ${TARGET})
	add_dependencies(fuzz_hacl fuzz_${tmp_name})
endforeach (file ${FUZZ_HACL_SOURCES})

add_custom_target(fuzz_gmp)
file(GLOB FUZZ_GMP_SOURCES ./gmp_fp25519_*.c)
foreach (file ${FUZZ_GMP_SOURCES})
	get_filename_component(tmp_name ${file} NAME_WE)
	add_executable(fuzz_${tmp_name} EXCLUDE_FROM_ALL ${file})
	add_dependencies(fuzz_${tmp_name} get_fuzzer)
	target_link_libraries(fuzz_${tmp_name} ${TARGET} gmp)
	add_dependencies(fuzz_gmp fuzz_${tmp_name})
endforeach (file ${FUZZ_GMP_SOURCES})

add_custom_target(fuzz)
add_dependencies(fuzz fuzz_hacl fuzz_gmp)
