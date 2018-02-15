## See:
##    https://llvm.org/docs/LibFuzzer.html
##    https://releases.llvm.org/5.0.0/docs/LibFuzzer.html

FUZZER=./third_party/llvm-build/Release+Asserts/bin/clang -fsanitize=fuzzer,address

CODE_A= -I./hacl ./hacl/Hacl_Curve25519.c
CODE_B=	-I../include/ ../build/lib/librfc7748_precomputed.a
CFLAGS+= -g -O3 -Wall -Wextra -fno-omit-frame-pointer

all: fuzz_keygen fuzz_shared

get_fuzzer:
	mkdir TMP_CLANG
	cd TMP_CLANG; git clone https://chromium.googlesource.com/chromium/src/tools/clang
	./TMP_CLANG/clang/scripts/update.py

fuzz_%: fuzz_%.c
	$(FUZZER) -o $@ $(CFLAGS) $^ $(CODE_A) $(CODE_B)

clean:
	rm -f fuzz_keygen fuzz_shared

cleanall:
	rm -rf ./TMP_CLANG ./third_party

